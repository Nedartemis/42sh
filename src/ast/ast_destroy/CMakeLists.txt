add_library(ast_destroy STATIC
    ast_destroy.c
    ast_destroy.h
    ast_destroy2.c
    ast_destroy2.h
)

target_link_libraries(ast_destroy PUBLIC
    common_options
    sh
    ast
    tools
)

target_include_directories(ast_destroy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

