#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

add_custom_target(adt
    DEPENDS
    array
    multimap
    )

add_library(array
    impl/array.cpp
    )
target_link_libraries(array
    amt
    )

add_library(balance_table_hamt
    impl/balance_table_hamt.cpp
    )
target_link_libraries(balance_table_hamt
    address
    hamt
    )

add_library(multimap
    impl/multimap.cpp
    )
target_link_libraries(multimap
    array
    hamt
    )
