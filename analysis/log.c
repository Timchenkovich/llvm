#include <stdio.h>

void printUse(char* user, unsigned long long us_addr, char* operand, unsigned long long op_addr) {
    printf("(%llu) %s<-%s (%llu)\n", op_addr, operand, user, us_addr);
}