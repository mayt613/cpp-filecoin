/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sectorbuilder/fs/types.hpp"

#include "primitives/address/address_codec.hpp"
#include "testutil/literals.hpp"
#include "testutil/outcome.hpp"
#include "testutil/storage/base_fs_test.hpp"

class FSTypesTest : public test::BaseFS_Test {
 public:
  FSTypesTest() : test::BaseFS_Test("fc_fs_types_test") {}
};

using fc::primitives::address::decodeFromString;
using fc::primitives::address::encode;
using fc::sectorbuilder::DataType;
using fc::sectorbuilder::sectorName;
using fc::sectorbuilder::SectorPath;
using fc::sectorbuilder::SectorPathErrors;

TEST_F(FSTypesTest, SectorPath_SectorFunction) {
  Path sector_dir_path =
      fs::canonical(base_path)
          .append(fc::sectorbuilder::toString(DataType::DATA_CACHE))
          .string();
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path =
      boost::filesystem::unique_path(
          fs::canonical(sector_dir_path).append("s-t%%%%%-98"))
          .string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto path = SectorPath(sector_file_path);
  ASSERT_EQ(path.storage(), fs::canonical(base_path).string());
}

TEST_F(FSTypesTest, SectorPath_TypeFunction) {
  auto sector_dir_path = fs::canonical(base_path).append(
      fc::sectorbuilder::toString(DataType::DATA_SEALED));
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path =
      boost::filesystem::unique_path(
          fs::canonical(sector_dir_path).append("s-t%%%%%-98"))
          .string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto path = SectorPath(sector_file_path);
  ASSERT_EQ(path.type(), DataType::DATA_SEALED);
}

TEST_F(FSTypesTest, SectorPath_NumFunction_Success) {
  auto sector_dir_path =
      boost::filesystem::unique_path(fs::canonical(base_path).append(
          fc::sectorbuilder::toString(DataType::DATA_UNSEALED)));
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path =
      boost::filesystem::unique_path(
          fs::canonical(sector_dir_path).append("sector-t%%%%%-123"))
          .string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto path = SectorPath(sector_file_path);
  EXPECT_OUTCOME_EQ(path.num(), SectorNumber(123));
}

TEST_F(FSTypesTest, SectorPath_NumFunction_InvalidName) {
  auto sector_dir_path = fs::canonical(base_path).append(
      fc::sectorbuilder::toString(DataType::DATA_STAGING));
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path = boost::filesystem::unique_path(
                              fs::canonical(sector_dir_path).append("%%%%%"))
                              .string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto path = SectorPath(sector_file_path);
  EXPECT_OUTCOME_ERROR(SectorPathErrors::INVALID_SECTOR_FILENAME, path.num());
}

TEST_F(FSTypesTest, SectorPath_NumFunction_InvalidSectorNum) {
  auto sector_dir_path = fs::canonical(base_path).append(
      fc::sectorbuilder::toString(DataType::DATA_STAGING));
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path =
      boost::filesystem::unique_path(
          fs::canonical(sector_dir_path).append("s-t%%%%%-12d4"))
          .string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto path = SectorPath(sector_file_path);
  EXPECT_OUTCOME_ERROR(SectorPathErrors::INVALID_SECTOR_NUM, path.num());

  Path sector_file_2_path =
      boost::filesystem::unique_path(
          fs::canonical(sector_dir_path).append("s-%%%%%-124d"))
          .string();
  boost::filesystem::ofstream(sector_file_2_path).close();

  path = SectorPath(sector_file_2_path);
  EXPECT_OUTCOME_ERROR(SectorPathErrors::INVALID_SECTOR_NUM, path.num());
}

TEST_F(FSTypesTest, SectorPath_MinerFunction_Success) {
  auto sector_dir_path = fs::canonical(base_path).append(
      fc::sectorbuilder::toString(DataType::DATA_STAGING));
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path =
      boost::filesystem::unique_path(
          fs::canonical(sector_dir_path)
              .append("s-t17uoq6tp427uzv7fztkbsnn64iwotfrristwpryy-%%%%"))
          .string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto path = SectorPath(sector_file_path);
  EXPECT_OUTCOME_TRUE(miner, path.miner());
  ASSERT_EQ(encode(miner), "01fd1d0f4dfcd7e99afcb99a8326b7dc459d32c628"_unhex);
}

TEST_F(FSTypesTest, SectorPath_MinerFunction_InvalidName) {
  auto sector_dir_path = fs::canonical(base_path).append(
      fc::sectorbuilder::toString(DataType::DATA_STAGING));
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path =
      boost::filesystem::unique_path(
          fs::canonical(sector_dir_path).append("sector-%%%%"))
          .string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto path = SectorPath(sector_file_path);
  EXPECT_OUTCOME_ERROR(SectorPathErrors::INVALID_SECTOR_FILENAME, path.miner());
}

TEST_F(FSTypesTest, sectorName) {
  auto sector_dir_path =
      fs::canonical(base_path).append(toString(DataType::DATA_CACHE));
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path =
      fs::canonical(sector_dir_path).append("s-t0999-84").string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto path = SectorPath(sector_file_path);
  EXPECT_OUTCOME_TRUE(miner, path.miner());
  EXPECT_OUTCOME_TRUE(num, path.num())
  ASSERT_EQ(sectorName(miner, num), "s-t0999-84");
}

TEST_F(FSTypesTest, StoragePath_SectorFunction) {
  auto sector_dir_path =
      fs::canonical(base_path).append(toString(DataType::DATA_CACHE));
  boost::filesystem::create_directory(sector_dir_path);
  Path sector_file_path =
      fs::canonical(sector_dir_path).append("s-t0999-84").string();
  boost::filesystem::ofstream(sector_file_path).close();

  auto result_path = SectorPath(sector_file_path);
  EXPECT_OUTCOME_TRUE(miner, result_path.miner())
  EXPECT_OUTCOME_TRUE(num, result_path.num())
  ASSERT_EQ(result_path.storage().sector(DataType::DATA_CACHE, miner, num),
            result_path);
}
