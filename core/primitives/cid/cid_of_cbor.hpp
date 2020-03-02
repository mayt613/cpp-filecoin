/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_PRIMITIVES_CID_CID_OF_CBOR_HPP
#define CPP_FILECOIN_CORE_PRIMITIVES_CID_CID_OF_CBOR_HPP

#include <libp2p/multi/multicodec_type.hpp>

#include "codec/cbor/cbor.hpp"
#include "common/buffer.hpp"
#include "crypto/hasher/hasher.hpp"
#include "primitives/cid/cid.hpp"

namespace fc::primitives::cid {
  using common::Buffer;

  /**
   * @brief helper function to calculate cid of an object
   * @tparam T object type
   * @param value object instance reference
   * @return calculated cid or error
   */
  template <class T>
  outcome::result<CID> getCidOfCbor(const T &value) {
    OUTCOME_TRY(bytes, fc::codec::cbor::encode(value));
    return common::getCidOf(bytes);
  }

  struct CidParams {
    using Version = CID::Version;
    using Content = libp2p::multi::MulticodecType::Code;
    using Hash = libp2p::multi::HashType;

    // TODO: outcome
    template <typename T>
    static Buffer encode(const T &value) {
      return Buffer{codec::cbor::encode(value).value()};
    }

    template <typename T>
    static CidParams get() {
      return {Version::V1, Content::DAG_CBOR, Hash::blake2b_256};
    }

    // TODO: outcome
    CID compute(gsl::span<const uint8_t> bytes) {
      return {version, content, crypto::Hasher::calculate(hash, bytes)};
    }

    // TODO: outcome
    template <typename T>
    static CID cidOf(const T &value) {
      auto bytes = encode(value);
      return get<T>().compute(gsl::make_span(bytes));
    }

    Version version;
    Content content;
    Hash hash;
  };
}  // namespace fc::primitives::cid

#endif  // CPP_FILECOIN_CORE_PRIMITIVES_CID_CID_OF_CBOR_HPP
