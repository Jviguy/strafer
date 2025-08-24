enable_testing()

add_executable(strafer_test
        strafer_test.cpp
)

target_link_libraries(
        strafer_test
        GTest::gtest_main
        strafer_lib
)
