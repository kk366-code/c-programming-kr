#include <stdio.h>
#include <string.h>

// K&Rからの引用のreverse関数
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

int main() {
    // 1. 反転する文字列を準備
    // 配列として宣言し、変更可能にする必要がある
    char str1[] = "hello";
    char str2[] = "programming";
    char str3[] = "a";
    char str4[] = "";  // 空文字列

    printf("--- reverse関数のテスト ---\n");

    // 2. 1つ目の文字列をテスト
    printf("元の文字列: %s\n", str1);
    reverse(str1);                     // 文字列を反転
    printf("反転後:   %s\n\n", str1);  // 出力: olleh

    // 3. 2つ目の文字列をテスト
    printf("元の文字列: %s\n", str2);
    reverse(str2);                     // 文字列を反転
    printf("反転後:   %s\n\n", str2);  // 出力: gnimmargorp

    // 4. 短い文字列をテスト
    printf("元の文字列: %s\n", str3);
    reverse(str3);                     // 文字列を反転
    printf("反転後:   %s\n\n", str3);  // 出力: a

    // 5. 空文字列をテスト
    printf("元の文字列: %s\n", str4);
    reverse(str4);                   // 文字列を反転
    printf("反転後:   %s\n", str4);  // 出力: (何も出力されない)

    return 0;
}
