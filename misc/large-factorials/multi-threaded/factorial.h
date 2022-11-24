#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#ifdef _WIN32

#include <windows.h>
#include <time.h>
#pragma warning(disable: 4996)  //strncpy
#define _USE_32BIT_TIME_T

VOID CALLBACK FactorialMul(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work);

#elif __linux__

#include <pthread.h>

void* FactorialMul(void* context);
#endif

typedef struct _THREAD_CALLBACK_PARAMS {
    char* output;
    char* multiplicand;
    char* multiplier;
    size_t iMultiplier;
} THREAD_CALLBACK_PARAMS;

#define DIGIT_STR_INT_CONV '0'
#define NUM_PRODUCTS 6
#define MEM_SIZE 500000

char* factorial(int n);
void SumProducts(char** products, char* szMultiplicand);
char DigitStrToInt(char digit);
char DigitIntToStr(char digit);
void RevStr(char* str);
uint64_t TimeNow(void);
