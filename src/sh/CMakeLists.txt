add_library(sh STATIC
    sh.c
    log.c
    sh.h
    struct.h
    sh_instance.c
    sh_instance.h
)

target_link_libraries(sh PUBLIC
    common_options
    get_input
    execution
    parser
    ast
    lexer
    tools
)

add_subdirectory(input)

target_include_directories(sh PUBLIC .
    ${CMAKE_CURRENT_SOURCE_DIR}
    )
