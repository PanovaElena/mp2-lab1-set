set(target ${MP2_TESTS})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.c*")

add_executable(${target} ${srcs} ${hdrs})

target_include_directories(
        ${target}
        PUBLIC
        "${MP2_GTEST_INCLUDES}"
        "${MP2_INCLUDES}"
)

target_link_libraries(
        ${target}
        gtest
        gtest_main
        ${MP2_LIBRARY}
)

include(GoogleTest)

gtest_discover_tests(${target})