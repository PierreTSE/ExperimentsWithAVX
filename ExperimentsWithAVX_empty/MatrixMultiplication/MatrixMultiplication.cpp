#include <iostream> // for cin/cout
#include <string>   // for std::strings
#include "Matrix.hpp"
#include <chrono>


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
void multMatrix(float *mat1/*fr*irc*/, float *mat2/*irc*fc*/, float *res/*fr*fc*/, int finalRows, int intermRowsCols, int finalCols) {
    // TODO: implement
}

/* Computes matrix mult with SSE optimization. Assumes res is zeroed
Goal: find the correct for loop to optimize!
*/
void multMatrixSSE(float *mat1/*fr*irc*/, float *mat2/*irc*fc*/, float *res/*fr*fc*/, int finalRows, int intermRowsCols, int finalCols) {
    // TODO: implement
 }

/* Computes matrix mult with FMA optimization. Assumes res is zeroed */
void multMatrixFMA(float *mat1/*fr*irc*/, float *mat2/*irc*fc*/, float *res/*fr*fc*/, int finalRows, int intermRowsCols, int finalCols) {
    // TODO: implement
}


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

    // print initial matrices
    printMatrix("MAT1", mat1);
    printMatrix("MAT2", mat2);
    printMatrix("RESU", res);

  return 0;
}
#else
BENCHMARK_MAIN();
#endif