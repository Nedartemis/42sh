add_library(ast_pretty_printer STATIC
    ast_pretty_printer.c
    ast_pretty_printer.h
    ast_pretty_printer2.c
    ast_pretty_printer2.h
)

target_link_libraries(ast_pretty_printer PUBLIC
    common_options
    ast
    tools
)

target_include_directories(ast_pretty_printer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

