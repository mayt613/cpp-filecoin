/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "libp2p_data_transfer_network.hpp"

namespace fc::data_transfer {

  Libp2pDataTransferNetwork::Libp2pDataTransferNetwork(
      std::shared_ptr<libp2p::Host> host)
      : host_(std::move(host)) {}

  void Libp2pDataTransferNetwork::setDelegate() {}

  outcome::result<void> Libp2pDataTransferNetwork::connectTo(
      const PeerInfo &peer) {
    host_->connect(peer);
    return outcome::success();
  }

  outcome::result<std::shared_ptr<MessageSender>>
  Libp2pDataTransferNetwork::newMessageSender(const PeerInfo &peer) {
    std::shared_ptr<libp2p::connection::Stream> stream;
    host_->newStream(
        peer,
        DATA_TRANSFER_PROTOCOL,
        [&stream](
            // TODO handle error
            outcome::result<std::shared_ptr<libp2p::connection::Stream>> s) {
          stream = std::move(s.value());
        });
    return std::make_shared<StreamMessageSender>(stream);
  }

}  // namespace fc::data_transfer
