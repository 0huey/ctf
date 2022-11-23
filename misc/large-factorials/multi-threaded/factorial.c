#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#ifdef _WIN32
#pragma warning(disable: 4996)  //strncpy
#include <windows.h>
#include <time.h>
#define _USE_32BIT_TIME_T
#endif

#define DIGIT_STR_INT_CONV '0'
#define NUM_PRODUCTS 6
#define MEM_SIZE 500000
#define NUM_WORKERS 4

typedef struct _THREAD_CALLBACK_PARAMS {
    char * output;
    char * multiplicand;
    char * multiplier;
    size_t iMultiplier;
} THREAD_CALLBACK_PARAMS;

char * factorial(int n);
VOID CALLBACK FactorialMul(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work);
char DigitStrToInt(char digit);
char DigitIntToStr(char digit);
void RevStr(char *str);
uint64_t TimeNow(void);

int main(int argc, char**argv) {
    if (argc == 2) {
        char * result = factorial(atoi(argv[1]));

        if (strlen(result) > 10) {
            char sci[16] = {'\0'};
            sci[0] = result[0];
            sci[1] = '.';

            strncpy(sci + 2, result + 1, 9);

            printf("%s x 10^%zu\n", sci, strlen(result) - 1);
        }
        else {
            printf("%s\n", result);
        }
    }
    return 0;
}

char * factorial(int n) {
    char * szMultiplicand = malloc(MEM_SIZE);
    szMultiplicand[0] = '1';
    szMultiplicand[1] = '\0';

    if (n == 0 || n == 1) {
        return szMultiplicand;
    }
    if (n < 0 || n > 999999) {
        * szMultiplicand = '\0';
        return szMultiplicand;
    }

    char szMultiplier[16] = {'\0'};

    // 'products' are the lines of multiplication results for each digit of the multiplier before summing
    // NUM_PRODUCTS is the max length of the multiplier supported
    // so with NUM_PRODUCTS == 6, we could calculate up to 999999!
    char * products[NUM_PRODUCTS];

    for (int i = 0; i < NUM_PRODUCTS; i++) {
        // 100000! has 456573 digits
        char * mem = malloc(MEM_SIZE);
        * mem = '\0';
        products[i] = mem;
    }

    uint64_t startTime = TimeNow();

    TP_CALLBACK_ENVIRON callBackEnviron;
    InitializeThreadpoolEnvironment(&callBackEnviron);
    PTP_POOL pool = CreateThreadpool(NULL);
    SetThreadpoolThreadMaximum(pool, NUM_WORKERS);
    SetThreadpoolThreadMinimum(pool, NUM_WORKERS);
    SetThreadpoolCallbackPool(&callBackEnviron, pool);

    //implement long multiplication
    for (int multiplier = 2; multiplier <= n; multiplier++) {

        snprintf(szMultiplier, 16, "%d", multiplier);
        RevStr(szMultiplier);

        THREAD_CALLBACK_PARAMS params[NUM_PRODUCTS];
        PTP_WORK work[NUM_PRODUCTS] = {NULL};

        for (size_t iMultiplier = 0; iMultiplier < strlen(szMultiplier) && iMultiplier < NUM_PRODUCTS; iMultiplier++) {
            params[iMultiplier].output = products[iMultiplier];
            params[iMultiplier].multiplicand = szMultiplicand;
            params[iMultiplier].multiplier = szMultiplier;
            params[iMultiplier].iMultiplier = iMultiplier;

            work[iMultiplier] = CreateThreadpoolWork((PTP_WORK_CALLBACK)FactorialMul, &params[iMultiplier], &callBackEnviron);

            SubmitThreadpoolWork(work[iMultiplier]);
        }

        for (int i = 0; i < NUM_PRODUCTS; i++) {
            if (work[i] != NULL) {
                WaitForThreadpoolWorkCallbacks(work[i], false);
            }
        }

        //pointer iterators for each product line to be summed
        char * pProducts[NUM_PRODUCTS];
        for (int i = 0; i < NUM_PRODUCTS; i++) {
            pProducts[i] = products[i];
        }

        char * pSzSum = szMultiplicand;
        int carry = 0;
        bool done = false;
        char * digit;

        while (!done) {
            //sum each product line
            done = true;
            int sum = 0;

            for (int i = 0; i < NUM_PRODUCTS; i++) {
                if (pProducts[i] != NULL) {
                    digit = pProducts[i]++;

                    if (* digit != '\0') {
                        done = false;
                        sum += DigitStrToInt(* digit);
                    }
                    else {
                        pProducts[i] = NULL;
                    }
                }
            }
            if (!done) {
                sum += carry;
                carry = sum / 10;
                * pSzSum++ = DigitIntToStr( sum % 10 );
            }
        }
        if (carry > 0) {
            * pSzSum = DigitIntToStr(carry);
            pSzSum++;
        }
        * pSzSum = '\0';

        if (multiplier % 100 == 0) {
            uint64_t elapsedTime = TimeNow()  -  startTime;
            printf("%d: %zu.%zu s\n", multiplier, elapsedTime / 1000000000, elapsedTime%1000000000);
        }
    }

    for (int i = 0; i < NUM_PRODUCTS; i++) {
        free(products[i]);
    }

    RevStr(szMultiplicand);
    return szMultiplicand;
}

VOID CALLBACK FactorialMul(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work) {
    THREAD_CALLBACK_PARAMS *params = (THREAD_CALLBACK_PARAMS *)context;

    char * pSzProduct = params->output;
    char * szMultiplicand = params->multiplicand;
    char * szMultiplier = params->multiplier;
    size_t iMultiplier = params->iMultiplier;

    for (size_t i = 0; i < iMultiplier; i++) {
        //append a 0 for each xth digit of the multiplier
        // ie 9* 111 == 9 + 90 + 900
        * pSzProduct++ = DigitIntToStr(0);
    }

    int carry = 0;

    for (size_t iMultiplicand = 0; iMultiplicand < strlen(szMultiplicand); iMultiplicand++) {
        char mul1 = DigitStrToInt( szMultiplicand[iMultiplicand] );
        char mul2 = DigitStrToInt( szMultiplier[iMultiplier] );

        char prod = (mul1 * mul2) + carry;
        carry = prod / 10;

        * pSzProduct++ = DigitIntToStr( prod % 10 );
    }
    if (carry > 0) {
        * pSzProduct++ = DigitIntToStr(carry);
    }
    * pSzProduct = '\0';
}


char DigitStrToInt(char digit) {
    return digit - DIGIT_STR_INT_CONV;
}

char DigitIntToStr(char digit) {
    return digit + DIGIT_STR_INT_CONV;
}

void RevStr(char * str) {
    char * p1 = str;
    char * p2 = str  +  strlen(str)  -  1;
    while (p1 < p2) {
        char temp = * p1;
        * p1++ = * p2;
        * p2-- = temp;
    }
}

uint64_t TimeNow(void) {
#ifdef _WIN32
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return 1000000000 * ts.tv_sec + ts.tv_nsec;
#else
    return 0;
#endif
}
