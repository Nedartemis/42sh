add_library(lexer_var STATIC
    lexer_var.c
    lexer_var.h
)

target_link_libraries(lexer_var PUBLIC
    common_options
    lexer
    tools
)

target_include_directories(lexer_var PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
