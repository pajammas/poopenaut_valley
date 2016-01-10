file(GLOB_RECURSE common_sources *.cpp *.cc)
file(GLOB_RECURSE common_headers *.h *.hpp)

include_directories(${external_includes})

add_library(seg_src STATIC
        ${common_sources}
        ${common_headers}
)

target_link_libraries(seg_src ${external_libs})
