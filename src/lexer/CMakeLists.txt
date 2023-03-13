add_library(lexer STATIC
    lexer.c
    lexer.h
    lexer_test.c
    lexer_run.c
    lexer_run.h
    operator.c
    operator.h
    token.c
    token.h
)

target_link_libraries(lexer PUBLIC
    common_options
    lexer_read_input
    lexer_quoting
    lexer_cmd_subst
    lexer_alias
    sh
)

add_subdirectory("lexer_cmd_subst")
add_subdirectory("lexer_read_input")
add_subdirectory("lexer_quoting")
add_subdirectory("lexer_var")
add_subdirectory("lexer_alias")


target_include_directories(lexer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)



