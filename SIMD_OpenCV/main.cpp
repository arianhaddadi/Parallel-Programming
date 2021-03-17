#include <iostream>
#include <x86intrin.h>
#include <sys/time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void printGroupMemberNames() {
	cout << "Group Members:" << endl;
	cout << "1 - Aryan Haddadi 810196448" << endl;
	cout << "2 - Iman Moradi 810196560" << endl;
	cout << "-------------" << endl;
}

void Q1() {
    long int parallelTime, serialTime;

    Mat inImg1 = imread("CA03__Q1__Image__01.png", IMREAD_GRAYSCALE);
    Mat inImg2 = imread("CA03__Q1__Image__02.png", IMREAD_GRAYSCALE);
    Mat serialOut (inImg1.rows, inImg1.cols, CV_8U);
    Mat parallelOut (inImg1.rows, inImg1.cols, CV_8U);

    unsigned int NCOLS = inImg1.cols;
    unsigned int NROWS = inImg1.rows;

    struct timeval start, end;


    // Serial
    gettimeofday(&start, NULL);

    unsigned char* inImg1PointerSerial = (unsigned char *) inImg1.data;
    unsigned char* inImg2PointerSerial = (unsigned char *) inImg2.data;
    unsigned char* outImgPointerSerial = (unsigned char *) serialOut.data;

    unsigned int offsetSerial = 0;
    char diff;
    for (int row = 0; row < NROWS; row++) {
        for (int col = 0; col < NCOLS; col++) {
            offsetSerial = row * NCOLS + col;
            diff = *(inImg1PointerSerial + offsetSerial) - *(inImg2PointerSerial + offsetSerial);
            *(outImgPointerSerial + offsetSerial) = diff >= 0 ? diff : -diff;
        }
    }

    gettimeofday(&end, NULL);
    serialTime  = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    // Parallel
    gettimeofday(&start, NULL);

    __m128i* inImg1PointerParallel = (__m128i *) inImg1.data;
    __m128i* inImg2PointerParallel = (__m128i *) inImg2.data;
    __m128i* OutImgPointerParallel = (__m128i *) parallelOut.data;
    __m128i firstImagePixelsSet, secondImagePixelsSet, output;
    unsigned int offsetParallel;

    for (int row = 0; row < NROWS; row++)
        for (int col = 0; col < NCOLS/16; col++) {
            offsetParallel = row * NCOLS/16 + col;
            firstImagePixelsSet = _mm_loadu_si128(inImg1PointerParallel + offsetParallel);
            secondImagePixelsSet = _mm_loadu_si128(inImg2PointerParallel + offsetParallel);
            output = _mm_or_si128(_mm_subs_epu8(firstImagePixelsSet, secondImagePixelsSet), _mm_subs_epu8(secondImagePixelsSet, firstImagePixelsSet));
            _mm_storeu_si128 (OutImgPointerParallel + offsetParallel, output);
        }

    gettimeofday(&end, NULL);
    parallelTime  = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    // display images
    namedWindow("Serial Output", WINDOW_AUTOSIZE);  
    imshow("Serial Output", serialOut);     

    namedWindow("Parallel Output", WINDOW_AUTOSIZE);    
    imshow("Parallel Output", parallelOut);

    waitKey(0);

    cout << "Run Time For Serial Calculation for Q1 is " << serialTime << " microseconds." << endl;
    cout << "Run Time For Parallel Calculation for Q1 is " << parallelTime << " microseconds." << endl;
    cout << "Q1 Speedup = " << float(serialTime) / float(parallelTime) << endl;


}


void Q2() {
    long int parallelTime, serialTime;

    Mat serialOut = imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);
    Mat parallelOut = imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);
    Mat inImg = imread("CA03__Q2__Image__02.png", IMREAD_GRAYSCALE);


    unsigned int NCOLS = inImg.cols;
    unsigned int NROWS = inImg.rows;
    unsigned int OUTCOLS = serialOut.cols;

    struct timeval start, end;

    // Serial
    gettimeofday(&start, NULL);

    unsigned char* inImgPointerSerial = (unsigned char *) inImg.data;
    unsigned char* OutImgPointerSerial = (unsigned char *) serialOut.data;

    unsigned int pixel;

    unsigned char* outAddrSerial;

    for (int row = 0; row < NROWS; row++) {
        for (int col = 0; col < NCOLS; col++) {
            outAddrSerial = OutImgPointerSerial + row * OUTCOLS + col;
            pixel = *(inImgPointerSerial + row * NCOLS + col) * 0.5 + *(outAddrSerial);
            *(outAddrSerial) = pixel <= 255 ? pixel : 255;
        }
    }

    gettimeofday(&end, NULL);
    serialTime = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;


    // Parallel
    gettimeofday(&start, NULL);

    __m128i* inImgPointerParallel = (__m128i *) inImg.data;
    __m128i* outImgPointerParallel = (__m128i *) parallelOut.data;

    __m128i in;
    __m128i mask = _mm_set1_epi8(0x7f);

    __m128i* outAddrParallel;

    for (int row = 0; row < NROWS; row++) {
        for (int col = 0; col < NCOLS/16; col++) {
            outAddrParallel = outImgPointerParallel + row * OUTCOLS/16 + col;
            in = _mm_loadu_si128(inImgPointerParallel + row * NCOLS/16 + col);
            in >> 1;
            in = _mm_and_si128(in, mask);
            in = _mm_adds_epu8(in, *(outAddrParallel));
			_mm_storeu_si128 (outAddrParallel, in);
        }
    }

    gettimeofday(&end, NULL);
    parallelTime = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    

    // display images
	namedWindow("Serial Output", WINDOW_AUTOSIZE); 	
	imshow("Serial Output", serialOut); 	

    namedWindow("Parallel Output", WINDOW_AUTOSIZE); 	
	imshow("Parallel Output", parallelOut); 

    waitKey(0);


    cout << "Run Time For Serial Calculation Q2 is " << serialTime << " microseconds." << endl;
    cout << "Run Time For Parallel Calculation Q2 is " << parallelTime << " microseconds." << endl;
    cout << "Q2 Speedup = " << float(serialTime) / float(parallelTime) << endl;

}

int main()
{
    printGroupMemberNames();
    Q1();
    Q2();
}

