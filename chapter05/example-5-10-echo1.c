// 1. echoという名前でコンパイル
// gcc example-5-10-echo1.c -o echo

// 2. 引数を渡して実行
// ./echo hello, world

#include <stdio.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("%s%s", argv[i], (i < argc - 1) ? " " : "");
    }
    printf("\n");

    return 0;
}
