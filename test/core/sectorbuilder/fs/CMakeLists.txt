#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

addtest(fs_test
       types_test.cpp
       )

target_link_libraries(fs_test
        fs
        base_fs_test
        )
