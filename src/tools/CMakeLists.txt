add_library(tools STATIC
    tools.c
    tools.h
    ope_string.c
    ope_string.h
    null_array.c
    null_array.h
    )

target_link_libraries(tools PUBLIC
    common_options
    list
)

add_subdirectory("dico")
add_subdirectory("list")
add_subdirectory("vector")

target_include_directories(tools PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
