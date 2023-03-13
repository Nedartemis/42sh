add_library(ast_graphviz STATIC
    ast_graphviz.c
    ast_graphviz.h
)

target_link_libraries(ast_graphviz PUBLIC
    common_options
    ast
    tools
)

target_include_directories(ast_graphviz PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

