add_library(functions STATIC
    functions.c
    functions.h
)

target_link_libraries(functions PUBLIC
    common_options
    tools
    dico
)

target_include_directories(functions PUBLIC .
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
