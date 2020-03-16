/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_DATA_TRANSFER_GRAPHSYNC_MANAGER_HPP
#define CPP_FILECOIN_DATA_TRANSFER_GRAPHSYNC_MANAGER_HPP

#include "data_transfer/manager.hpp"

#include <libp2p/host/host.hpp>
#include "data_transfer/impl/libp2p_data_transfer_network.hpp"

namespace fc::data_transfer::graphsync {

  using libp2p::Host;

  class GraphSyncManager : public Manager {
   public:
    explicit GraphSyncManager(std::shared_ptr<Host> host);

    // do not need - validate on construction
    //    /**
    //     * RegisterVoucherType registers a validator for the given voucher
    //     type
    //     * @param validator
    //     * @return error if there is a voucher type registered with an
    //     identical
    //     * identifier
    //     */
    //    outcome::result<void> registerVoucherType(
    //        const std::type_info &type, const RequestValidator &validator)
    //        override;

    // TODO
    //    outcome::result<ChannelId> openPushDataChannel(
    //        // ctx context.Context,
    //        PeerId to,
    //        Voucher voucher,
    //        CID base_cid,
    //        std::shared_ptr<IPLDNode> selector) override;
    //
    //    // TODO
    //    outcome::result<ChannelId> openPullDataChannel(
    //        // ctx context.Context,
    //        PeerId to,
    //        Voucher voucher,
    //        CID base_cid,
    //        std::shared_ptr<IPLDNode> selector) override;
    //
    //    void closeDataTransferChannel(ChannelId id) override;
    //
    //    Status transferChannelStatus(ChannelId id) override;
    //
    //    Unsubscribe subscribeToEvents(Subscriber subscriber) override;
    //
    //    std::map<ChannelId, ChannelState> inProgressChannels() override;

   private:
    Libp2pDataTransferNetwork network_;
    //    /**
    //     * Encapsulates message creation and posting to the data transfer
    //     network
    //     * with the provided parameters
    //     * @param selector
    //     * @param is_pool
    //     * @param voucher
    //     * @param base_cid
    //     * @return
    //     */
    //    outcome::result<void> sendDtRequest(std::shared_ptr<IPLDNode>
    //    selector,
    //                                        bool is_pool,
    //                                        const Voucher &voucher,
    //                                        const CID &base_cid);
    //
    //    std::shared_ptr<Host> host_;
    //    // TODO is it 0?
    //    std::atomic<DataTransferId> last_tx_id{0};
  };

}  // namespace fc::data_transfer::graphsync

#endif  // CPP_FILECOIN_DATA_TRANSFER_GRAPHSYNC_MANAGER_HPP
