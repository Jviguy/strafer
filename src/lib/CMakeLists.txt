add_library(strafer_lib
        StrafeManager.cpp
        Strafe.cpp
)

target_include_directories(strafer_lib
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(strafer_lib PUBLIC raylib)
