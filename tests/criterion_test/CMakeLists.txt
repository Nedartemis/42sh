# list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
# find_package(Criterion REQUIRED)


add_executable(testsuite_criterion
    echo_test.c
)

target_compile_options(testsuite_criterion PUBLIC
    -lcriterion
)

target_link_options(testsuite_criterion PUBLIC
    -lcriterion
)

target_link_libraries(testsuite_criterion PRIVATE
    builtin
)
