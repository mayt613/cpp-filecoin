/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "vm/actor/builtin/payment_channel/payment_channel_actor.hpp"

#include "vm/actor/actor_method.hpp"
#include "vm/exit_code/exit_code.hpp"

namespace fc::vm::actor {
  using namespace builtin::payment_channel;

  using primitives::address::Protocol;

  ACTOR_METHOD_IMPL(Construct) {
    if (actor.code != kAccountCodeCid)
      return VMExitCode::PAYMENT_CHANNEL_WRONG_CALLER;

    if (params.to.getProtocol() != Protocol::ID)
      return VMExitCode::PAYMENT_CHANNEL_ILLEGAL_ARGUMENT;
    OUTCOME_TRY(target_actor_code_id, runtime.getActorCodeID(params.to));
    if (target_actor_code_id != kAccountCodeCid)
      return VMExitCode::PAYMENT_CHANNEL_ILLEGAL_ARGUMENT;

    PaymentChannelActorState state{
        runtime.getImmediateCaller(), params.to, 0, 0, 0, {}};

    OUTCOME_TRY(runtime.commitState(state));
    return fc::outcome::success();
  }

  ACTOR_METHOD_IMPL(UpdateChannelState) {
    OUTCOME_TRY(state,
                runtime.getIpfsDatastore()->getCbor<PaymentChannelActorState>(
                    actor.head));
    Address signer = runtime.getImmediateCaller();
    if (signer != state.from || signer != state.to) {
      return VMExitCode::PAYMENT_CHANNEL_WRONG_CALLER;
    }

    // TODO (a.chernyshov) not implemented yet FIL-129

    return fc::outcome::success();
  }

  ACTOR_METHOD_IMPL(Settle) {
    // TODO (a.chernyshov) not implemented yet FIL-129

    return fc::outcome::success();
  }

  ACTOR_METHOD_IMPL(Collect) {
    // TODO (a.chernyshov) not implemented yet FIL-129

    return fc::outcome::success();
  }
}  // namespace fc::vm::actor
