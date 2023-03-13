add_library(dico STATIC
    hash_map.c
    hash_map.h
    dico_word.c
    dico_word.h
    dico_func.c
    dico_func.h
    hash.c
)

target_link_libraries(dico PUBLIC
    common_options
    ast
    ast_pretty_printer
    ast_destroy
)

target_include_directories(dico PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
