#include <iostream> // for cin/cout
#include <string>   // for std::strings
#include "Matrix.hpp"
#include <chrono>

#ifdef BENCHMARK_ALLOW
#include <benchmark/benchmark.h>
#endif

using namespace std;

constexpr int ROWS1 = 12;
constexpr int COLS1 = 4;
constexpr int ROWS2 = COLS1;
constexpr int COLS2 = 12;    // !!BEWARE!!: MUST BE A MULTIPLE OF REGISTERS' SIZE !!!!

/* Init/Zeroes the matrix */
void initMatrix(Matrix<float, ROWS1, COLS1>& a, Matrix<float, ROWS2, COLS2>& b) {
    a = {1, 2, 3, 4,
         5, 6, 7, 8, 
         9, 10, 11, 12, 
         13, 14, 15, 16, 
         17, 18, 19, 20, 
         21, 22, 23, 24, 
         25, 26, 27, 28, 
         29, 30, 31, 32, 
         33, 34, 35, 36, 
         37, 38, 39, 40, 
         41, 42, 43, 44, 
         45, 46, 47, 48};
    
    b = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
         13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 
         25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 
         37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48};
}

/* Display the matrix */
template<size_t ROW, size_t COL>
void printMatrix(std::string const& s, Matrix<float, ROW, COL>const& m) {
    std::cout << s << std::endl << m;
}

/* Computes matrix mult without optimization. Assumes res is zeroed */
Matrix<float, ROWS1, COLS2> multMatrix(Matrix<float, ROWS1, COLS1> const& a, Matrix<float, ROWS2, COLS2> const& b) {
    Matrix<float, ROWS1, COLS2> output;

    for (size_t i = 0; i < ROWS1; ++i) {
        for (size_t j = 0; j < COLS2; ++j) {
            for (size_t k = 0; k < COLS1; ++k) {
                output(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    return output;
}

/* Computes matrix mult without optimization. Assumes res is zeroed */
Matrix<float, ROWS1, COLS2> multMatrix2(Matrix<float, ROWS1, COLS1> const& a, Matrix<float, ROWS2, COLS2> const& b) {
    Matrix<float, ROWS1, COLS2> output;

    for (size_t i = 0; i < ROWS1; ++i) {
        for (size_t k = 0; k < COLS1; ++k) {
            for (size_t j = 0; j < COLS2; ++j) {
                output(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    return output;
}

/* Computes matrix mult with SSE optimization. Assumes res is zeroed
Goal: find the correct for loop to optimize!
*/
auto multMatrixSSE(Matrix<float, ROWS1, COLS1>& a, Matrix<float, ROWS2, COLS2>& b) {
    return a*b;
}

/* Computes matrix mult with SSE optimization. Assumes res is zeroed
Goal: find the correct for loop to optimize!
*/
auto multMatrixSSENoTranspose(Matrix<float, ROWS1, COLS1>& a, Matrix<float, ROWS2, COLS2>& b) {
    Matrix<float, ROWS1, COLS2> output;
    
    for (size_t i = 0; i < ROWS1; ++i) {
        for (size_t k = 0; k < COLS1; ++k) {
            for (size_t j = 0; j < COLS2; j+=4) {
                __m128 line1 = _mm_set1_ps(a(i, k));
                __m128 line2 = _mm_load_ps(b.data() + k*b.width()+j);
                __m128 previous = _mm_load_ps(output.data() + i*output.width() + j);
                previous = _mm_add_ps(previous, _mm_mul_ps(line1, line2));
                _mm_store_ps(output.data() + i*output.width() + j, previous);
            }
        }
    }
    
    
    return output;
}

/* Computes matrix mult with FMA optimization. Assumes res is zeroed */
Matrix<float, ROWS1, COLS2> multMatrixFMA(Matrix<float, ROWS1, COLS1>& a, Matrix<float, ROWS2, COLS2>& b) {
    Matrix<float, ROWS1, COLS2> output;

    for (size_t i = 0; i < ROWS1; ++i) {
        for (size_t k = 0; k < COLS1; ++k) {
            for (size_t j = 0; j < COLS2; j+=4) {
                __m128 line1 = _mm_set1_ps(a(i, k));
                __m128 line2 = _mm_load_ps(b.data() + k*b.width()+j);
                __m128 previous = _mm_load_ps(output.data() + i*output.width() + j);
                previous = _mm_fmadd_ps(line1, line2, previous);
                _mm_store_ps(output.data() + i*output.width() + j, previous);
            }
        }
    }


    return output;
}

#ifdef BENCHMARK_ALLOW
static void BM_basic(benchmark::State& state) {
    Matrix<float, ROWS1, COLS1> a;
    Matrix<float, ROWS2, COLS2> b;
    initMatrix(a, b);
    for (auto _ : state)
    {
        auto res = multMatrix(a, b);
        benchmark::DoNotOptimize(res);
    }
}
static void BM_sse(benchmark::State& state) {
    Matrix<float, ROWS1, COLS1> a;
    Matrix<float, ROWS2, COLS2> b;
    initMatrix(a, b);
    for (auto _ : state)
    {
        auto res = multMatrixSSE(a, b);
        benchmark::DoNotOptimize(res);
    }
}
static void BM_sseNoTranspose(benchmark::State& state) {
    Matrix<float, ROWS1, COLS1> a;
    Matrix<float, ROWS2, COLS2> b;
    initMatrix(a, b);
    for (auto _ : state)
    {
        auto res = multMatrixSSENoTranspose(a, b);
        benchmark::DoNotOptimize(res);
    }
}
static void BM_fma(benchmark::State& state) {
    Matrix<float, ROWS1, COLS1> a;
    Matrix<float, ROWS2, COLS2> b;
    initMatrix(a, b);
    for (auto _ : state)
    {
        auto res = multMatrixFMA(a, b);
        benchmark::DoNotOptimize(res);
    }
}
BENCHMARK(BM_basic);
BENCHMARK(BM_sse);
BENCHMARK(BM_sseNoTranspose);
BENCHMARK(BM_fma);
#endif

#ifndef BENCHMARK_ALLOW
int main()
{
    // Initialises rand mat1 & mat2
    Matrix<float, ROWS1, COLS1> mat1;
    Matrix<float, ROWS2, COLS2> mat2;
    initMatrix(mat1, mat2);

    auto time1 = std::chrono::high_resolution_clock::now();
    auto res = mat1 * mat2;
    auto time2 = std::chrono::high_resolution_clock::now();
    std::cout << "(" << (time2 - time1).count() << ")" << std::endl;
    printMatrix("RESU", res);
    printMatrix("RESU2", multMatrix2(mat1, mat2));

    auto r = multMatrixSSENoTranspose(mat1, mat2);
    printMatrix("RESU SSE", r);
    
    // print initial matrices
    //printMatrix("MAT1", mat1);
    //printMatrix("MAT2", mat2);

  return 0;
}
#else
BENCHMARK_MAIN();
#endif