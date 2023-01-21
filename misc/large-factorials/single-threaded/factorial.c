#include "factorial.h"

#define PRINT_INTERVAL 1000

int main(int argc, char** argv) {
    if (argc == 2) {
        int n = atoi(argv[1]);
        char* result = factorial(n);

        if (strlen(result) > 10) {
            char sci[16] = {'\0'};
            sci[0] = result[0];
            sci[1] = '.';

            strncpy(sci + 2, result + 1, 9);

            printf("%d! = %s x 10^%zu\n", n, sci, strlen(result) - 1);
        }
        else {
            printf("%s\n", result);
        }
    }
    return 0;
}

char* factorial(int n) {
    char* str_result = malloc(2);
    str_result[0] = '1';
    str_result[1] = '\0';

    if (n == 0 || n == 1) {
        return str_result;
    }

    char str_multiplier[16] = {'\0'};

    //str_multiplier[16] puts a hard limit at 999999999999999! (15 chars)
    if (n < 0 || n > 999999999999999) {
        *str_result = '\0';
        return str_result;
    }

    /*
    products are the lines of multiplication results for each digit of the multiplier before summing
    ie, 111 * 99 in long multiplication is solved as

         111
        x 99
        ----
         999  <----- both of these are the 'products'
      + 9990  <-----
        ----
       10989
    */
    char** products = NULL;

    uint64_t startTime = TimeNow();

    //implement long multiplication
    for (int multiplier = 2; multiplier <= n; multiplier++) {

        int len_multiplier = snprintf(str_multiplier, 16, "%d", multiplier);

        RevStr(str_multiplier);

        products = calloc(len_multiplier, sizeof(char*));

        for (int i = 0; i < len_multiplier; i++) {
            products[i] = FactorialMul(str_result, str_multiplier[i], i);
        }

        free(str_result);

        str_result = SumProducts(products, len_multiplier, str_result);

        for (int i = 0; i < len_multiplier; i++) {
            free(products[i]);
        }
        free(products);
        products = NULL;

        if (multiplier % PRINT_INTERVAL == 0) {
            uint64_t elapsedTime = TimeNow() - startTime;
            printf("%d: %zu.%zu s\n", multiplier, elapsedTime / SEC_TO_NSEC, elapsedTime % SEC_TO_NSEC);
        }
    }

    uint64_t elapsedTime = TimeNow() - startTime;
    printf("Total elapsed: %zu.%zu s\n", elapsedTime / SEC_TO_NSEC, elapsedTime % SEC_TO_NSEC);

    RevStr(str_result);
    return str_result;
}

char* FactorialMul(char* multiplicand, char multiplier, size_t num_zeros) {

    // store the digits in a linked list since we dont know how many we'll need ahead of time
    LIST_DIGIT* list = NULL;
    int list_len = 0;

    while (num_zeros-- > 0) {
        //append a 0 for each xth digit of the multiplier
        // ie 9* 111 == 9 + 90 + 900
        list = PrependList(list, DigitIntToStr(0), &list_len);
    }

    int carry = 0;

    while (*multiplicand != '\0') {
        char prod = DigitStrToInt(*multiplicand) * DigitStrToInt(multiplier) + carry;
        multiplicand++;

        carry = prod / 10;

        list = PrependList(list, DigitIntToStr( prod % 10 ), &list_len);
    }

    if (carry > 0) {
        list = PrependList(list, DigitIntToStr(carry), &list_len);
    }

    char* output = malloc(list_len + 1);
    CollapseList(list, output);
    RevStr(output);
    return output;
}

LIST_DIGIT* PrependList(LIST_DIGIT* list, char value, int* count) {
    LIST_DIGIT* new = malloc(sizeof(LIST_DIGIT));
    new->digit = value;
    new->next = list;
    *count = *count + 1;
    return new;
}

void CollapseList(LIST_DIGIT* list, char* output) {
    while (list != NULL) {
        *output++ = list->digit;
        void* temp = list;
        list = list->next;
        free(temp);
    }
    *output = '\0';
}

char* SumProducts(char** products, int num_products, char* str_result) {
    int len_result = strlen(products[num_products-1]) * 2;
    str_result = malloc(len_result);
    //pointer iterators for each product line to be summed
    char** p_products = calloc(num_products, sizeof(char*));
    memcpy(p_products, products, num_products * sizeof(char*));

    char* p_sum = str_result;
    int carry = 0;
    bool done = false;

    while (!done) {
        //sum each product line
        //need a done flag because each product line will have a different number of digits
        done = true;
        int sum = 0;

        for (int i = 0; i < num_products; i++) {
            if (p_products[i] != NULL) {

                if (*p_products[i] != '\0') {
                    done = false;
                    sum += DigitStrToInt(*p_products[i]++);
                }
                else {
                    p_products[i] = NULL;
                }
            }
        }
        if (!done) {
            sum += carry;
            carry = sum / 10;
            *p_sum++ = DigitIntToStr( sum % 10 );
        }
    }
    if (carry > 0) {
        *p_sum++ = DigitIntToStr(carry);
    }
    *p_sum = '\0';

    return str_result;
}

void RevStr(char* str) {
    char* p1 = str;
    char* p2 = str + strlen(str) - 1;
    while (p1 < p2) {
        char temp = *p1;
        *p1++ = *p2;
        *p2-- = temp;
    }
}

uint64_t TimeNow(void) {
    struct timespec ts;
#ifdef _WIN32
    timespec_get(&ts, TIME_UTC);

#elif __linux__
    clock_gettime(CLOCK_MONOTONIC, &ts);

#endif
    return (uint64_t)(ts.tv_sec) * (uint64_t)SEC_TO_NSEC + (int64_t)ts.tv_nsec;
}
