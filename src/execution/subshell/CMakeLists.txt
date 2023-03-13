add_library(subshell STATIC
    subshell.c
    subshell.h
)

target_link_libraries(subshell PUBLIC
    common_options
    sh
    ast
    execution
    parser
    tools
)

target_include_directories(subshell PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

