/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_SECTORBUILDER_FS_HPP
#define CPP_FILECOIN_CORE_SECTORBUILDER_FS_HPP

#include <gsl/span>
#include "sectorbuilder/fs/types.hpp"

using fc::primitives::SectorSize;

namespace fc::sectorbuilder {

    class FS {

        virtual ~FS() = default;

        virtual outcome::result<SectorPath> findSector(DataType type, const Address &miner, SectorNumber num) = 0;

        virtual outcome::result<StoragePath> findBestPath(uint64_t qty_bytes_needed, bool is_cached, bool strict) = 0;

        virtual outcome::result<SectorPath> forceAllocSector(DataType type, const Address & miner, SectorSize ssize, bool is_cached, SectorNumber num) = 0;

        virtual outcome::result<SectorPath> allocSector(DataType type, const Address & miner, SectorSize ssize, bool is_cached, SectorNumber num) = 0;

        virtual outcome::result<SectorPath> prepareCacheMove(const SectorPath &sector, SectorSize ssize, bool to_cache) = 0;

        virtual outcome::result<void> moveSector(const SectorPath &from, const SectorPath &to) = 0;

    };

    std::shared_ptr<FS> OpenFs(gsl::span<const PathConfig> cfg);

}

#endif //CPP_FILECOIN_CORE_SECTORBUILDER_FS_HPP
