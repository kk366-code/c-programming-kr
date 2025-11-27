#include <limits.h>  // INT_MIN を使うために必要
#include <stdio.h>
#include <string.h>

void itoa(int n, char s[]);
void reverse(char s[]);

// main関数（テスト用）
int main(void) {
    char buffer[20];

    printf("--- K&R 演習 3-4 itoa テスト ---\n");

    // 1. INT_MIN のテスト (32bit環境で -2147483648)
    int smallest_neg = INT_MIN;
    itoa(smallest_neg, buffer);
    printf("INT_MIN (%d) -> \"%s\"\n", smallest_neg, buffer);

    // 2. 他の負の数のテスト
    itoa(-12345, buffer);
    printf("-12345 -> \"%s\"\n", buffer);

    // 3. 正の数と 0 のテスト（ロジックは変わらず正しく処理される）
    itoa(123, buffer);
    printf("123    -> \"%s\"\n", buffer);
    itoa(0, buffer);
    printf("0      -> \"%s\"\n", buffer);

    return 0;
}

// ------------------------------------
// 演習 3-4 の回答：INT_MIN を処理する itoa 関数
// ------------------------------------
void itoa(int n, char s[]) {
    int i, sign;

    // 符号を保存する
    sign = n;

    // n が負の場合、符号のチェックは後回しにして、
    // 以下の do-while ループ内での計算のために n を正の数として扱う。
    // ただし、INT_MIN の場合、単純に n = -n; とするとオーバーフローするため、
    // 以下の do-while ループ内での計算を工夫する。

    i = 0;

    do {
        // ここが重要な変更点。
        // 負の数の絶対値を取る代わりに、n % 10 の結果を正の数に変換する。
        // n が負の場合、n % 10 は負またはゼロになる。
        // 例：-12 % 10 は -2。これを正の数 2 に変換する。
        int digit = n % 10;
        if (digit < 0) {
            digit = -digit;
        }

        s[i++] = digit + '0';

        // n を更新。INT_MIN を含め、すべての n で n /= 10 が正しく次の桁を与える。
    } while ((n /= 10) != 0);  // ループ条件も n != 0 に変更するのが一般的

    // INT_MIN の場合、n の値は正しく n/=10 で減っていくため、
    // ここで正しい符号を追加できる。
    if (sign < 0) {
        s[i++] = '-';
    }

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
