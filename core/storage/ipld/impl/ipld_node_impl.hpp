/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef FILECOIN_STORAGE_IPLD_NODE_IMPL_HPP
#define FILECOIN_STORAGE_IPLD_NODE_IMPL_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <boost/optional.hpp>

#include "common/buffer.hpp"
#include "common/outcome.hpp"
#include "primitives/cid/cid_of_cbor.hpp"
#include "storage/ipld/impl/ipld_link_impl.hpp"
#include "storage/ipld/impl/ipld_node_encoder_pb.hpp"
#include "storage/ipld/ipld_block.hpp"
#include "storage/ipld/ipld_link.hpp"

namespace fc::storage::ipld {
  class IPLDNode {
   public:
    size_t size() const;

    void assign(common::Buffer input);

    const common::Buffer &content() const;

    inline CID getCID() const {
      return CidParams::cidOf(*this);
    }

    inline Buffer getRawBytes() const {
      return serialize();
    }

    outcome::result<void> addChild(const std::string &name,
                                   std::shared_ptr<const IPLDNode> node);

    outcome::result<std::reference_wrapper<const IPLDLink>> getLink(
        const std::string &name) const;

    void removeLink(const std::string &name);

    void addLink(const IPLDLink &link);

    std::vector<std::reference_wrapper<const IPLDLink>> getLinks() const;

    static std::shared_ptr<IPLDNode> createFromString(
        const std::string &content);

    static outcome::result<std::shared_ptr<IPLDNode>> createFromRawBytes(
        gsl::span<const uint8_t> input);

    common::Buffer serialize() const;

   private:
    common::Buffer content_;
    std::map<std::string, IPLDLinkImpl> links_;
    size_t child_nodes_size_{};
  };

  using IPLDNodeImpl = IPLDNode;

  enum class IPLDNodeError { LINK_NOT_FOUND = 1, INVALID_RAW_DATA };
}  // namespace fc::storage::ipld

OUTCOME_HPP_DECLARE_ERROR(fc::storage::ipld, IPLDNodeError)

namespace fc::primitives::cid {
  template <>
  inline Buffer CidParams::encode<storage::ipld::IPLDNodeImpl>(
      const storage::ipld::IPLDNodeImpl &node) {
    return node.serialize();
  }

  template <>
  inline CidParams CidParams::get<storage::ipld::IPLDNodeImpl>() {
    return {Version::V0, Content::DAG_PB, Hash::sha256};
  }
}  // namespace fc::primitives::cid

#endif
