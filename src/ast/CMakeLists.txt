add_library(ast STATIC
    ast.c
    ast.h
    ast2.c
    ast2.h
    ast_struct.h
)

target_link_libraries(ast PUBLIC
    common_options
    sh
    ast_destroy
    ast_graphviz
    ast_pretty_printer
    execution
    parser
    tools
)

add_subdirectory("ast_destroy")
add_subdirectory("ast_graphviz")
add_subdirectory("ast_pretty_printer")

target_include_directories(ast PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

