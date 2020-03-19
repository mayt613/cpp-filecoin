/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_SECTORBUILDER_TYPES_HPP
#define CPP_FILECOIN_CORE_SECTORBUILDER_TYPES_HPP

#include "common/outcome.hpp"
#include "primitives/address/address.hpp"
#include "primitives/sector/sector.hpp"
#include "storage/filestore/path.hpp"

using fc::primitives::SectorNumber;
using fc::primitives::address::Address;
using fc::storage::filestore::Path;

namespace fc::sectorbuilder {

  struct PathInfo {
    bool is_cached;
    int weight;
  };

  struct PathConfig {
      std::string path;
      bool is_cached;
      int weight;
  };

  enum class DataType {
    DATA_CACHE = 1,
    DATA_STAGING,
    DATA_SEALED,
    DATA_UNSEALED,
  };

  DataType fromString(const std::string &type);
  std::string toString(DataType type);
  uint64_t overheadMul(DataType type);

  class SectorPath;

  std::string sectorName(const Address &miner, SectorNumber sector_num);

  class StoragePath : public std::string {
   public:
    StoragePath(const std::string &path);

    SectorPath sector(const DataType &type,
                      const Address &miner,
                      SectorNumber num) const;
  };

  class SectorPath : public std::string {
   public:
    SectorPath(const std::string &str);

    StoragePath storage() const;

    DataType type() const;

    outcome::result<SectorNumber> num() const;

    outcome::result<Address> miner() const;
  };

  enum class SectorPathErrors {
    INVALID_SECTOR_FILENAME = 1,
    INVALID_SECTOR_NUM,
  };

}  // namespace fc::sectorbuilder

OUTCOME_HPP_DECLARE_ERROR(fc::sectorbuilder, SectorPathErrors);

#endif  // CPP_FILECOIN_CORE_SECTORBUILDER_TYPES_HPP
