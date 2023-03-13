add_library(expansion STATIC
    expansion.c
    expansion.h
    expansion_helper.c
    expansion_helper.h
    expansion_quote.c
    expansion_quote.h
)

target_link_libraries(expansion PUBLIC
    common_options
    tools
    sh
    dico
    vector
    list
    expansion_substitution
    field_splitting
)

add_subdirectory("expansion_substitution")
add_subdirectory("field_splitting")

target_include_directories(expansion PUBLIC .
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
