add_executable(multiply MatrixMultiplication.cpp Matrix.hpp test_macro.hpp)
target_compile_options(multiply PUBLIC -mavx2)

find_package(benchmark)
target_compile_definitions(multiply PUBLIC -DBENCHMARK_ALLOW)
target_link_libraries(multiply PUBLIC benchmark)
target_link_libraries(multiply PUBLIC pthread)