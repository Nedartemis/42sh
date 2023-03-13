add_library(parser_helper STATIC
    parser_helper.c
    parser_helper.h
    parser_helper2.c
    parser_helper2.h
    )

target_link_libraries(parser_helper PUBLIC
    common_options
    sh
    ast
    lexer
    parse_rule
    tools
)

target_include_directories(parser_helper PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

