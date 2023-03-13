add_library(redirection STATIC
    redirection.c
    redirection.h
    redirection_helper.c
)

target_link_libraries(redirection PUBLIC
    common_options
    lexer
    ast
)

target_include_directories(redirection PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
