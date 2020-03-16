/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_DATA_TRANSFER_GRAPHSYNC_RECEIVER_HPP
#define CPP_FILECOIN_DATA_TRANSFER_GRAPHSYNC_RECEIVER_HPP

#include "data_transfer/message_receiver.hpp"

#include "data_transfer/impl/libp2p_data_transfer_network.hpp"
#include "storage/ipfs/graphsync/graphsync.hpp"

namespace fc::data_transfer {

  using storage::ipfs::graphsync::Graphsync;

  class GraphsyncReceiver : public MessageReceiver {
   public:
    outcome::result<void> receiveRequest(
        const PeerInfo &initiator, const DataTransferRequest &request) override;

    void receiveResponse() override;
    void receiveError() override;

   private:
    outcome::result<void> sendResponse(const PeerInfo &peer,
                                       bool is_accepted,
                                       const TransferId &transfer_id);

    /**
     * Creates a new channel id and channel state and saves to channels
     * @return error if the channel exists already
     */
    outcome::result<void> createChannel(const TransferId &transfer_id,
                                        const CID &base_cid,
                                        std::shared_ptr<IPLDNode> selector,
                                        const std::vector<uint8_t> &voucher,
                                        const PeerInfo &initiator,
                                        const PeerInfo &sender_peer,
                                        const PeerInfo &receiver_peer);

    outcome::result<void> sendGraphSyncRequest();

    /**
     * validateVoucher converts a voucher in an incoming message to its
     * appropriate voucher struct, then runs the validator and returns the
     * results.
     * @param sender
     * @param incoming_request
     * @return error if:
     *  - voucherFromRequest fails
     *  - deserialization of selector fails
     *  - validation fails
     */
    outcome::result<void> validateVoucher(
        const PeerInfo &sender, const DataTransferRequest &incoming_request);

    Libp2pDataTransferNetwork network_;
    std::shared_ptr<Graphsync> graphsync_;
    PeerInfo peer_;
    std::map<ChannelId, ChannelState> channels_;
  };

  /**
   * @brief Type of errors returned by Keystore
   */
  enum class GraphsyncReceiverError {
    STATE_ALREADY_EXISTS
  };

}  // namespace fc::data_transfer

OUTCOME_HPP_DECLARE_ERROR(fc::data_transfer, GraphsyncReceiverError);

#endif  // CPP_FILECOIN_DATA_TRANSFER_GRAPHSYNC_RECEIVER_HPP
