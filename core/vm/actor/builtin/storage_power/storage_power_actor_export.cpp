/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "vm/actor/builtin/storage_power/storage_power_actor_export.hpp"

#include "vm/actor/builtin/init/init_actor.hpp"
#include "vm/actor/builtin/miner/miner_actor.hpp"
#include "vm/actor/builtin/shared/shared.hpp"

namespace fc::vm::actor::builtin::storage_power {
  using fc::primitives::SectorStorageWeightDesc;
  using fc::primitives::TokenAmount;
  using fc::primitives::address::decode;
  using fc::vm::actor::ActorExports;
  using fc::vm::actor::kConstructorMethodNumber;
  using fc::vm::actor::builtin::requestMinerControlAddress;
  using fc::vm::actor::builtin::storage_power::kEpochTotalExpectedReward;
  using fc::vm::actor::builtin::storage_power::kPledgeFactor;
  using fc::vm::actor::builtin::storage_power::StoragePower;
  using fc::vm::actor::builtin::storage_power::StoragePowerActor;
  using fc::vm::actor::builtin::storage_power::StoragePowerActorState;
  using fc::vm::runtime::InvocationOutput;
  using fc::vm::runtime::Runtime;

  outcome::result<StoragePowerActor> getCurrentState(Runtime &runtime) {
    auto datastore = runtime.getIpfsDatastore();
    OUTCOME_TRY(state,
                runtime.getCurrentActorStateCbor<StoragePowerActorState>());
    return StoragePowerActor(datastore, state);
  }

  outcome::result<InvocationOutput> slashPledgeCollateral(
      Runtime &runtime,
      StoragePowerActor &power_actor,
      Address miner,
      TokenAmount to_slash) {
    OUTCOME_TRY(
        slashed,
        power_actor.subtractMinerBalance(miner, to_slash, TokenAmount{0}));
    OUTCOME_TRY(runtime.sendFunds(kBurntFundsActorAddress, slashed));

    return outcome::success();
  }

  outcome::result<void> deleteMinerActor(Runtime &runtime,
                                         StoragePowerActor &state,
                                         const Address &miner) {
    OUTCOME_TRY(amount_slashed, state.deleteMiner(miner));
    OUTCOME_TRY(miner::OnDeleteMiner::send(runtime, miner, {}, 0));
    OUTCOME_TRY(runtime.sendFunds(kBurntFundsActorAddress, amount_slashed));
    return outcome::success();
  }

  const ActorExports exports{
      Construct::pair(),
      AddBalance::pair(),
      WithdrawBalance::pair(),
      CreateMiner::pair(),
      DeleteMiner::pair(),
      OnSectorProveCommit::pair(),
      OnSectorTerminate::pair(),
      OnSectorTerminate::pair(),
  };
}  // namespace fc::vm::actor::builtin::storage_power

namespace fc::vm::actor {
  using namespace builtin::storage_power;

  ACTOR_METHOD_IMPL(Construct) {
    if (runtime.getImmediateCaller() != kSystemActorAddress)
      return VMExitCode::STORAGE_POWER_ACTOR_WRONG_CALLER;

    auto datastore = runtime.getIpfsDatastore();
    OUTCOME_TRY(empty_state, StoragePowerActor::createEmptyState(datastore));

    OUTCOME_TRY(runtime.commitState(empty_state));
    return fc::outcome::success();
  }

  ACTOR_METHOD_IMPL(AddBalance) {
    OUTCOME_TRY(miner_code_cid, runtime.getActorCodeID(params.miner));
    if (miner_code_cid != kStorageMinerCodeCid)
      return VMExitCode::STORAGE_POWER_ILLEGAL_ARGUMENT;

    Address immediate_caller = runtime.getImmediateCaller();
    OUTCOME_TRY(control_addresses,
                requestMinerControlAddress(runtime, params.miner));
    if (immediate_caller != control_addresses.owner
        && immediate_caller != control_addresses.worker)
      return VMExitCode::STORAGE_POWER_FORBIDDEN;

    OUTCOME_TRY(power_actor, getCurrentState(runtime));
    OUTCOME_TRY(power_actor.addMinerBalance(params.miner,
                                            runtime.getMessage().get().value));

    OUTCOME_TRY(power_actor_state, power_actor.flushState());
    OUTCOME_TRY(runtime.commitState(power_actor_state));
    return fc::outcome::success();
  }

  ACTOR_METHOD_IMPL(WithdrawBalance) {
    OUTCOME_TRY(miner_code_cid, runtime.getActorCodeID(params.miner));
    if (miner_code_cid != kStorageMinerCodeCid)
      return VMExitCode::STORAGE_POWER_ILLEGAL_ARGUMENT;

    Address immediate_caller = runtime.getImmediateCaller();
    OUTCOME_TRY(control_addresses,
                requestMinerControlAddress(runtime, params.miner));
    if (immediate_caller != control_addresses.owner
        && immediate_caller != control_addresses.worker)
      return VMExitCode::STORAGE_POWER_FORBIDDEN;

    if (params.requested < TokenAmount{0})
      return VMExitCode::STORAGE_POWER_ILLEGAL_ARGUMENT;

    OUTCOME_TRY(power_actor, getCurrentState(runtime));

    if (!power_actor.hasClaim(params.miner))
      return VMExitCode::STORAGE_POWER_ILLEGAL_ARGUMENT;
    OUTCOME_TRY(claim, power_actor.getClaim(params.miner));

    /*
     * Pledge for sectors in temporary fault has already been subtracted from
     * the claim. If the miner has failed a scheduled PoSt, collateral remains
     * locked for further penalization. Thus the current claimed pledge is the
     * amount to keep locked.
     */
    OUTCOME_TRY(subtracted,
                power_actor.subtractMinerBalance(
                    params.miner, params.requested, claim.pledge));

    OUTCOME_TRY(runtime.sendFunds(control_addresses.owner, subtracted));

    OUTCOME_TRY(power_actor_state, power_actor.flushState());
    OUTCOME_TRY(runtime.commitState(power_actor_state));
    return fc::outcome::success();
  }

  ACTOR_METHOD_IMPL(CreateMiner) {
    OUTCOME_TRY(immediate_caller_code_id,
                runtime.getActorCodeID(runtime.getImmediateCaller()));
    if (!isSignableActor((immediate_caller_code_id)))
      return VMExitCode::STORAGE_POWER_FORBIDDEN;

    auto message = runtime.getMessage().get();
    builtin::miner::Construct::Params construct_miner_parameters{
        message.from, params.worker, params.sector_size, params.peer_id};
    OUTCOME_TRY(encoded_construct_miner_parameters,
                encodeActorParams(construct_miner_parameters));
    OUTCOME_TRY(addresses_created,
                builtin::init::Exec::M::send(
                    runtime,
                    kInitAddress,
                    {
                        .code = kStorageMinerCodeCid,
                        .params = encoded_construct_miner_parameters,
                    },
                    0));

    OUTCOME_TRY(power_actor, getCurrentState(runtime));
    OUTCOME_TRY(power_actor.addMiner(addresses_created.id_address));
    OUTCOME_TRY(power_actor.setMinerBalance(addresses_created.id_address,
                                            message.value));

    OUTCOME_TRY(power_actor_state, power_actor.flushState());
    OUTCOME_TRY(runtime.commitState(power_actor_state));

    return Result{addresses_created.id_address,
                  addresses_created.robust_address};
  }

  ACTOR_METHOD_IMPL(DeleteMiner) {
    Address immediate_caller = runtime.getImmediateCaller();
    OUTCOME_TRY(control_addresses,
                requestMinerControlAddress(runtime, params.miner));
    if (immediate_caller != control_addresses.owner
        && immediate_caller != control_addresses.worker)
      return VMExitCode::STORAGE_POWER_FORBIDDEN;

    OUTCOME_TRY(power_actor, getCurrentState(runtime));

    OUTCOME_TRY(deleteMinerActor(runtime, power_actor, params.miner));

    OUTCOME_TRY(power_actor_state, power_actor.flushState());
    OUTCOME_TRY(runtime.commitState(power_actor_state));
    return fc::outcome::success();
  }

  ACTOR_METHOD_IMPL(OnSectorProveCommit) {
    OUTCOME_TRY(immediate_caller_code_id,
                runtime.getActorCodeID(runtime.getImmediateCaller()));
    if (immediate_caller_code_id != kStorageMinerCodeCid)
      return VMExitCode::STORAGE_POWER_ACTOR_WRONG_CALLER;

    OUTCOME_TRY(power_actor, getCurrentState(runtime));

    StoragePower power = consensusPowerForWeight(params.weight);
    OUTCOME_TRY(network_power, power_actor.getTotalNetworkPower());
    TokenAmount pledge = pledgeForWeight(params.weight, network_power);
    OUTCOME_TRY(
        power_actor.addToClaim(runtime.getMessage().get().from, power, pledge));

    OUTCOME_TRY(power_actor_state, power_actor.flushState());
    OUTCOME_TRY(runtime.commitState(power_actor_state));

    return Result{.pledge = pledge};
  }

  ACTOR_METHOD_IMPL(OnSectorTerminate) {
    OUTCOME_TRY(immediate_caller_code_id,
                runtime.getActorCodeID(runtime.getImmediateCaller()));
    if (immediate_caller_code_id != kStorageMinerCodeCid)
      return VMExitCode::STORAGE_POWER_ACTOR_WRONG_CALLER;

    Address miner_address = runtime.getMessage().get().from;
    StoragePower power = consensusPowerForWeights(params.weights);

    OUTCOME_TRY(power_actor, getCurrentState(runtime));

    OUTCOME_TRY(power_actor.addToClaim(miner_address, -power, -params.pledge));

    if (params.termination_type
        != SectorTerminationType::SECTOR_TERMINATION_EXPIRED) {
      TokenAmount amount_to_slash = pledgePenaltyForSectorTermination(
          params.pledge, params.termination_type);
      OUTCOME_TRY(slashPledgeCollateral(
          runtime, power_actor, miner_address, amount_to_slash));
    }

    OUTCOME_TRY(power_actor_state, power_actor.flushState());
    OUTCOME_TRY(runtime.commitState(power_actor_state));
    return fc::outcome::success();
  }

  ACTOR_METHOD_IMPL(OnSectorTemporaryFaultEffectiveBegin) {
    OUTCOME_TRY(immediate_caller_code_id,
                runtime.getActorCodeID(runtime.getImmediateCaller()));
    if (immediate_caller_code_id != kStorageMinerCodeCid)
      return VMExitCode::STORAGE_POWER_ACTOR_WRONG_CALLER;

    StoragePower power = consensusPowerForWeights(params.weights);

    OUTCOME_TRY(power_actor, getCurrentState(runtime));

    OUTCOME_TRY(power_actor.addToClaim(
        runtime.getMessage().get().from, -power, -params.pledge));

    OUTCOME_TRY(power_actor_state, power_actor.flushState());
    OUTCOME_TRY(runtime.commitState(power_actor_state));
    return fc::outcome::success();
  }
}  // namespace fc::vm::actor
