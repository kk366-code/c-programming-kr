#include <limits.h>  // INT_MIN を使うために必要
#include <stdio.h>
#include <string.h>

// ----------------------
// プロトタイプ宣言
// ----------------------
void itob(int n, char s[], int b);
void reverse(char s[]);

// ----------------------
// main関数（テスト用）
// ----------------------
int main(void) {
    char buffer[100];  // 結果の文字列を格納するバッファ

    printf("--- K&R 演習 3-5 itob(n, s, b) テスト ---\n");

    // 1. 2進数 (基数 2)
    itob(255, buffer, 2);
    printf("10進数: 255 (b=2) -> \"%s\"\n", buffer);  // 結果: 11111111

    // 2. 8進数 (基数 8)
    itob(100, buffer, 8);
    printf("10進数: 100 (b=8) -> \"%s\"\n", buffer);  // 結果: 144

    // 3. 10進数 (基数 10)
    itob(-12345, buffer, 10);
    printf("10進数: -12345 (b=10) -> \"%s\"\n", buffer);  // 結果: -12345

    // 4. 16進数 (基数 16)
    itob(255, buffer, 16);
    printf("10進数: 255 (b=16) -> \"%s\"\n", buffer);  // 結果: FF

    // 5. INT_MIN と 16進数
    itob(INT_MIN, buffer, 16);
    printf("INT_MIN (%d) (b=16) -> \"%s\"\n", INT_MIN, buffer);  // 結果: -80000000

    // 6. 36進数 (0-9, A-Z を使用)
    itob(35 * 36 + 10, buffer, 36);
    printf("10進数: 1270 (b=36) -> \"%s\"\n", buffer);  // 結果: Y A

    return 0;
}

// ------------------------------------
// 演習 3-5 の回答：任意の基数 b に変換する itob 関数
// ------------------------------------
void itob(int n, char s[], int b) {
    int i, sign;
    int digit_val;  // 各桁の数値 (0〜b-1)

    // 符号を保存する
    if ((sign = n) < 0) {
        // INT_MIN 対策のため、n は負のままにしておく。
    }

    i = 0;

    do {
        // 基数 b で割った余りを取得
        // n が負の場合、n % b の結果は負またはゼロになる。
        digit_val = n % b;

        // 負の余りを正の数に変換する (例: -2 -> 2)
        if (digit_val < 0) {
            digit_val = -digit_val;
        }

        // 数値 (0〜35) を文字 ('0'〜'9', 'A'〜'Z') に変換
        if (digit_val <= 9) {
            // 0から9の場合
            s[i++] = digit_val + '0';
        } else {
            // 10から35の場合 (10は'A', 11は'B', ...)
            // 'A'のASCII値から 10 を引いた値に digit_val を足す
            s[i++] = digit_val - 10 + 'A';
        }

        // n を基数 b で割って次の桁へ移動 (n が負でも問題なく機能する)
    } while ((n /= b) != 0);

    // 元の数値が負であれば符号を追加
    if (sign < 0) {
        s[i++] = '-';
    }

    s[i] = '\0';
    reverse(s);  // 文字列を反転して正しい順序にする
}

// ------------------------------------
// reverse 関数（演習 3-4 から流用）
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
