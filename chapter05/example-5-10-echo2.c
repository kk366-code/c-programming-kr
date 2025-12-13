// 1. echoという名前でコンパイル
// gcc example-5-10-echo2.c -o echo

// 2. 引数を渡して実行
// ./echo hello, world

#include <stdio.h>

int main(int argc, char *argv[]) {
    while (--argc > 0) {
        printf("%s%s", *++argv, (argc > 1) ? " " : "");
    }
    printf("\n");

    return 0;
}
