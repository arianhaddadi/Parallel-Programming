#include <iostream>
#include <cmath>
#include <x86intrin.h>
#include "sys/time.h"


#define ARRAY_SIZE (1024 * 1024)

using namespace std;

typedef union {
    __m128 float128;
    float  fvec[4];
} floatReg;

void printGroupMemberNames() {
	cout << "Group Members:" << endl;
	cout << "1 - Aryan Haddadi 810196448" << endl;
	cout << "2 - Iman Moradi 810196560" << endl;
	cout << "-------------" << endl;
}

long int Q2SerialMode(float array[ARRAY_SIZE]) {
    struct timeval start, end;

    gettimeofday(&start, NULL);

    float meanTemp[4], mean, standardDeviationTemp[4], standardDeviation = 0;
    long int calculationTime;

    // Mean Calculation
    meanTemp[0] = meanTemp[1] = meanTemp[2] = meanTemp[3] = 0;
    for (int i = 0; i < ARRAY_SIZE; i+=4) {
        meanTemp[0] += array[i];
    }
    for (int i = 0; i < ARRAY_SIZE; i+=4) {
        meanTemp[1] += array[i + 1];
    }
    for (int i = 0; i < ARRAY_SIZE; i+=4) {
        meanTemp[2] += array[i + 2];
    }
    for (int i = 0; i < ARRAY_SIZE; i+=4) {
        meanTemp[3] += array[i + 3];
    }
    mean = (meanTemp[0] + meanTemp[1] + meanTemp[2] + meanTemp[3]) / ARRAY_SIZE;

    // Standard Devivation Calculation
    standardDeviationTemp[0] = standardDeviationTemp[1] = standardDeviationTemp[2] = standardDeviationTemp[3] = 0;
    for (int i = 0; i < ARRAY_SIZE; i+=4) {
        standardDeviationTemp[0] += (array[i] - mean) * (array[i] - mean);
    }
    for (int i = 0; i < ARRAY_SIZE; i+=4) {
        standardDeviationTemp[1] += (array[i + 1] - mean) * (array[i + 1] - mean);
    }
    for (int i = 0; i < ARRAY_SIZE; i+=4) {
        standardDeviationTemp[2] += (array[i + 2] - mean) * (array[i + 2] - mean);
    }
    for (int i = 0; i < ARRAY_SIZE; i+=4) {
        standardDeviationTemp[3] += (array[i + 3] - mean) * (array[i + 3] - mean);
    }
    standardDeviation = sqrt((standardDeviationTemp[0] + standardDeviationTemp[1] + standardDeviationTemp[2] + standardDeviationTemp[3]) / ARRAY_SIZE);

    gettimeofday(&end, NULL);

    cout << "Mean in Serial Calculation is " << mean << endl;
    cout << "Standard Deviation in Serial Calculation is " << standardDeviation << endl;
    calculationTime = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    cout << "Run Time For Serial Calculation is " << calculationTime << " microseconds.\n" << endl;
    return calculationTime;
}


long int Q2ParallelMode(float array[ARRAY_SIZE]) {
    struct timeval start, end;

    gettimeofday(&start, NULL);
    long int calculationTime;

    // Mean Calculation
    __m128 mean =  _mm_set1_ps(0.0f), numReg = _mm_set1_ps(ARRAY_SIZE);    
    for(int i = 0; i < ARRAY_SIZE; i += 4) {
        mean =_mm_add_ps(mean, _mm_loadu_ps(&array[i]));
    }
    mean = _mm_hadd_ps(mean, mean);
    mean = _mm_hadd_ps(mean, mean);
    mean = _mm_div_ps(mean, numReg);

    // Standard Devivation Calculation
    __m128 standardDeviation = _mm_set1_ps(0.0f), temp;
    for(int i = 0; i < ARRAY_SIZE; i += 4) {
        temp = _mm_sub_ps(_mm_loadu_ps(&array[i]), mean);
        standardDeviation = _mm_add_ps(standardDeviation, _mm_mul_ps(temp, temp));
    }
    standardDeviation = _mm_hadd_ps(standardDeviation, standardDeviation);
    standardDeviation = _mm_hadd_ps(standardDeviation, standardDeviation);
    standardDeviation = _mm_sqrt_ps(_mm_div_ps(standardDeviation, numReg));


    float outputs[2]; // 0 for mean, 1 for standard deviation
    outputs[0] = _mm_cvtss_f32(mean);
    outputs[1] = _mm_cvtss_f32(standardDeviation);
    gettimeofday(&end, NULL);
    cout << "Mean in Parallel Calculation is " << outputs[0] << endl;
    cout << "Standard Deviation in Parallel Calculation is " << outputs[1] << endl;
    calculationTime = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    cout << "Run Time For Parallel Calculation is " << calculationTime << " microseconds.\n" << endl;
    return calculationTime;
}

long int Q1SerialMode(float array[ARRAY_SIZE]) {
    struct timeval start, end;
    long int calculationTime;

    gettimeofday(&start, NULL);

    float max = -0.1;
    int maxIndex = 0;

    for (int i = 0; i < ARRAY_SIZE; i += 4) {
		if (max < array[i]) {
			max = array[i];
			maxIndex = i;
		}
	}
	for (int i = 1; i < ARRAY_SIZE; i += 4) {
		if (max < array[i]) {
			max = array[i];
			maxIndex = i;
		}
	}
	for (int i = 2; i < ARRAY_SIZE; i += 4) {
		if (max < array[i]) {
			max = array[i];
			maxIndex = i;
		}
	}
	for (int i = 3; i < ARRAY_SIZE; i += 4) {
		if (max < array[i]) {
			max = array[i];
			maxIndex = i;
		}
	}

    gettimeofday(&end, NULL);

    cout << "Maximum Element in Serial Calculation is " << max << endl;
    cout << "Index Of Maximum Element in Serial Calculation is " << maxIndex << endl;
    calculationTime = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    cout << "Run Time For Serial Calculation is " << calculationTime << " microseconds.\n" << endl;
    return calculationTime;
}


long int Q1ParallelMode(float array[ARRAY_SIZE]) {
    struct timeval start, end;
    long int calculationTime;

    gettimeofday(&start, NULL);

    floatReg maximumValues, maximumIndexes;
    maximumValues.float128 = _mm_set1_ps(-0.1f);
	maximumIndexes.float128 = _mm_set1_ps(0);
    __m128 currentValues, currentIndexes, criteria;
	for (int i = 0; i < ARRAY_SIZE; i += 4)
	{
		currentIndexes = _mm_set_ps(i + 3, i + 2, i + 1, i);
		currentValues = _mm_loadu_ps(&array[i]);
		criteria = _mm_cmpgt_ps(currentValues, maximumValues.float128);
		maximumIndexes.float128 = _mm_blendv_ps(maximumIndexes.float128, currentIndexes, criteria);
        maximumValues.float128 = _mm_max_ps(currentValues, maximumValues.float128);
	}


    float max = maximumValues.fvec[0];
    int maxIndex = maximumIndexes.fvec[0];

	for(int i = 1; i < 4; i++) {
		if(max < maximumValues.fvec[i]) {
			max = maximumValues.fvec[i];
			maxIndex = maximumIndexes.fvec[i];
		}
	}

    gettimeofday(&end, NULL);

    cout << "Maximum Element in Parallel Calculation is " << max << endl;
    cout << "Index Of Maximum Element in Parallel Calculation is " << maxIndex << endl;
    calculationTime = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    cout << "Run Time For Parallel Calculation is " << calculationTime << " microseconds.\n" << endl;
    return calculationTime;
}

void Q1(float array[ARRAY_SIZE]) {
    cout << "Maximum Element And Maximum Element Index Section:\n" << endl;
    

    long int serialCalculationTime = Q1SerialMode(array);
    long int parallelCalculationTime = Q1ParallelMode(array);

    cout << "Speedup: " << float(serialCalculationTime) / float(parallelCalculationTime) << endl;
	cout << "-------------" << endl;
}

void Q2(float array[ARRAY_SIZE]) {
    cout << "Mean and Standard Deviation Section:\n" << endl;
    
    

    long int serialCalculationTime = Q2SerialMode(array);
    long int parallelCalculationTime = Q2ParallelMode(array);

    cout << "Speedup: " << float(serialCalculationTime) / float(parallelCalculationTime) << endl;
	cout << "-------------" << endl;
}


int main() {
    srand((unsigned int)time(NULL));
    printGroupMemberNames();

    float array[ARRAY_SIZE];

    // Generate Array With Random Numbers in range 0 -> 100
    for(int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = static_cast<float> (rand()) / static_cast<float> (RAND_MAX) * 100;
    }
    Q1(array);
    Q2(array);

}