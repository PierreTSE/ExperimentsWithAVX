add_executable(mandel Mandelbrot.cpp)
target_compile_options(mandel PUBLIC -mfma)

find_package(OpenCV)
target_link_libraries(mandel PUBLIC ${OpenCV_LIBS})