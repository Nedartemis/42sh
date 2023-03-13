add_library(parser STATIC
    parser.c
    parser.h
    )

target_link_libraries(parser PUBLIC
    common_options
    sh
    ast
    lexer
    parser_helper
    parse_rule
    tools
)

add_subdirectory("parse_rule")
add_subdirectory("parser_helper")

target_include_directories(parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

