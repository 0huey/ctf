#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #pragma warning(disable: 4996)  //strncpy
    #define _USE_32BIT_TIME_T
#endif

typedef struct _LIST_DIGIT {
    struct _LIST_DIGIT* next;
    char digit;
} LIST_DIGIT;

#define SEC_TO_NSEC 1000000000

#define DIGIT_STR_INT_CONV '0'
#define DigitStrToInt(digit) (digit - DIGIT_STR_INT_CONV)
#define DigitIntToStr(digit) (digit + DIGIT_STR_INT_CONV)

char* factorial(int n);
char* FactorialMul(char* multiplicand, char multiplier, size_t num_zeros);
LIST_DIGIT* PrependList(LIST_DIGIT* list, char value, int* count);
void CollapseList(LIST_DIGIT* list, char* output);
char* SumProducts(char** products, int num_products, char* szMultiplicand);
void RevStr(char* str);
uint64_t TimeNow(void);
