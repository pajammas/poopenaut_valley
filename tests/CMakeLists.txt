include_directories(${PROJECT_SOURCE_DIR}/)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})

include_directories(${external_includes})

add_subdirectory(unit)

### TEST MODULE COMMON ###
add_executable(test_common
                tests_all.cpp
		${srcs_common_all}
                )

target_link_libraries(test_common
                      ${GTEST_BOTH_LIBRARIES}
                      seg_src
                      ${external_libs}
		      )

### TEST INTEGRATION AND MODULES ###
add_executable(test_all
                tests_all.cpp
		${srcs_common_all}
                )

target_link_libraries(test_all
                      ${GTEST_BOTH_LIBRARIES}
                      seg_src
                      ${external_libs}
		            )