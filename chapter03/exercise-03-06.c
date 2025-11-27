#include <limits.h>
#include <stdio.h>
#include <string.h>

// ----------------------
// プロトタイプ宣言
// ----------------------
void itoa(int n, char s[], int min_width);
void reverse(char s[]);

// ----------------------
// main関数（テスト用）
// ----------------------
int main(void) {
    char buffer[20];

    printf("--- K&R 演習 3-6 itoa(n, s, min_width) テスト ---\n");

    // 1. 指定幅よりも短い正の数 (幅 5)
    itoa(123, buffer, 5);
    printf("123 (幅 5) -> \"%s\"\n", buffer);  // 結果: "  123"

    // 2. 指定幅と同じ長さの負の数 (幅 5)
    itoa(-1234, buffer, 5);
    printf("-1234 (幅 5) -> \"%s\"\n", buffer);  // 結果: "-1234"

    // 3. 指定幅よりも長い数 (幅 3)
    itoa(12345, buffer, 3);
    printf("12345 (幅 3) -> \"%s\"\n", buffer);  // 結果: "12345"

    // 4. INT_MIN のテスト (幅 15)
    int smallest_neg = INT_MIN;
    itoa(smallest_neg, buffer, 15);
    printf("INT_MIN (幅 15) -> \"%s\"\n", buffer);  // 結果: " -2147483648"

    return 0;
}

// ------------------------------------
// 演習 3-6 の回答：最小フィールド幅を扱う itoa 関数
// ------------------------------------
void itoa(int n, char s[], int min_width) {
    int i, sign;

    // 符号を保存する
    if ((sign = n) < 0) {
        // INT_MIN 対策のため、n は負のままにしておく。
    }

    i = 0;

    // 1. 文字列への変換（逆順）
    do {
        int digit = n % 10;

        // 負の余りを正の数に変換する
        if (digit < 0) {
            digit = -digit;
        }

        s[i++] = digit + '0';

    } while ((n /= 10) != 0);

    // 2. 符号の追加
    if (sign < 0) {
        s[i++] = '-';
    }

    // 3. パディングの処理 (追加されたロジック)
    // i は現在、数字と符号を含めた文字列の長さになっている
    while (i < min_width) {
        s[i++] = ' ';  // スペースで埋める
    }

    // 4. 終端文字と反転
    s[i] = '\0';
    reverse(s);
}

// ------------------------------------
// reverse 関数（変更なし）
// ------------------------------------
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
