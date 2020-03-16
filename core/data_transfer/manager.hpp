/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_DATA_TRANSFER_MANAGER_HPP
#define CPP_FILECOIN_DATA_TRANSFER_MANAGER_HPP

#include <map>
#include "data_transfer/types.hpp"

namespace fc::data_transfer {

  /**
   * Manager is the core interface presented by all implementations of of
   * the data transfer subsystem
   */
  class Manager {
   public:
    virtual ~Manager() = default;

    //    /**
    //     * RegisterVoucherType registers a validator for the given voucher
    //     type will
    //     * error if voucher type does not implement voucher or if there is a
    //     voucher
    //     * type registered with an identical identifier
    //     */
    //    virtual outcome::result<void> registerVoucherType(
    //        const std::type_info &type,
    //        const RequestValidator &validator) = 0;

    /**
     * Open a data transfer that will send data to the recipient peer and
     * transfer parts of the piece that match the selector
     */
    // TODO
//    virtual outcome::result<ChannelId> openPushDataChannel(
//        // ctx context.Context,
//        PeerId to,
//        Voucher voucher,
//        CID base_cid,
//        std::shared_ptr<IPLDNode> selector) = 0;
//
//    /**
//     * Open a data transfer that will request data from the sending peer and
//     * transfer parts of the piece that match the selector
//     */
//    // TODO
//    virtual outcome::result<ChannelId> openPullDataChannel(
//        // ctx context.Context,
//        PeerId to,
//        Voucher voucher,
//        CID base_cid,
//        std::shared_ptr<IPLDNode> selector) = 0;
//
//    /** close an open channel (effectively a cancel) */
//    virtual void closeDataTransferChannel(ChannelId id) = 0;
//
//    /** get status of a transfer */
//    virtual Status transferChannelStatus(ChannelId id) = 0;
//
//    /** get notified when certain types of events happen */
//    virtual Unsubscribe subscribeToEvents(Subscriber subscriber) = 0;
//
//    /** get all in progress transfers */
//    virtual std::map<ChannelId, ChannelState> inProgressChannels() = 0;
  };

}  // namespace fc::data_transfer

#endif  // CPP_FILECOIN_DATA_TRANSFER_MANAGER_HPP
