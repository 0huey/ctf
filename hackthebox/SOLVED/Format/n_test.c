#include <stdio.h>

void main() {
    int x = 666;
    int a, b, c;

    printf("AAAA%3$nBBBB%100d%nCCCC%n\n", &a, x, &b, &c);

    printf("%d %d %d\n", a, b, c);
}
