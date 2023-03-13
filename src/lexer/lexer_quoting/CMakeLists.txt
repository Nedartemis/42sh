add_library(lexer_quoting STATIC
    lexer_quoting.c
    lexer_quoting.h
)

target_link_libraries(lexer_quoting PUBLIC
    common_options
    lexer
    lexer_var
    tools
)

target_include_directories(lexer_quoting PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
