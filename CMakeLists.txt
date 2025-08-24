cmake_minimum_required(VERSION 4.0)
project(strafer)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
# raylib
FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG 5.5
        GIT_PROGRESS TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "") # Don't build raylib's examples

# googletest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/6986c2b575f77135401a4e1c65a7a42f20e18fef.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib googletest)
add_subdirectory(src)
add_subdirectory(tests)
