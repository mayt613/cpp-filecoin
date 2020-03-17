/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_CHAIN_VALIDATOR_SYNTAX_RULES_HPP
#define CPP_FILECOIN_CORE_CHAIN_VALIDATOR_SYNTAX_RULES_HPP

#include "common/outcome.hpp"
#include "primitives/block/block.hpp"

namespace fc::blockchain::block_validator {
  class SyntaxRules {
   protected:
    using BlockHeader = primitives::block::BlockHeader;

   public:
    static outcome::result<void> parentsCount(const BlockHeader &block);

    static outcome::result<void> parentWeight(const BlockHeader &block);

    static outcome::result<void> minerAddress(const BlockHeader &block);

    static outcome::result<void> timestamp(const BlockHeader &block);

    static outcome::result<void> ticket(const BlockHeader &block);

    static outcome::result<void> electionPost(const BlockHeader &block);

    static outcome::result<void> forkSignal(const BlockHeader &block);
  };

  enum class SyntaxError {
    INVALID_PARENTS_COUNT = 1,
    INVALID_PARENT_WEIGHT,
    INVALID_MINER_ADDRESS,
    INVALID_TIMESTAMP,
    INVALID_TICKET,
    INVALID_ELECTION_POST,
    INVALID_FORK_SIGNAL
  };
}  // namespace fc::blockchain::block_validator

OUTCOME_HPP_DECLARE_ERROR(fc::blockchain::block_validator, SyntaxError);

#endif
