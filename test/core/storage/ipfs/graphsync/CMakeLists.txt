addtest(graphsync_acceptance_test
    graphsync_acceptance_test.cpp
    graphsync_acceptance_common.cpp
    )

target_link_libraries(graphsync_acceptance_test
    graphsync
    ipfs_merkledag_service
    p2p::asio_scheduler
    )
