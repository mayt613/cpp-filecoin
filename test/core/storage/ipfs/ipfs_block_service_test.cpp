/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "storage/ipfs/impl/ipfs_block_service.hpp"

#include <memory>

#include <gtest/gtest.h>
#include "common/outcome.hpp"
#include "storage/ipfs/impl/in_memory_datastore.hpp"
#include "storage/ipld/ipld_block.hpp"
#include "testutil/cbor.hpp"

using fc::CID;
using fc::common::Buffer;
using fc::common::getCidOf;
using fc::storage::ipfs::InMemoryDatastore;
using fc::storage::ipfs::IpfsBlockService;
using fc::storage::ipld::IpldBlock;

/**
 * @class Test fixture for BlockService
 */
class BlockServiceTest : public ::testing::Test {
 protected:
  IpfsBlockService block_service_{std::make_shared<InMemoryDatastore>()};
  CID cid = "010001020002"_cid;
  Buffer value{"DEAD"_unhex};
};

/**
 * @given Sample block with pre-defined data
 * @when Adding, checking existence and retrieving block back from BlockStorage
 * @then Add block @and check its existence @and retrieve block back from
 * BlockStorage
 */
TEST_F(BlockServiceTest, StoreBlockSuccess){
    EXPECT_OUTCOME_TRUE_1(block_service_.set(cid, value))
        EXPECT_OUTCOME_EQ(block_service_.contains(cid), true)
            EXPECT_OUTCOME_EQ(block_service_.get(cid), value)}

/**
 * @given CID of the block, which doesn't exist in the BlockService
 * @when Trying to check block existence in the BlockService
 * @then Operation must be completed successfully with result "not exists"
 */
TEST_F(BlockServiceTest, CheckExistenceSuccess){
    EXPECT_OUTCOME_EQ(block_service_.contains(cid), false)}

/**
 * @given Sample block with pre-defined data
 * @when Trying to remove an existence block
 * @then Operation must be completed successfully
 */
TEST_F(BlockServiceTest, RemoveBlockSuccess){
    EXPECT_OUTCOME_TRUE_1(block_service_.set(cid, value))
        EXPECT_OUTCOME_EQ(block_service_.contains(cid), true)
            EXPECT_OUTCOME_TRUE_1(block_service_.remove(cid))
                EXPECT_OUTCOME_EQ(block_service_.contains(cid), false)}

/**
 * @given CID of the block, which doesn't exist in the BlockService
 * @when Try to get nonexistent block from BlockService
 * @then Attempt fails
 */
TEST_F(BlockServiceTest, GetInvalidCidFailure) {
  EXPECT_OUTCOME_FALSE_1(block_service_.get(cid))
}
