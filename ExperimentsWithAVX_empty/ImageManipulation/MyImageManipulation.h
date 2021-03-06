#ifndef __MY_IMAGE_MANIPULATION__
#define __MY_IMAGE_MANIPULATION__

#include <chrono>

#include "ImageManipulation.h"

class MyImageManipulation : public ImageManipulation {

public: 
    MyImageManipulation();
    MyImageManipulation(std::string const& src, std::string const& dest);
    static MyImageManipulation getInstance(std::string const& src, std::string const& dest);

    static const int MAX_TIMING_ITERATIONS;

    long timeMethod(void (MyImageManipulation::*methodToBeTimed)());

    void fillWithZero();           // dummy 'for' loop
    void fillWithZeroOptimized();  // loop on pointer
    void fillWithZeroSSE();        // use SSE registers
    void fillWithZeroAVX();        // use AVX registers

    void fillWithColor();          // dummy 'for' loop
    void fillWithColorOptimized(); // loop on pointer
    void fillWithColorSSE();       // use sse registers
    void fillWithColorAVX();       // use avx registers

    ///** Decreases pixels intensity towards 0 */
    void fadeToZero();           // loop on pointer
    void fadeToZeroAVX();        // use AVX registers

    ///** Decreases pixels intensity towards a constant color */
    // reuses setTargetColor()
    void fadeToColor();        // loop on pointer
    void fadeToColorAVX();     // use AVX registers

    ///** Decreases pixels intensity towards the color of the same pixel in the other image */
    void fadeToImage();       // loop on pointers
    void fadeToImageAVX();    // use AVX pointers

};

#endif