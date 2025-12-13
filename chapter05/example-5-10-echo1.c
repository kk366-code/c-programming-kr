// 1. echoという名前でコンパイル
// gcc example-5-10-echo1.c -o echo

// 2. 引数を渡して実行
// ./echo hello, world

#include <stdio.h>

// char *argv[] は、char * 型（文字へのポインタ、つまり文字列）の配列を意味する。
// C言語では、配列を関数に渡す際、それは配列の先頭要素へのポインタとして渡される。
// argv の実際の型は char **（char へのポインタへのポインタ）。

// char	1文字
// char *	文字列（char へのポインタ）
// char ** (または char *[])	文字列へのポインタの配列（ポインタのポインタ）

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("%s%s", argv[i], (i < argc - 1) ? " " : "");
    }
    printf("\n");

    return 0;
}
