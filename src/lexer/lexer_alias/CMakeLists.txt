add_library(lexer_alias STATIC
    lexer_alias.c
    lexer_alias.h
)

target_link_libraries(lexer_alias PUBLIC
    common_options
    lexer
    lexer_var
    tools
    dico
)

target_include_directories(lexer_alias PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
