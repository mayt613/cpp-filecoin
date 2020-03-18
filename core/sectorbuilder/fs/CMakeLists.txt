#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

add_library(fs
        types.cpp
        )

target_link_libraries(fs
        outcome
        Boost::filesystem
        address
        )
