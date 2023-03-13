add_library(pipe STATIC
    pipe.c
    pipe.h
)

target_link_libraries(pipe PUBLIC
    common_options
    sh
    ast
    execution
    parser
    tools
)

target_include_directories(pipe PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

