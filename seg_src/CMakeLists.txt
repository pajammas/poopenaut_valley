file(GLOB_RECURSE common_sources *.cpp *.cc)
file(GLOB_RECURSE common_headers *.h *.hpp)

include_directories(${PROJECT_SOURCE_DIR}/)
include_directories(${external_includes})

add_library(seg_src STATIC
        ${common_sources}
        ${common_headers}
)

target_link_libraries(seg_src ${external_libs})



# Create test executables
set(app_programs
	main)

foreach(app ${app_programs})
    add_executable(${app} ${app}.cpp)
    target_link_libraries(${app}
                          seg_src
                          ${external_libs})
endforeach()
