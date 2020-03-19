/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sectorbuilder/fs/impl/fs_impl.hpp"

namespace fc::sectorbuilder {

  outcome::result<SectorPath> FSImpl::findSector(DataType type,
                                                 const Address &miner,
                                                 SectorNumber num) {
    return outcome::success();
  }

  outcome::result<StoragePath> FSImpl::findBestPath(uint64_t qty_bytes_needed,
                                                    bool is_cached,
                                                    bool strict) {
    return outcome::success();
  }

  outcome::result<SectorPath> FSImpl::forceAllocSector(DataType type,
                                                       const Address &miner,
                                                       SectorSize ssize,
                                                       bool is_cached,
                                                       SectorNumber num) {
    return outcome::success();
  }

  outcome::result<SectorPath> FSImpl::allocSector(DataType type,
                                                  const Address &miner,
                                                  SectorSize ssize,
                                                  bool is_cached,
                                                  SectorNumber num) {
    return outcome::success();
  }

  outcome::result<SectorPath> FSImpl::prepareCacheMove(const SectorPath &sector,
                                                       SectorSize ssize,
                                                       bool to_cache) {
    return outcome::success();
  }

  outcome::result<void> FSImpl::moveSector(const SectorPath &from,
                                           const SectorPath &to) {
    return outcome::success();
  }

  std::shared_ptr<FS> OpenFs(gsl::span<const PathConfig> cfg) {
    return std::shared_ptr<FSImpl>();
  }

}  // namespace fc::sectorbuilder
