#include "MyImageManipulation.h"

#ifdef BENCHMARK_ALLOW
#include <benchmark/benchmark.h>
#endif

#ifdef PICOBENCHMARK_ALLOW
#define PICOBENCH_IMPLEMENT_WITH_MAIN
#include <picobench/picobench.hpp>
const std::string srcImage  = "../big_image1.jpg";
const std::string destImage = "../big_image2.jpg";  /* MUST BE OF THE SAME SIZE!!!!*/
static MyImageManipulation manip(srcImage, destImage);
static void fillWithZero(picobench::state& state) {
    manip.recoverSrcImage();
    picobench::scope scope(state);
    for (auto _ : state)
    {
        manip.fillWithZero();
    }
}
static void fillWithZeroOptimized(picobench::state& state) {
    manip.recoverSrcImage();
    picobench::scope scope(state);
    for (auto _ : state)
    {
        manip.fillWithZeroOptimized();
    }
}
static void fillWithZeroSSE(picobench::state& state) {
    manip.recoverSrcImage();
    picobench::scope scope(state);
    for (auto _ : state)
    {
        manip.fillWithZeroSSE();
    }
}
static void fillWithZeroAVX(picobench::state& state) {
    manip.recoverSrcImage();
    picobench::scope scope(state);
    for (auto _ : state)
    {
        manip.fillWithZeroAVX();
    }
}
PICOBENCH_SUITE("Fill-Zero");
PICOBENCH(fillWithZero);
PICOBENCH(fillWithZeroOptimized);
PICOBENCH(fillWithZeroSSE);
PICOBENCH(fillWithZeroAVX);
#endif


#ifdef PICOBENCHMARK_ALLOW
#elif defined(BENCHMARK_ALLOW)
BENCHMARK_MAIN();
#else
int main(int argc, char** argv)
{
    std::string srcImage  = "../big_image1.jpg";
    std::string destImage = "../big_image2.jpg";  /* MUST BE OF THE SAME SIZE!!!!*/

	MyImageManipulation manip;

    bool ok = manip.setImageSrc(srcImage);

	if (ok) {
        //// DEBUG
	    std::cout << ImageManipulation::MAX_CHANTYPE << std::endl;
	    std::cout << SCALE << std::endl;

        //manip.displayImage("ORIGINAL image");
        manip.backupSrcImage();

        ////// Fill-Zero

        // manip.fillWithZero();
        // manip.displayImage("Normal Fill*0*");
        // manip.recoverSrcImage();
        
        manip.fillWithZeroOptimized();
        manip.displayImage("Optimized Fill*0*");
        manip.recoverSrcImage();
        
        // manip.fillWithZeroSSE();
        // manip.displayImage("SSE Fill*0*");
        // manip.recoverSrcImage();

        //manip.fillWithZeroAVX();
        //manip.displayImage("AVX Fill*0*");
        //manip.recoverSrcImage();

        //// Fill-Color
        //// RGBA
        //manip.setTargetColor(1.0, 0.0, 0.0, 0.0);

        //manip.fillWithColor();
        //manip.displayImage("Normal Fill*C*");
        //manip.recoverSrcImage();

        //manip.fillWithColorOptimized();
        //manip.displayImage("Optimized Fill*C*");
        //manip.recoverSrcImage();

        //manip.fillWithColorSSE();
        //manip.displayImage("SSE Fill*C*");
        //manip.recoverSrcImage();

        //manip.fillWithColorAVX();
        //manip.displayImage("AVX Fill*C*");
        //manip.recoverSrcImage();

        //// Fade-0
        //manip.setAlpha(.8);

        //manip.fadeToZero();
        //manip.displayImage("Normal Fade*0*-1");
        //manip.fadeToZero();
        //manip.displayImage("Normal Fade*0*-2");
        //manip.fadeToZero();
        //manip.displayImage("Normal Fade*0*-3");
        //manip.recoverSrcImage();

        //manip.fadeToZeroAVX();
        //manip.displayImage("AVX Fade*0*-1");
        //manip.fadeToZeroAVX();
        //manip.displayImage("AVX Fade*0*-2");
        //manip.fadeToZeroAVX();
        //manip.displayImage("AVX Fade*0*-3");
        //manip.recoverSrcImage();

        ////// Fade-Color
        //manip.setAlpha(.7);
        //// %RGBA
        //manip.setTargetColor(0.0, 0.0, 1.0, 0.0);

        //manip.fadeToColor();
        //manip.displayImage("Normal Fade*C*-1");
        //manip.fadeToColor();
        //manip.displayImage("Normal Fade*C*-2");
        //manip.fadeToColor();
        //manip.displayImage("Normal Fade*C*-3");
        //manip.recoverSrcImage();

        //manip.fadeToColorAVX();
        //manip.displayImage("AVX Fade*C*-1");
        //manip.fadeToColorAVX();
        //manip.displayImage("AVX Fade*C*-2");
        //manip.fadeToColorAVX();
        //manip.displayImage("AVX Fade*C*-3");
        //manip.recoverSrcImage();

        //manip.setAlpha(.70);
        //manip.setImageDest(destImage);
        //manip.fadeToImage();
        //manip.displayImage("Normal Fade*I*-1");
        //manip.fadeToImage();
        //manip.displayImage("Normal Fade*I*-2");
        //manip.fadeToImage();
        //manip.displayImage("Normal Fade*I*-3");

        //manip.setImageDest(destImage);
        //manip.fadeToImageAVX();
        //manip.displayImage("AVX Fade*I*-1");
        //manip.fadeToImageAVX();
        //manip.displayImage("AVX Fade*I*-2");
        //manip.fadeToImageAVX();
        //manip.displayImage("AVX Fade*I*-3");

    }

	return 0;
}
#endif