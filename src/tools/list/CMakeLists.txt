add_library(list STATIC
    list.c
    list.h
    list_helper.c
    list_helper.h
)

target_link_libraries(list PUBLIC
    common_options
)

target_include_directories(list PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
