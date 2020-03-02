/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef FILECOIN_STORAGE_IPLD_BLOCK_HPP
#define FILECOIN_STORAGE_IPLD_BLOCK_HPP

#include "codec/cbor/cbor.hpp"
#include "common/buffer.hpp"
#include "primitives/cid/cid_of_cbor.hpp"

namespace fc::storage::ipld {
  using common::Buffer;
  using primitives::cid::CidParams;

  /**
   * @class Interface for various data structure, linked with CID
   */
  struct IpldBlock {
    // TODO: outcome
    template <typename T>
    static IpldBlock create(const T &value) {
      auto bytes = CidParams::encode(value);
      return {CidParams::get<T>().compute(bytes), bytes};
    }

    CID cid;
    Buffer bytes;
  };
}  // namespace fc::storage::ipld

#endif
