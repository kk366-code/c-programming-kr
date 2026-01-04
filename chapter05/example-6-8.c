#include <stdio.h>

/* 定数の型を区別するための定数 */
#define INT 1
#define FLOAT 2
#define STRING 3

typedef struct {
    int type;
    union {
        int ival;
        float fval;
        char *sval;
    } u;
} Constant;

void print_constant(Constant c) {
    switch (c.type) {
        case INT:
            printf("Integer: %d\n", c.u.ival);
            break;
        case FLOAT:
            printf("Float: %f\n", c.u.fval);
            break;
        case STRING:
            printf("String: %s\n", c.u.sval);
            break;
    }
}

int main() {
    Constant c1, c2;

    c1.type = INT;
    c1.u.ival = 42;

    c2.type = STRING;
    c2.u.sval = "Hello Union";

    print_constant(c1);
    print_constant(c2);

    /* 注意！ivalを入れた後にfvalを読み出すと、データは壊れている */
    c1.u.fval = 3.14;
    printf("After changing to float, ival is garbage: %d\n", c1.u.ival);

    return 0;
}
