/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_VM_ACTOR_BUILTIN_PAYMENT_CHANNEL_ACTOR_HPP
#define CPP_FILECOIN_VM_ACTOR_BUILTIN_PAYMENT_CHANNEL_ACTOR_HPP

#include "codec/cbor/streams_annotation.hpp"
#include "common/outcome.hpp"
#include "primitives/address/address_codec.hpp"
#include "vm/actor/actor_method.hpp"
#include "vm/actor/builtin/payment_channel/payment_channel_actor_state.hpp"
#include "vm/runtime/runtime.hpp"
#include "vm/runtime/runtime_types.hpp"

namespace fc::vm::actor::builtin::payment_channel {

  using fc::vm::runtime::InvocationOutput;
  using fc::vm::runtime::Runtime;

  ACTOR_METHOD_DECL(1, Construct) {
    struct Params {
      /** Voucher receiver */
      Address to;
    };
  };

  ACTOR_METHOD_DECL(2, UpdateChannelState) {
    struct Params {
      SignedVoucher signed_voucher;
      Buffer secret;
      Buffer proof;
    };
  };

  ACTOR_METHOD_DECL(3, Settle){};

  ACTOR_METHOD_DECL(4, Collect){};

  CBOR_TUPLE(Construct::Params, to)

  CBOR_TUPLE(UpdateChannelState::Params, signed_voucher, secret, proof)

}  // namespace fc::vm::actor::builtin::payment_channel

#endif  // CPP_FILECOIN_VM_ACTOR_BUILTIN_PAYMENT_CHANNEL_ACTOR_HPP
