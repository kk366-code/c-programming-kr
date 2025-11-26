#include <stdio.h>

int bitcount(unsigned x);

int main(void) {
    unsigned int number = 13;  // 2進数で 0...01101
    int count = bitcount(number);

    printf("数値 %u の2進数表現に含まれる '1' のビットの数: %d\n", number, count);

    // 別の例
    number = 255;  // 2進数で 11111111 (8ビットの場合)
    count = bitcount(number);
    printf("数値 %u の2進数表現に含まれる '1' のビットの数: %d\n", number, count);

    // 別の例
    number = 0xAAAAAAAA;  // 10101010... (全ビット数の半分が 1)
    count = bitcount(number);
    printf(
        "数値 0xAAAAAAAA の2進数表現に含まれる '1' のビットの数: %d (一般的な "
        "32-bit 環境では 16)\n",
        count);

    return 0;
}

int bitcount(unsigned x) {
    int b;
    // ループの度に x を 1ビット右にシフト
    // 全ビットが0になったらループ終了
    for (b = 0; x != 0; x >>= 1) {
        // x の最下位ビットが 1 ならカウントアップ
        if (x & 01) {
            b++;
        }
    }

    return b;
}
