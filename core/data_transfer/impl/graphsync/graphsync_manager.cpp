/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "data_transfer/impl/graphsync/graphsync_manager.hpp"
#include "data_transfer/message.hpp"

namespace fc::data_transfer::graphsync {

  using common::Buffer;

  GraphSyncManager::GraphSyncManager(std::shared_ptr<Host> host)
      : network_(std::move(host)) {}

  //  outcome::result<void> GraphSyncManager::registerVoucherType(
  //      const std::type_info &type, const RequestValidator &validator) {
  //
  //    // TODO validate voucher type
  //    return outcome::success();
  //  }

  //  outcome::result<void> GraphSyncManager::sendDtRequest(
  //      std::shared_ptr<IPLDNode> selector,
  //      bool is_pool,
  //      const Voucher &voucher,
  //      const CID &base_cid) {
  //    Buffer selector_bytes = selector->getRawBytes();
  //    OUTCOME_TRY(voucher_bytes, voucher.toBytes());
  //    TransferId tx_id = ++last_tx_id;
  //    DataTransferRequest request{true,
  //                                tx_id,
  //                                is_pool,
  //                                voucher.getType(),
  //                                voucher_bytes,
  //                                base_cid,
  //                                selector_bytes,
  //                                false};
  //    // TODO
  //    // create Graphsync message request
  //    // send message
  //    return outcome::success();
  //  }

  // TODO
  //  outcome::result<ChannelId> GraphSyncManager::openPushDataChannel(
  //      // ctx context.Context,
  //      PeerId to,
  //      Voucher voucher,
  //      CID base_cid,
  //      std::shared_ptr<IPLDNode> selector) {
  //    // TODO
  //    // 1. SendDtRequest
  //    // 2. Open Channel
  //    return outcome::success();
  //  }

  // TODO
  //  outcome::result<ChannelId> GraphSyncManager::openPullDataChannel(
  //      // ctx context.Context,
  //      PeerId to,
  //      Voucher voucher,
  //      CID base_cid,
  //      std::shared_ptr<IPLDNode> selector) {
  //    return outcome::success();
  //  }

  //  void GraphSyncManager::closeDataTransferChannel(ChannelId id) {}

  //  Status GraphSyncManager::transferChannelStatus(ChannelId id) {
  //    return Status::FAILED;
  //  }

  //  Unsubscribe GraphSyncManager::subscribeToEvents(Subscriber subscriber) {
  //    return Unsubscribe{};
  //  }

  //  std::map<ChannelId, ChannelState> GraphSyncManager::inProgressChannels() {
  //    return std::map<ChannelId, ChannelState>{};
  //  }
}  // namespace fc::data_transfer::graphsync
