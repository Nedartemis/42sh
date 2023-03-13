add_library(expansion_substitution STATIC
    expansion_cmd_subst.c
    expansion_cmd_subst.h
    expansion_substitution.c
    expansion_substitution.h
    special_var.c
    special_var.h
    expansion_var.c
    expansion_var.h
)

target_link_libraries(expansion_substitution PUBLIC
    common_options
    tools
    sh
    dico
    vector
    list
    lexer_cmd_subst
    lexer
    field_splitting
)

target_include_directories(expansion_substitution PUBLIC .
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
