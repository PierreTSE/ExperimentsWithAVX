add_executable(basics FusedMultAdd.cpp)
target_compile_options(basics PUBLIC -mfma)

find_package(benchmark)
target_compile_definitions(basics PUBLIC -DBENCHMARK_ALLOW)
target_link_libraries(basics PUBLIC benchmark)
target_link_libraries(basics PUBLIC pthread)