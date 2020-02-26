/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_VM_ACTOR_ACTOR_METHOD_HPP
#define CPP_FILECOIN_CORE_VM_ACTOR_ACTOR_METHOD_HPP

#include "codec/cbor/cbor.hpp"
#include "common/outcome.hpp"
#include "vm/actor/actor.hpp"
#include "vm/exit_code/exit_code.hpp"
#include "vm/runtime/runtime.hpp"

#define ACTOR_METHOD_DECL(number, name) struct name : Method<number, name>

#define ACTOR_METHOD_IMPL(name)                                \
  template <>                                                  \
  outcome::result<MethodM<name>::Result> MethodM<name>::apply( \
      const Actor &actor,                                      \
      Runtime &runtime,                                        \
      const MethodM<name>::Params &params)

namespace fc::vm::actor {

  using common::Buffer;
  using primitives::TokenAmount;
  using runtime::InvocationOutput;
  using runtime::Runtime;

  /**
   * Actor method signature
   * @param Actor - actor itself
   * @param Runtime - VM context exposed to actors during method execution
   * @param MethodParams - parameters for method call
   * @return InvocationOutput - invocation method result or error occurred
   */
  using ActorMethod = std::function<outcome::result<InvocationOutput>(
      const Actor &, Runtime &, const MethodParams &)>;

  /// Actor methods exported by number
  using ActorExports = std::map<MethodNumber, ActorMethod>;

  /// Decode actor params, raises appropriate error
  template <typename T>
  outcome::result<T> decodeActorParams(MethodParams params_bytes) {
    auto maybe_params = codec::cbor::decode<T>(params_bytes);
    if (!maybe_params) {
      return VMExitCode::DECODE_ACTOR_PARAMS_ERROR;
    }
    return maybe_params;
  }

  using runtime::encodeActorParams;

  template <typename T>
  outcome::result<T> decodeActorReturn(const InvocationOutput &result) {
    OUTCOME_TRY(decoded,
                codec::cbor::decode<T>(result.return_value.toVector()));
    return std::move(decoded);
  }

  template <typename T>
  outcome::result<InvocationOutput> encodeActorReturn(const T &result) {
    OUTCOME_TRY(encoded, codec::cbor::encode(result));
    return InvocationOutput{Buffer{encoded}};
  }

  struct None {};
  CBOR_ENCODE(None, none) {
    return s;
  }
  CBOR_DECODE(None, none) {
    return s;
  }

  template <typename T>
  struct MethodM {
    using Result = typename T::Result;
    using Params = typename T::Params;

    static outcome::result<Result> apply(const Actor &actor,
                                         Runtime &runtime,
                                         const Params &params);

    static outcome::result<InvocationOutput> applyRaw(
        const Actor &actor, Runtime &runtime, const MethodParams &params) {
      // TODO(turuslan): adt.EmptyValue from specs-actors
      OUTCOME_TRY(params2, decodeActorParams<Params>(params));
      OUTCOME_TRY(result, apply(actor, runtime, params2));
      return encodeActorReturn(result);
    }

    static auto send(Runtime &runtime,
                     const Address &address,
                     const Params &params,
                     TokenAmount value) {
      return runtime.sendPR<Result>(address, T::Number, params, value);
    }
  };

  template <uint64_t number, typename T>
  struct Method {
    using Params = None;
    using Result = None;
    static constexpr MethodNumber Number{number};
    using M = MethodM<T>;

    static auto pair() {
      return std::make_pair(Number, ActorMethod(M::applyRaw));
    }
  };
}  // namespace fc::vm::actor

#endif  // CPP_FILECOIN_CORE_VM_ACTOR_ACTOR_METHOD_HPP
