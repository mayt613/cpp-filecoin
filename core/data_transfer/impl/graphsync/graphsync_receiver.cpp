/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "data_transfer/impl/graphsync/graphsync_receiver.hpp"

namespace fc::data_transfer {

  outcome::result<void> GraphsyncReceiver::receiveRequest(
      const PeerInfo &initiator, const DataTransferRequest &request) {
    auto validated = validateVoucher(initiator, request);
    if (!validated) {
      logger_->warn("Voucher is not valid: " + validated.error().message());
      return sendResponse(initiator, false, request.transfer_id);
    }

    OUTCOME_TRY(selector, IPLDNodeImpl::createFromRawBytes(request.selector));
    OUTCOME_TRY(base_cid, CID::fromString(request.base_cid));

    if (request.is_pull) {
      auto channel = createChannel(request.transfer_id,
                                   base_cid,
                                   selector,
                                   request.voucher,
                                   initiator,
                                   peer_,
                                   initiator);
      if (!channel) {
        logger_->warn("Cannot create channel: " + channel.error().message());
        return sendResponse(initiator, false, request.transfer_id);
      }
    } else {
      OUTCOME_TRY(sendGraphSyncRequest());

      auto channel = createChannel(request.transfer_id,
                                   base_cid,
                                   selector,
                                   request.voucher,
                                   initiator,
                                   initiator,
                                   peer_);
      if (!channel) {
        logger_->warn("Cannot create channel: " + channel.error().message());
        return sendResponse(initiator, false, request.transfer_id);
      }
    }

    return sendResponse(initiator, true, request.transfer_id);
  }

  void GraphsyncReceiver::receiveResponse() {}

  void GraphsyncReceiver::receiveError() {}

  outcome::result<void> GraphsyncReceiver::sendResponse(
      const PeerInfo &peer, bool is_accepted, const TransferId &transfer_id) {
    DataTransferMessage response = createResponse(is_accepted, transfer_id);
    OUTCOME_TRY(sender, network_.newMessageSender(peer));
    OUTCOME_TRY(sender->sendMessage(response));
    return outcome::success();
  }

  outcome::result<void> GraphsyncReceiver::createChannel(
      const TransferId &transfer_id,
      const CID &base_cid,
      std::shared_ptr<IPLDNode> selector,
      const std::vector<uint8_t> &voucher,
      const PeerInfo &initiator,
      const PeerInfo &sender_peer,
      const PeerInfo &receiver_peer) {
    ChannelId channel_id{.initiator = initiator, .id = transfer_id};
    Channel channel{.transfer_id = 0,
                    .base_cid = base_cid,
                    .selector = std::move(selector),
                    .voucher = voucher,
                    .sender = sender_peer,
                    .recipient = receiver_peer};
    ChannelState state{.channel = channel};

    // TODO check thread-safety of channels_
    auto res = channels_.try_emplace(channel_id, state);
    if (!res.second) return GraphsyncReceiverError::STATE_ALREADY_EXISTS;

    return outcome::success();
  }

  outcome::result<void> GraphsyncReceiver::sendGraphSyncRequest() {
    return outcome::success();
  }

}  // namespace fc::data_transfer

OUTCOME_CPP_DEFINE_CATEGORY(fc::data_transfer, GraphsyncReceiverError, e) {
  using fc::data_transfer::GraphsyncReceiverError;

  switch (e) {
    case GraphsyncReceiverError::STATE_ALREADY_EXISTS:
      return "GraphsyncReceiverError: state already exists";
  }

  return "unknown error";
}
