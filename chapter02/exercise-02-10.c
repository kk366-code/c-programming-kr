#include <stdio.h>

int lower(char input) {
    // ASCII文字コードは大文字と小文字が同じ間隔（距離）で並んでいる
    // 'a' - 'A' は 97 - 65 で 32
    // 大文字に小文字との距離である 32 を足すと小文字が得られる
    return ('A' <= input && input <= 'Z') ? input + 'a' - 'A' : input;
}

int main(void) {
    printf("%d\n", 'A');
    printf("%d\n", lower('B'));
    printf("%c\n", lower('B'));
    printf("%d\n", 'B');

    return 0;
}
