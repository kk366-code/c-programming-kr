#include <ctype.h>
#include <stdio.h>

int atoi(char s[]) {
    int i;
    int n;
    int sign;

    // 1. 先頭の空白文字をスキップ
    for (i = 0; isspace(s[i]); i++) {
        //
    }

    // 2. 符号を決定
    sign = (s[i] == '-') ? -1 : 1;

    // 符号文字（'+'または'-'）をスキップ
    if (s[i] == '+' || s[i] == '-') {
        i++;
    }

    // 3. 数値部分を変換
    for (n = 0; isdigit(s[i]); i++) {
        n = 10 * n + (s[i] - '0');
    }

    // 4. 符号を適用して結果を返す
    return sign * n;
}

int main() {
    // 変換する文字列の配列
    char* strings[] = {
        "12345",      // 基本的な正の整数
        "-987",       // 負の整数
        "  +500",     // 先頭の空白とプラス符号
        "007",        // 先頭のゼロ
        "42abc",      // 途中に文字がある場合 (結果は42)
        "  -100end",  // 空白、負の符号、途中に文字がある場合
        "non-number"  // 数字が全くない場合 (結果は0)
    };

    int num_strings = sizeof(strings) / sizeof(strings[0]);

    printf("--- K&Rのatoi関数による文字列から整数への変換 ---\n\n");

    // 配列の各文字列に対してatoiを呼び出し、結果を出力
    for (int j = 0; j < num_strings; j++) {
        int result = atoi(strings[j]);
        printf("入力文字列: \"%s\" \t-> 変換結果: %d\n", strings[j], result);
    }

    return 0;
}
