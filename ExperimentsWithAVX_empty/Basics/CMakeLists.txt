add_executable(basics FusedMultAdd.cpp)
target_compile_options(basics PUBLIC -mfma)