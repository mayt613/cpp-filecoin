/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sectorbuilder/fs/types.hpp"
#include <boost/filesystem/path.hpp>
#include <primitives/address/address_codec.hpp>

namespace fc::sectorbuilder {

  DataType fromString(const std::string &type) {
    if (type == "cache") {
      return DataType::DATA_CACHE;
    }
    if (type == "staging") {
      return DataType::DATA_STAGING;
    }
    if (type == "sealed") {
      return DataType::DATA_SEALED;
    }
    if (type == "unsealed") {
      return DataType::DATA_UNSEALED;
    }

    return static_cast<DataType>(0);
  }

  std::string toString(DataType type) {
    switch (type) {
      case DataType::DATA_CACHE:
        return "cache";
      case DataType::DATA_STAGING:
        return "staging";
      case DataType::DATA_SEALED:
        return "sealed";
      case DataType::DATA_UNSEALED:
        return "unsealed";
    }

    return "unkown";
  }

  uint64_t overheadMul(DataType type) {
    switch (type) {
      case DataType::DATA_CACHE:
        return 11;
      case DataType::DATA_STAGING:
      case DataType::DATA_SEALED:
      case DataType::DATA_UNSEALED:
        return 1;
    }

    return 1000;
  }

  std::string sectorName(const Address &miner, SectorNumber sector_num) {
    return "s-" + fc::primitives::address::encodeToString(miner) + "-"
           + std::to_string(sector_num);
  }

  StoragePath::StoragePath(const std::string &str)
      : std::string(boost::filesystem::path(str).string()) {}

  SectorPath StoragePath::sector(const DataType &type,
                                 const Address &miner,
                                 SectorNumber num) const {
    return SectorPath(boost::filesystem::path(*this)
                          .append(toString(type))
                          .append(sectorName(miner, num))
                          .string());
  }

  SectorPath::SectorPath(const std::string &str)
      : std::string(boost::filesystem::path(str).string()) {}

  StoragePath SectorPath::storage() const {
    boost::filesystem::path path(*this);
    auto current_dir = path.remove_filename();
    return StoragePath(current_dir.parent_path().string());
  }

  DataType SectorPath::type() const {
    boost::filesystem::path path(*this);
    auto current_dir = path.remove_filename();
    return fromString(current_dir.leaf().string());
  }

  outcome::result<SectorNumber> SectorPath::num() const {
    boost::filesystem::path path(*this);
    auto file_name = path.leaf().string();
    auto index = file_name.find_last_of('-');
    if (index == std::string::npos) {
      return SectorPathErrors::INVALID_SECTOR_FILENAME;
    }

    uint64_t num;
    try {
      num = boost::lexical_cast<uint64_t>(file_name.substr(index + 1));
    } catch (boost::bad_lexical_cast const &e) {
      return SectorPathErrors::INVALID_SECTOR_NUM;
    }

    return SectorNumber(num);
  }

  outcome::result<Address> SectorPath::miner() const {
    boost::filesystem::path path(*this);
    auto file_name = path.leaf().string();
    auto start_index = file_name.find_first_of('-');
    auto finish_index = file_name.find_last_of('-');
    if (start_index == std::string::npos || start_index == finish_index) {
      return SectorPathErrors::INVALID_SECTOR_FILENAME;
    }

    return fc::primitives::address::decodeFromString(
        file_name.substr(start_index + 1, finish_index - start_index - 1));
  }

}  // namespace fc::sectorbuilder

OUTCOME_CPP_DEFINE_CATEGORY(fc::sectorbuilder, SectorPathErrors, e) {
  using fc::sectorbuilder::SectorPathErrors;
  switch (e) {
    case (SectorPathErrors::INVALID_SECTOR_FILENAME):
      return "Sector path: malformed sector file name";
    case (SectorPathErrors::INVALID_SECTOR_NUM):
      return "Sector path: invalid sector num";
    default:
      return "Sector path: unknown error";
  }
}
