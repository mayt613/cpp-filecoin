/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_VM_ACTOR_BUILTIN_STORAGE_POWER_ACTOR_HPP
#define CPP_FILECOIN_VM_ACTOR_BUILTIN_STORAGE_POWER_ACTOR_HPP

#include "codec/cbor/streams_annotation.hpp"
#include "primitives/address/address_codec.hpp"
#include "vm/actor/actor.hpp"
#include "vm/actor/actor_method.hpp"
#include "vm/actor/builtin/miner/types.hpp"
#include "vm/actor/builtin/storage_power/policy.hpp"
#include "vm/actor/builtin/storage_power/storage_power_actor_state.hpp"
#include "vm/runtime/runtime.hpp"
#include "vm/runtime/runtime_types.hpp"

namespace fc::vm::actor::builtin::storage_power {

  using miner::PeerId;
  using runtime::InvocationOutput;
  using runtime::Runtime;

  ACTOR_METHOD_DECL(1, Construct){};

  ACTOR_METHOD_DECL(2, AddBalance) {
    struct Params {
      Address miner;
    };
  };

  ACTOR_METHOD_DECL(3, WithdrawBalance) {
    struct Params {
      Address miner;
      TokenAmount requested;
    };
  };

  ACTOR_METHOD_DECL(4, CreateMiner) {
    struct Params {
      Address worker;  // must be an ID-address
      uint64_t sector_size;
      PeerId peer_id;
    };
    struct Result {
      Address id_address;      // The canonical ID-based address for the actor
      Address robust_address;  // A mre expensive but re-org-safe address for
                               // the newly created actor
    };
  };

  ACTOR_METHOD_DECL(5, DeleteMiner) {
    struct Params {
      Address miner;
    };
  };

  ACTOR_METHOD_DECL(6, OnSectorProveCommit) {
    struct Params {
      SectorStorageWeightDesc weight;
    };
    struct Result {
      TokenAmount pledge;
    };
  };

  ACTOR_METHOD_DECL(7, OnSectorTerminate) {
    struct Params {
      SectorTerminationType termination_type;
      std::vector<SectorStorageWeightDesc> weights;
      TokenAmount pledge;
    };
  };

  ACTOR_METHOD_DECL(8, OnSectorTemporaryFaultEffectiveBegin) {
    struct Params {
      std::vector<SectorStorageWeightDesc> weights;
      TokenAmount pledge;
    };
  };

  ACTOR_METHOD_DECL(9, OnSectorTemporaryFaultEffectiveEnd) {
    struct Params {
      std::vector<SectorStorageWeightDesc> weights;
      TokenAmount pledge;
    };
  };

  ACTOR_METHOD_DECL(10, OnSectorModifyWeightDesc) {
    struct Params {
      SectorStorageWeightDesc prev_weight;
      TokenAmount prev_pledge;
      SectorStorageWeightDesc new_weight;
    };
  };

  ACTOR_METHOD_DECL(11, OnMinerSurprisePoStSuccess){};

  ACTOR_METHOD_DECL(12, OnMinerSurprisePoStFailure) {
    struct Params {
      uint64_t num_consecutive_failures;
    };
  };

  ACTOR_METHOD_DECL(13, EnrollCronEvent) {
    struct Params {
      ChainEpoch event_epoch;
      Buffer payload;
    };
  };

  ACTOR_METHOD_DECL(14, ReportConsensusFault){};

  ACTOR_METHOD_DECL(15, OnEpochTickEnd){};

  inline bool operator==(const CreateMiner::Result &lhs,
                         const CreateMiner::Result &rhs) {
    return lhs.id_address == rhs.id_address
           && lhs.robust_address == rhs.robust_address;
  }

  inline bool operator==(const OnSectorProveCommit::Result &lhs,
                         const OnSectorProveCommit::Result &rhs) {
    return lhs.pledge == rhs.pledge;
  }

  /**
   * Get current storage power actor state
   * @param runtime - current runtime
   * @return current storage power actor state or appropriate error
   */
  outcome::result<StoragePowerActor> getCurrentState(Runtime &runtime);

  outcome::result<InvocationOutput> slashPledgeCollateral(
      Runtime &runtime,
      StoragePowerActor &power_actor,
      Address miner,
      TokenAmount to_slash);

  /**
   * Deletes miner from state and slashes miner balance
   * @param runtime - current runtime
   * @param state - current storage power actor state
   * @param miner address to delete
   * @return error in case of failure
   */
  outcome::result<void> deleteMinerActor(Runtime &runtime,
                                                StoragePowerActor &state,
                                                const Address &miner);

  /** Exported StoragePowerActor methods to invoker */
  extern const ActorExports exports;

  CBOR_TUPLE(AddBalance::Params, miner)

  CBOR_TUPLE(WithdrawBalance::Params, miner, requested)

  CBOR_TUPLE(CreateMiner::Params, worker, sector_size, peer_id)
  CBOR_TUPLE(CreateMiner::Result, id_address, robust_address)

  CBOR_TUPLE(DeleteMiner::Params, miner)

  CBOR_TUPLE(OnSectorProveCommit::Params, weight)
  CBOR_TUPLE(OnSectorProveCommit::Result, pledge)

  CBOR_TUPLE(OnSectorTerminate::Params, termination_type, weights, pledge)

  CBOR_TUPLE(OnSectorTemporaryFaultEffectiveBegin::Params, weights, pledge)

  CBOR_TUPLE(OnSectorModifyWeightDesc::Params,
             prev_weight,
             prev_pledge,
             new_weight)

  CBOR_TUPLE(OnMinerSurprisePoStFailure::Params, num_consecutive_failures)

  CBOR_TUPLE(EnrollCronEvent::Params, event_epoch, payload);

}  // namespace fc::vm::actor::builtin::storage_power

#endif  // CPP_FILECOIN_VM_ACTOR_BUILTIN_STORAGE_POWER_ACTOR_HPP
