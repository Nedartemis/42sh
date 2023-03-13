add_library(parse_rule STATIC
    parse_rule.c
    parse_rule.h
    parse_rule2.c
    parse_rule2.h
    )

target_link_libraries(parse_rule PUBLIC
    common_options
    sh
    ast
    lexer
    tools
)

target_include_directories(parse_rule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

