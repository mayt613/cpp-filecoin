/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_VM_ACTOR_BUILTIN_MINER_MINER_ACTOR_HPP
#define CPP_FILECOIN_CORE_VM_ACTOR_BUILTIN_MINER_MINER_ACTOR_HPP

#include "codec/cbor/streams_annotation.hpp"
#include "primitives/address/address.hpp"
#include "primitives/address/address_codec.hpp"
#include "vm/actor/actor_method.hpp"
#include "vm/actor/builtin/miner/types.hpp"

namespace fc::vm::actor::builtin::miner {

  constexpr MethodNumber kSubmitElectionPoStMethodNumber{20};

  ACTOR_METHOD_DECL(2, Construct) {
    struct Params {
      Address owner;
      Address worker;
      SectorSize sector_size;
      PeerId peer_id;
    };
  };

  ACTOR_METHOD_DECL(2, GetControlAddresses) {
    struct Result {
      Address owner;
      Address worker;
    };
  };

  ACTOR_METHOD_DECL(3, ChangeWorkerAddress) {
    struct Params {
      Address new_worker;
    };
  };

  ACTOR_METHOD_DECL(4, ChangePeerId) {
    struct Params {
      PeerId new_id;
    };
  };

  ACTOR_METHOD_DECL(5, SubmitWindowedPoSt) {
    using Params = OnChainPoStVerifyInfo;
  };

  ACTOR_METHOD_DECL(6, OnDeleteMiner){};

  ACTOR_METHOD_DECL(7, PreCommitSector) {
    using Params = SectorPreCommitInfo;
  };

  ACTOR_METHOD_DECL(8, ProveCommitSector) {
    struct Params {
      SectorNumber sector;
      SealProof proof;
    };
  };

  ACTOR_METHOD_DECL(9, ExtendSectorExpiration) {
    struct Params {
      SectorNumber sector;
      ChainEpoch new_expiration;
    };
  };

  ACTOR_METHOD_DECL(10, TerminateSectors) {
    struct Params {
      boost::optional<RleBitset> sectors;
    };
  };

  ACTOR_METHOD_DECL(11, DeclareTemporaryFaults) {
    struct Params {
      RleBitset sectors;
      EpochDuration duration;
    };
  };

  ACTOR_METHOD_DECL(12, OnDeferredCronEvent) {
    struct Params {
      Buffer callback_payload;
    };
  };

  CBOR_TUPLE(Construct::Params, owner, worker, sector_size, peer_id)

  CBOR_TUPLE(GetControlAddresses::Result, owner, worker)

  CBOR_TUPLE(ChangeWorkerAddress::Params, new_worker)

  CBOR_TUPLE(ChangePeerId::Params, new_id)

  CBOR_TUPLE(ProveCommitSector::Params, sector, proof)

  CBOR_TUPLE(ExtendSectorExpiration::Params, sector, new_expiration)

  CBOR_TUPLE(TerminateSectors::Params, sectors)

  CBOR_TUPLE(DeclareTemporaryFaults::Params, sectors, duration)

  CBOR_TUPLE(OnDeferredCronEvent::Params, callback_payload)

  extern const ActorExports exports;

}  // namespace fc::vm::actor::builtin::miner

#endif  // CPP_FILECOIN_CORE_VM_ACTOR_BUILTIN_MINER_MINER_ACTOR_HPP
