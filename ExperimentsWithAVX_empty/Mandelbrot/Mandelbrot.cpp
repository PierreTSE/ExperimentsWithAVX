// Inspired from: https://software.intel.com/en-us/articles/introduction-to-intel-advanced-vector-extensions

// Standard outputs/strings
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include <immintrin.h>

// TODO: include AVX intrinsics

using namespace std;


// Compute Mandelbrot (Basic C++ with objects)
#include <complex>
void mandelbrotCPU(float x1, float y1, float x2, float y2, int width, int height, int maxIters, unsigned short *image) {
	float dx = (x2-x1)/width, dy = (y2-y1)/height;
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			complex<float> c (x1+dx*i, y1+dy*j), z(0,0);
			unsigned short count = 0;
			while ((count++ < maxIters) && (norm(z) < 4.0))
				z = z*z+c;
			*image++ = count;
		}
}

// Compute Mandelbrot (Basic C++ with simple floats)
void mandelbrotCPU2(float x1, float y1, float x2, float y2, int width, int height, int maxIters, unsigned short *image) {
    float dx = (x2-x1)/width, dy = (y2-y1)/height;
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
        {
            float ca = x1+dx*i, cb = y1+dy*j, za = 0, zb = 0;
            unsigned short count = 0;
            while ((count++ < maxIters) && (za*za+zb*zb < 4.0*4.0)) {
                float temp = (za*za - zb*zb) + ca;
                zb = (2*za*zb) + cb;
                za = temp;
            }
            *image++ = count;
        }
}

unsigned short hMax16(__m128i a)
{
    a = _mm_subs_epu16(_mm_set1_epi16(0x7FFF), a);
    return 0x7FFF - _mm_cvtsi128_si32(_mm_minpos_epu16(a));
}

// Compute Mandelbrot with SSE
void mandelbrotSSE(float x1, float y1, float x2, float y2, int width, int height, int maxIters, unsigned short *image) {
    __m128 dx = _mm_set1_ps((x2-x1)/width), dy = _mm_set1_ps((y2-y1)/height);
    __m128 x1sse = _mm_set1_ps(x1);
    __m128 y1sse = _mm_set1_ps(y1);
    __m128 thresh = _mm_set1_ps(16.0);
    __m128 two = _mm_set1_ps(2);
    __m128i zero = _mm_setzero_si128();
    __m128i one = _mm_set1_epi16(1);
    for (int j = 0; j < height; j++)
    {
        __m128 jsse = _mm_set1_ps(j);
        for(int i = 0; i < width; i += 4)
        {
            __m128 isse = _mm_set_ps(i, i + 1, i + 2, i + 3);
            __m128 ca = _mm_add_ps(_mm_mul_ps(isse, dx), x1sse);
            __m128 cb = _mm_add_ps(_mm_mul_ps(jsse, dy), y1sse);
            __m128 za = _mm_set1_ps(0);
            __m128 zb = _mm_set1_ps(0);

            __m128i count = zero;
            int cond2;
            do 
            {
                __m128 temp = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(za, za), _mm_mul_ps(zb, zb)), ca);
                zb = _mm_add_ps(_mm_mul_ps(two, _mm_mul_ps(za, zb)), cb);
                za = temp;
                __m128 cond = _mm_cmplt_ps(_mm_add_ps(_mm_mul_ps(za, za), _mm_mul_ps(zb, zb)), thresh);
                cond2 = _mm_movemask_ps(cond);
                count = _mm_adds_epu16(count, _mm_blend_epi16(zero, one, cond2));
            } while((hMax16(count) < maxIters) && cond2 > 0);

            memcpy(image, &count, 8);
            image += 4;
        }
    }
}

// Compute Mandelbrot with AVX
void mandelbrotAVX(float x1, float y1, float x2, float y2, int width, int height, int maxIters, unsigned short *image) {
    // TODO: implement
}

// Display images
#include <opencv2/opencv.hpp>
// COLORMAP_AUTUMN = 0,
// COLORMAP_BONE = 1,
// COLORMAP_JET = 2,
// COLORMAP_WINTER = 3,
// COLORMAP_RAINBOW = 4,
// COLORMAP_OCEAN = 5,
// COLORMAP_SUMMER = 6,
// COLORMAP_SPRING = 7,
// COLORMAP_COOL = 8,
// COLORMAP_HSV = 9,
// COLORMAP_PINK = 10,
// COLORMAP_HOT = 11,
// COLORMAP_PARULA = 12
using namespace cv;
void displayImage(string title, unsigned short *image, int size) {
    // Transform image to OpenCV type
    Mat colored(size, size*2, CV_8UC1, image, Mat::AUTO_STEP/*sizeof(unsigned short)*size*/);  //HACK: unsigned short = 2U = 2*8 = 16 bits => devrait être size*size*CV16
    applyColorMap(colored, colored, COLORMAP_JET);
    // Show the image & wait for keypress
    namedWindow(title);
    imshow(title, colored);
    waitKey(0);
    destroyWindow(title);
}


int main(int argc, char **argv)
{
	int size = 200;

	unsigned short *image1 = new unsigned short[size*size];
	unsigned short *image2 = new unsigned short[size*size];
	unsigned short *image3 = new unsigned short[size*size+4];
	unsigned short *image4 = new unsigned short[size*size];

    mandelbrotCPU(0.29768f, 0.48364f, 0.29778f, 0.48354f, size, size, 4096, image1);
    cout << "displaying CPU" << endl;
    displayImage("Normal CPU", image1, size);

    mandelbrotCPU2(0.29768f, 0.48364f, 0.29778f, 0.48354f, size, size, 4096, image2);
    cout << "displaying CPU-Optimized" << endl;
    displayImage("CPU Optimized", image2, size);

    mandelbrotSSE(0.29768f, 0.48364f, 0.29778f, 0.48354f, size, size, 4096, image3);
    cout << "displaying SSE" << endl;
    displayImage("SSE", image3, size);

    //mandelbrotAVX(0.29768f, 0.48364f, 0.29778f, 0.48354f, size, size, 4096, image4);
    //cout << "displaying AVX" << endl;
    //displayImage("AVX", image4, size);

	delete [] image1;
	delete [] image2;
	delete [] image3;
	delete [] image4;
}