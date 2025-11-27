#include <stdio.h>
#include <string.h>

void itoa(int n, char s[]);
void reverse(char s[]);

int main(void) {
    // 文字列を格納するためのバッファを用意します。
    // 十分なサイズ（例えば、32ビット整数の桁数10 + 符号1 + 終端文字1 = 12）
    // を確保する必要があります。
    char buffer[20];
    int num1 = 12345;
    int num2 = -987;
    int num3 = 0;

    printf("--- itoa関数 実行サンプル ---\n");

    // 1. 正の数を変換
    itoa(num1, buffer);
    printf("整数: %d -> 文字列: \"%s\"\n", num1, buffer);

    // 2. 負の数を変換
    itoa(num2, buffer);
    printf("整数: %d -> 文字列: \"%s\"\n", num2, buffer);

    // 3. ゼロを変換
    itoa(num3, buffer);
    printf("整数: %d -> 文字列: \"%s\"\n", num3, buffer);

    return 0;
}

void itoa(int n, char s[]) {
    int i;
    int sign;

    if ((sign = n) < 0) {
        // nが負の場合、その絶対値を取る
        n = -n;
    }

    i = 0;

    // do-whileループで、少なくとも一度は処理を実行し、逆順に数字を格納
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) {
        s[i++] = '-';
    }

    s[i] = '\0';
    reverse(s);
}

void reverse(char s[]) {
    int c;
    int i;
    int j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
