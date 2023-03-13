add_library(execution STATIC
    run_ast2.c
    run_ast2.h
    run_ast.c
    run_ast.h
)

target_link_libraries(execution PUBLIC
    common_options
    sh
    builtin
    expansion
    simple_commands
    functions
    ast
    tools
    pipe
    redirection
    subshell
)

add_subdirectory("builtin")
add_subdirectory("simple_commands")
add_subdirectory("pipe")
add_subdirectory("expansion")
add_subdirectory("redirection")
add_subdirectory("subshell")
add_subdirectory("functions")

target_include_directories(execution PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
