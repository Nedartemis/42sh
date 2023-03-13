add_library(get_input STATIC
    get_input.h
    get_input.c
)

target_link_libraries(get_input PUBLIC
    sh
)

target_include_directories(get_input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

