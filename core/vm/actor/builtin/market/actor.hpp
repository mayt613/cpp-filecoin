/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_VM_ACTOR_BUILTIN_MARKET_ACTOR_HPP
#define CPP_FILECOIN_CORE_VM_ACTOR_BUILTIN_MARKET_ACTOR_HPP

#include "codec/cbor/streams_annotation.hpp"
#include "primitives/types.hpp"
#include "vm/actor/actor_method.hpp"

namespace fc::vm::actor::builtin::market {
  using primitives::ChainEpoch;
  using primitives::DealId;
  using primitives::SectorSize;

  ACTOR_METHOD_DECL(6, VerifyDealsOnSectorProveCommit) {
    struct Params {
      std::vector<DealId> deals;
      ChainEpoch sector_expiry;
    };
  };

  ACTOR_METHOD_DECL(7, OnMinerSectorsTerminate) {
    struct Params {
      std::vector<DealId> deals;
    };
  };

  ACTOR_METHOD_DECL(8, ComputeDataCommitment) {
    struct Params {
      std::vector<DealId> deals;
      SectorSize sector_size;
    };
  };

  CBOR_TUPLE(VerifyDealsOnSectorProveCommit::Params, deals, sector_expiry)

  CBOR_TUPLE(OnMinerSectorsTerminate::Params, deals)

  CBOR_TUPLE(ComputeDataCommitment::Params, deals, sector_size)
}  // namespace fc::vm::actor::builtin::market

#endif  // CPP_FILECOIN_CORE_VM_ACTOR_BUILTIN_MARKET_ACTOR_HPP
