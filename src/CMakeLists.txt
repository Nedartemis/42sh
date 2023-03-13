add_executable(42sh main.c)
set_target_properties(42sh PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
) 

target_compile_options(common_options INTERFACE
)

add_subdirectory("sh")
add_subdirectory("execution")
add_subdirectory("parser")
add_subdirectory("ast")
add_subdirectory("lexer")
add_subdirectory("tools")

target_link_libraries(42sh PUBLIC
    common_options
    sh
    execution
    parser
    parse_rule
    ast
    lexer
    tools
    pipe
)
