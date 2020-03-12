/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "primitives/tipset/tipset_key.hpp"

#include <libp2p/multi/content_identifier_codec.hpp>
#include "common/outcome.hpp"

namespace fc::primitives::tipset {
  namespace {
    using ByteArray = std::vector<uint8_t>;
    outcome::result<ByteArray> cidVectorToBytes(const std::vector<CID> &cids) {
      ByteArray buffer{};
      for (auto &c : cids) {
        OUTCOME_TRY(v, c.toBytes());
        buffer.insert(buffer.end(), v.begin(), v.end());
      }

      return buffer;
    }
  }  // namespace

  std::string TipsetKey::toPrettyString() const {
    if (cids_.empty()) {
      return "{}";
    }

    std::string result = "{";
    auto size = cids_.size();
    for (size_t i = 0; i < size; ++i) {
      // TODO (yuraz): FIL-133 use CID::toString()
      //  after it is implemented for CID V1
      result += cids_[i].toPrettyString("");
      if (i != size - 1) {
        result += ", ";
      }
    }
    result += "}";

    return result;
  }

  outcome::result<std::vector<uint8_t>> TipsetKey::toBytes() const {
    return cidVectorToBytes(cids_);
  }

  outcome::result<TipsetKey> TipsetKey::create(std::vector<CID> cids) {
    OUTCOME_TRY(bytes, cidVectorToBytes(cids));
    auto hash = boost::hash_range(bytes.begin(), bytes.end());
    return TipsetKey(std::move(cids), hash);
  }

}  // namespace fc::primitives::tipset
