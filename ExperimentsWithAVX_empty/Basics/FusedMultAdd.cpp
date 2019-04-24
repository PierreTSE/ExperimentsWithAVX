#include <iostream>
#include <string>
#include <immintrin.h>
#include <chrono>

using namespace std;

/* Code à optimiser */
void basic(float a[4], float b[4], float c[4], float res[4]) {
    for (int i = 0 ; i < 4 ; i++)
        res[i] = a[i]*b[i] + c[i];
}

/* simple optimization with SSE mult+add */
void sse(float a[4], float b[4], float c[4], float res[4]) {
    __m128 a_sse = _mm_load_ps(a);
    __m128 b_sse = _mm_load_ps(b);
    __m128 c_sse = _mm_load_ps(c);
    __m128 r = _mm_add_ps(_mm_mul_ps(a_sse, b_sse), c_sse);
    _mm_store_ps(res, r);
}

/* better optimization with FMA */
void fma(float a[4], float b[4], float c[4], float res[4]) {
    __m128 a_sse = _mm_load_ps(a);
    __m128 b_sse = _mm_load_ps(b);
    __m128 c_sse = _mm_load_ps(c);
    __m128 r = _mm_fmadd_ps(a_sse, b_sse, c_sse);
    _mm_store_ps(res, r);
}

/* displays the results & their timing */
void printRes(string title, float res[4], chrono::high_resolution_clock::time_point time1, chrono::high_resolution_clock::time_point time2) {
    cout << title << " (" << (time2 - time1).count() << "): " << res[0] << " " << res[1] << " " << res[2] << " " << res[3] << endl;
}

int main() {
    alignas(16) float a[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    alignas(16) float b[4] = { 5.0f, 6.0f, 7.0f, 8.0f };
    alignas(16) float c[4] = { 9.0f, 10.0f, 11.0f, 12.0f };
    alignas(16) float res[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
 
    //// Basic
    auto time1 = std::chrono::high_resolution_clock::now();
    basic(a, b, c, res);
    auto time2 = std::chrono::high_resolution_clock::now();
    printRes("BAS", res, time1, time2);

    ////// SSE
    time1 = std::chrono::high_resolution_clock::now();
    sse(a, b, c, res);
    time2 = std::chrono::high_resolution_clock::now();
    printRes("SSE", res, time1, time2);

    /////// FMA
    time1 = std::chrono::high_resolution_clock::now();
    fma(a, b, c, res);
    time2 = std::chrono::high_resolution_clock::now();
    printRes("FMA", res, time1, time2);

    return 0;
}