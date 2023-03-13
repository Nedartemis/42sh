add_library(builtin STATIC
    output_builtin.c
    my_echo.c
    true_false.c
    builtin.h
    my_exit.c
    continue_break.c
    my_cd.c
    my_unset.c
    my_export.c
    dot.c
    my_alias.c
)

target_link_libraries(builtin PUBLIC
    common_options
    tools
    sh
    vector
)

target_include_directories(builtin PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
