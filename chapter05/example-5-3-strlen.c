#include <stdio.h>

// strlen: 文字列 s の長さを返す。終端の '\0' は含まない。
int strlen(char *s);

int main(void) {
    char str_array[] = "hello, world";
    char *pa;

    // pa = &str_array[0];
    pa = str_array;

    // hello, worldを出力
    printf("%s\n", pa);
    printf("%s\n", str_array);

    // hとeを出力
    printf("%c\n", *pa);
    printf("%c\n", *(pa + 1));

    // hとeを出力
    printf("%c\n", str_array[0]);
    printf("%c\n", str_array[1]);

    // hとeを出力
    printf("%c\n", pa[0]);
    printf("%c\n", pa[1]);

    // 以下の2つは同じ
    printf("%p\n", &str_array[2]);
    printf("%p\n", pa + 2);

    // 以下の2つは同じ
    printf("%d\n", strlen(str_array));
    printf("%d\n", strlen(pa));
    return 0;
}

int strlen(char *s) {
    int n;

    for (n = 0; *s != '\0'; s++) {
        n++;
    }

    return n;
}
