/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_CHAIN_VALIDATOR_CONSENSUS_RULES_HPP
#define CPP_FILECOIN_CORE_CHAIN_VALIDATOR_CONSENSUS_RULES_HPP

#include <memory>

#include "blockchain/weight_calculator.hpp"
#include "common/outcome.hpp"
#include "power/power_table.hpp"
#include "primitives/block/block.hpp"
#include "primitives/chain_epoch/chain_epoch.hpp"
#include "storage/ipfs/datastore.hpp"

namespace fc::blockchain::block_validator {
  class ConsensusRules {
   protected:
    using WeightCalculator = weight::WeightCalculator;
    using BlockHeader = primitives::block::BlockHeader;
    using PowerTable = power::PowerTable;
    using IpfsDatastore = storage::ipfs::IpfsDatastore;
    using ChainEpoch = primitives::ChainEpoch;
    using Tipset = primitives::tipset::Tipset;

   public:
    static outcome::result<void> activeMiner(
        const BlockHeader &block,
        const std::shared_ptr<PowerTable> &power_table);

    static outcome::result<void> parentWeight(
        const BlockHeader &block,
        const Tipset &parent_tipset,
        const std::shared_ptr<WeightCalculator> &weight_calculator);

    static outcome::result<void> epoch(const BlockHeader &block,
                                       ChainEpoch current_epoch);
  };

  enum class ConsensusError {
    INVALID_MINER = 1,
    GET_PARENT_TIPSET_ERROR,
    INVALID_PARENT_WEIGHT,
    BLOCK_EPOCH_IN_FUTURE,
    BLOCK_EPOCH_TOO_FAR
  };
}  // namespace fc::blockchain::block_validator

OUTCOME_HPP_DECLARE_ERROR(fc::blockchain::block_validator, ConsensusError);

#endif
