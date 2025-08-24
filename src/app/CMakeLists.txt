add_executable(strafer
        main.cpp
        App.cpp
        App.h
)

target_link_libraries(strafer PRIVATE strafer_lib raylib)
