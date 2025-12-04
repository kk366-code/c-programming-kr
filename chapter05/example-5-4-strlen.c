#include <stdio.h>

// ポインタ同士の引き算により距離（要素数）を返すことで文字列の長さを求める関数
int strlen(char *s) {
    char *p = s;  // 1. 作業用ポインタ p を用意し、先頭アドレス s で初期化

    while (*p != '\0') {  // 2. p が指す中身が '\0' (終端文字) でない間繰り返す
        p++;              // 3. p を次のアドレスへ進める
    }

    return p - s;  // 4. 終端のアドレス p から 先頭のアドレス s を引く
}

int main() {
    // ケース1: 通常の文字列リテラル
    char *str1 = "Hello, World!";
    int len1 = strlen(str1);

    printf("String: \"%s\"\n", str1);
    printf("Length: %d\n\n", len1);

    // ケース2: 配列として定義された文字列
    char str2[] = "C Programming";
    int len2 = strlen(str2);

    printf("String: \"%s\"\n", str2);
    printf("Length: %d\n\n", len2);

    // ケース3: 空文字（長さは0になるはず）
    char *str3 = "";
    int len3 = strlen(str3);

    printf("String: \"%s\" (Empty)\n", str3);
    printf("Length: %d\n\n", len3);

    // ケース4: 日本語（マルチバイト文字）の場合の注意
    // UTF-8環境では、日本語1文字は通常3バイトです。
    // strlenは「文字数」ではなく「バイト数」を返します。
    char *str4 = "あいう";
    int len4 = strlen(str4);

    printf("String: \"%s\"\n", str4);
    printf("Length: %d (Note: In UTF-8, 'あいう' is usually 9 bytes)\n", len4);

    return 0;
}
