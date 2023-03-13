add_library(lexer_cmd_subst STATIC
    lexer_cmd_subst.c
    lexer_cmd_subst.h
)

target_link_libraries(lexer_cmd_subst PUBLIC
    common_options
    lexer
    lexer_var
    tools
)

target_include_directories(lexer_cmd_subst PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
