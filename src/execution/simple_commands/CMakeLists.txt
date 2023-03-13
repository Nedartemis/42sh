add_library(simple_commands STATIC
    simple_commands.c
    simple_commands.h
)

target_link_libraries(simple_commands PUBLIC
    common_options
    tools
)

target_include_directories(simple_commands PUBLIC .
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
