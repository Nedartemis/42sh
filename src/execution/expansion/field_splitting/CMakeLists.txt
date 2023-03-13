add_library(field_splitting STATIC
    field_splitting.c
    field_splitting.h
)

target_link_libraries(field_splitting PUBLIC
    common_options
    tools
    sh
    dico
    vector
    list
    lexer_cmd_subst
    lexer
)

target_include_directories(field_splitting PUBLIC .
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
