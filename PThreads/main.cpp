#include <iostream>
#include <pthread.h>
#include "ipp.h"

#define ARRAY_SIZE 1048576 // 2^20
#define MINIMUM_SIZE_OF_THREAD_CHUNK 10000
#define NUM_OF_THREADS 8

using namespace std;

typedef struct {
    int startIndex;
    int endIndex;
    float* arr;
} thread_input_q1;

typedef struct {
	int begin;
	int end;
	float* numbers;
} thread_input_q2;

float maxParallel;
int maxIndexParallel;

pthread_mutex_t lock;


void printGroupMemberNames() {
	cout << "Group Members:" << endl;
	cout << "1 - Aryan Haddadi 810196448" << endl;
	cout << "2 - Iman Moradi 810196560" << endl;
	cout << "-------------" << endl;
}


Ipp64u Q1Serial(float* arr) {
    Ipp64u start, end;

    float max = -1;
    int maxIndex;

    start = ippGetCpuClocks();
    
    for(int i = 0; i < ARRAY_SIZE; i++) {
        if (arr[i] > max) {
            max = arr[i];
            maxIndex = i;
        }
    }

    end = ippGetCpuClocks();


    cout << "Maximum Element in Serial Calculation is " << max << endl;
    cout << "Maximum Element Index in Serial Calculation is " << maxIndex << endl;
    return end - start;
}

void* findLocalMaximum(void* arg) {
    thread_input_q1* input = (thread_input_q1*) arg;
    float* arr = input->arr;
    int startIndex = input->startIndex;
    int endIndex = input->endIndex;

    float localMax = 0;
    int localMaxIndex;

    for(int i = startIndex; i < endIndex; i++) {
        if (arr[i] > localMax) {
            localMax = arr[i];
            localMaxIndex = i;
        }
    }

    pthread_mutex_lock(&lock);

    if (localMax > maxParallel) {
        maxParallel = localMax;
        maxIndexParallel = localMaxIndex;
    }

    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

Ipp64u Q1Parallel(float* arr) {
    Ipp64u start, end;

    maxParallel = 0;

    start = ippGetCpuClocks();

    pthread_t th[NUM_OF_THREADS];
    thread_input_q1 thread_inputs[NUM_OF_THREADS];
    int chunkSize = ARRAY_SIZE / NUM_OF_THREADS;

    pthread_mutex_init(&lock, NULL);

    for(int i = 0; i < NUM_OF_THREADS; i++) {
        thread_inputs[i] = {i * chunkSize, (i + 1) * chunkSize, arr};
        pthread_create(&th[i], NULL, findLocalMaximum, (void *) &thread_inputs[i]);
    }

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(th[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    end = ippGetCpuClocks();

    cout << "Maximum Element in Parallel Calculation is " << maxParallel << endl;
    cout << "Maximum Element Index in Parallel Calculation is " << maxIndexParallel << endl;
    return end - start;
}

void Q1() {
    cout << "Question 1" << endl << endl;

    Ipp64u serialCalculationTime, parallelCalculationTime;

	// Initialization
    srand((unsigned int)time(NULL));
    float *arr = new float[ARRAY_SIZE];
    for(int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = static_cast<float> (rand()) / static_cast<float> (RAND_MAX) * ARRAY_SIZE;
    }

	// Serial
    serialCalculationTime = Q1Serial(arr);
    cout << "Serial Calculation Time is " << serialCalculationTime << " Clock Cycles. " << endl;

	// Parallel
    parallelCalculationTime = Q1Parallel(arr);
    cout << "Parallel Calculation Time is " << parallelCalculationTime << " Clock Cycles. " << endl;
    
    cout << "Speedup is " << float(serialCalculationTime) / float(parallelCalculationTime) << endl;
	cout << "-------------" << endl;
}

void arrcpy(float* clone, float* origin, int size) {
	for (int i = 0; i < size; i++) {
		clone[i] = origin[i];
    }
}

void swap(float* numbers, int firstIndex, int secondIndex) {
	float temp = numbers[firstIndex];
	numbers[firstIndex] = numbers[secondIndex];
	numbers[secondIndex] = temp;	
}

int partition(float* numbers, int startIndex, int endIndex) {
	int pivotIndex = startIndex;
	float pivotNumber = numbers[pivotIndex];
	for (int i = startIndex + 1; i <= endIndex; i++) {
		if (numbers[i] <= pivotNumber) {
			pivotIndex++;
			swap(numbers, i, pivotIndex);
		}
	}
	swap(numbers, pivotIndex, startIndex);
	return pivotIndex;
}

void serialQuickSort(float* numbers, int startIndex, int endIndex) {
	if (startIndex >= endIndex)
		return;
	int pivot = partition(numbers, startIndex, endIndex);
	serialQuickSort(numbers, startIndex, pivot - 1);
	serialQuickSort(numbers, pivot + 1, endIndex);
}

void* parallelSort(void* arg) {
	thread_input_q2* param = (thread_input_q2*) arg;
	if (param->end - param->begin < MINIMUM_SIZE_OF_THREAD_CHUNK) {
		serialQuickSort(param->numbers, param->begin, param->end);
	}
	else {
        int pivot = partition(param->numbers, param->begin, param->end);

        thread_input_q2 firstThreadParam = {param->begin, pivot-1, param->numbers};
        thread_input_q2 secondThreadParam = {pivot+1, param->end, param->numbers};

        pthread_t firstThread, secondThread;

        pthread_create(&firstThread, NULL, parallelSort, &firstThreadParam);
        pthread_create(&secondThread, NULL, parallelSort, &secondThreadParam);

        pthread_join(firstThread, NULL);
        pthread_join(secondThread, NULL);
	}
	pthread_exit(NULL);
}

Ipp64u Q2Serial(float* arr) {
	Ipp64u start, end;

	start = ippGetCpuClocks();

	serialQuickSort(arr, 0, ARRAY_SIZE - 1);

	end = ippGetCpuClocks();

	return end - start;
}

Ipp64u Q2Parallel(float* arr) {
	Ipp64u start, end;

	start = ippGetCpuClocks();

	thread_input_q2 params = {0, ARRAY_SIZE-1, arr};
	pthread_t mainThread;
	pthread_create(&mainThread, NULL, parallelSort, &params);
	pthread_join(mainThread, NULL);

	end = ippGetCpuClocks();

	return end - start;
}

void Q2() {
	cout << "Question 2" << endl << endl;;

    Ipp64u serialCalculationTime, parallelCalculationTime;

	// Initialization
    srand((unsigned int)time(NULL));
    float *arr = new float[ARRAY_SIZE];
    for(int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = static_cast<float> (rand()) / static_cast<float> (RAND_MAX) * ARRAY_SIZE;
    }

	// Serial
	float* serialArr = new float[ARRAY_SIZE];
	arrcpy(serialArr, arr, ARRAY_SIZE);
	serialCalculationTime = Q2Serial(serialArr);

	// Parallel
	float* parallelArr = new float[ARRAY_SIZE];
	arrcpy(parallelArr, arr, ARRAY_SIZE);
	parallelCalculationTime = Q2Parallel(parallelArr);

	// Result Verification
	for (int i = 0; i < ARRAY_SIZE - 1; i++) {
		if (serialArr[i] > serialArr[i+1]) {
			printf("\u001b[31mERROR :: Serial Sort Checking Error\u001b[0m: Array[%d] = %f > %f = Array[%d]\n", i, serialArr[i], serialArr[i+1], i+1);
			return;
		}
		if (parallelArr[i] > parallelArr[i+1]) {
			printf("\u001b[31mERROR :: Parallel Sort Checking Error\u001b[0m: Array[%d] = %f > %f = Array[%d]\n", i, parallelArr[i], parallelArr[i+1], i+1);
			return;
		}
		if (parallelArr[i] != serialArr[i]) {
			printf("\u001b[31mERROR :: Parallel Serial Equality Checking Error\u001b[0m: Array[%d] = %f > %f = Array[%d]\n", i, parallelArr[i], serialArr[i], i+1);
			return;
		}
	}

	cout << "Sorting Results Verification was \u001b[32mSuccessful\u001b[0m." << endl;
    cout << "Serial Calculation Time is " << serialCalculationTime << " Clock Cycles. " << endl;
    cout << "Parallel Calculation Time is " << parallelCalculationTime << " Clock Cycles. " << endl;
    cout << "Speedup is " << float(serialCalculationTime) / float(parallelCalculationTime) << endl;

}


int main() {
	printGroupMemberNames();
    Q1();
	Q2();

	return 0;
}