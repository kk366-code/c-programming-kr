#include <stdio.h>

unsigned getbits(unsigned x, int p, int n);

int main(void) {
    // ターゲットの数値 x を 16進数で定義
    // 16進数: 0xAABBCCDD
    // 2進数: 1010 1010 1011 1011 1100 1100 1101 1101
    unsigned int x = 0xAABBCCDD;

    // 抽出パラメータ
    // p = 11: 右端から数えて 11 番目のビットを最上位とする
    // n = 5: 5 ビット分抽出する
    int p = 11;
    int n = 5;

    // 抽出するフィールド: ビット位置 7, 8, 9, 10, 11 (合計5ビット)
    // 2進数: ... 1100 1101 1101
    //            ^-----^
    //           11     7

    unsigned int result = getbits(x, p, n);

    printf("--- K&R getbits の実行例 ---\n");
    printf("元の値 x (16進): 0x%08X\n", x);
    printf("最上位ビット位置 p: %d\n", p);
    printf("抽出ビット数 n: %d\n", n);
    printf("---------------------------\n");
    printf("抽出された値 (10進): %u\n", result);
    printf("抽出された値 (16進): 0x%X\n", result);

    return 0;
}

// p の位置から n ビットを取ってくる
unsigned getbits(unsigned x, int p, int n) {
    return (x >> (p + 1 - n)) & ~(~0 << n);
}
