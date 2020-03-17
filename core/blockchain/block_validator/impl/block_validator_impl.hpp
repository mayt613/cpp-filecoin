/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_CHAIN_IMPL_BLOCK_VALIDATOR_IMPL_HPP
#define CPP_FILECOIN_CORE_CHAIN_IMPL_BLOCK_VALIDATOR_IMPL_HPP

#include <functional>
#include <map>
#include <memory>

#include <boost/optional.hpp>
#include <libp2p/crypto/secp256k1_provider.hpp>
#include "blockchain/block_validator/block_validator.hpp"
#include "blockchain/weight_calculator.hpp"
#include "clock/chain_epoch_clock.hpp"
#include "clock/utc_clock.hpp"
#include "crypto/bls/bls_provider.hpp"
#include "power/power_table.hpp"
#include "storage/ipfs/datastore.hpp"
#include "vm/indices/indices.hpp"
#include "vm/interpreter/interpreter.hpp"

namespace fc::blockchain::block_validator {

  class BlockValidatorImpl : public BlockValidator {
   protected:
    using Stage = scenarios::Stage;
    using IpfsDatastore = storage::ipfs::IpfsDatastore;
    using EpochClock = clock::ChainEpochClock;
    using UTCClock = clock::UTCClock;
    using WeightCalculator = blockchain::weight::WeightCalculator;
    using PowerTable = power::PowerTable;
    using BlsProvider = crypto::bls::BlsProvider;
    using SecpProvider = libp2p::crypto::secp256k1::Secp256k1Provider;
    using Interpreter = vm::interpreter::Interpreter;
    using Tipset = primitives::tipset::Tipset;
    using Indices = vm::indices::Indices;

   public:
    BlockValidatorImpl(std::shared_ptr<IpfsDatastore> ipfs_store,
                       std::shared_ptr<UTCClock> utc_clock,
                       std::shared_ptr<EpochClock> epoch_clock,
                       std::shared_ptr<WeightCalculator> weight_calculator,
                       std::shared_ptr<PowerTable> power_table,
                       std::shared_ptr<BlsProvider> bls_crypto_provider,
                       std::shared_ptr<SecpProvider> secp_crypto_provider,
                       std::shared_ptr<Interpreter> vm_interpreter,
                       std::shared_ptr<Indices> indices)
        : stage_executors_{{Stage::SYNTAX_BV0, &BlockValidatorImpl::syntax},
                           {Stage::CONSENSUS_BV1,
                            &BlockValidatorImpl::consensus},
                           {Stage::BLOCK_SIGNATURE_BV2,
                            &BlockValidatorImpl::block_sign},
                           {Stage::ELECTION_POST_BV3,
                            &BlockValidatorImpl::election_post},
                           {Stage::MESSAGE_SIGNATURE_BV4,
                            &BlockValidatorImpl::message_sign},
                           {Stage::STATE_TREE_BV5,
                            &BlockValidatorImpl::state_tree}},
          datastore_{std::move(ipfs_store)},
          clock_{std::move(utc_clock)},
          epoch_clock_{std::move(epoch_clock)},
          weight_calculator_{std::move(weight_calculator)},
          power_table_{std::move(power_table)},
          bls_provider_{std::move(bls_crypto_provider)},
          secp_provider_{std::move(secp_crypto_provider)},
          vm_interpreter_{std::move(vm_interpreter)},
          vm_indices_{std::move(indices)} {}

    outcome::result<void> validateBlock(
        const BlockHeader &block, scenarios::Scenario scenario) const override;

   private:
    using StageExecutor = outcome::result<void> (BlockValidatorImpl::*)(
        const BlockHeader &block) const;

    std::map<scenarios::Stage, StageExecutor> stage_executors_;

    std::shared_ptr<IpfsDatastore> datastore_;
    std::shared_ptr<UTCClock> clock_;
    std::shared_ptr<EpochClock> epoch_clock_;
    std::shared_ptr<WeightCalculator> weight_calculator_;
    std::shared_ptr<PowerTable> power_table_;
    std::shared_ptr<BlsProvider> bls_provider_;
    std::shared_ptr<SecpProvider> secp_provider_;
    std::shared_ptr<Interpreter> vm_interpreter_;
    std::shared_ptr<Indices> vm_indices_;

    /**
     * BlockHeader CID -> Parent tipset
     * Assuming that block validation executes sequentially,
     * so for several blocks parent tipset will be the same
     */
    mutable boost::optional<std::pair<CID, Tipset>> parent_tipset_cache_;

    outcome::result<void> syntax(const BlockHeader &block) const;

    outcome::result<void> consensus(const BlockHeader &block) const;

    outcome::result<void> block_sign(const BlockHeader &block) const;

    outcome::result<void> election_post(const BlockHeader &block) const;

    outcome::result<void> chain_ancestry(const BlockHeader &block) const;

    outcome::result<void> message_sign(const BlockHeader &block) const;

    outcome::result<void> state_tree(const BlockHeader &block) const;

    outcome::result<std::reference_wrapper<Tipset>> get_parent_tipset(
        const BlockHeader &block) const;
  };

  enum class ValidatorError {
    UNKNOWN_STAGE = 1,
    UNKNOWN_BLOCK_SIGNATURE,
    INVALID_BLOCK_SIGNATURE,
    INVALID_MINER_PUBLIC_KEY,
    INVALID_PARENT_STATE
  };

}  // namespace fc::blockchain::block_validator

OUTCOME_HPP_DECLARE_ERROR(fc::blockchain::block_validator, ValidatorError);

#endif  // CPP_FILECOIN_CORE_CHAIN_IMPL_BLOCK_VALIDATOR_IMPL_HPP
