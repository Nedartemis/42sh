add_library(lexer_read_input STATIC
    lexer_read_input.c
    lexer_read_input.h
)

target_link_libraries(lexer_read_input PUBLIC
    common_options
    lexer
    tools
    sh
)

target_include_directories(lexer_read_input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
