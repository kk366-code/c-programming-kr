#include <stdio.h>

// 整数型 'unsigned int' のビット幅を取得するマクロ
// sizeof(unsigned int) はバイト数、CHAR_BIT は1バイトのビット数
#define INT_BITS (sizeof(unsigned int) * 8)

unsigned rightrot(unsigned x, int n);

int main() {
    // 16ビット環境を想定した値 0xAB12 (1010 1011 0001 0010)
    unsigned int x = 0xAB12;
    int n = 4;  // 4ビット回転

    unsigned int result = rightrot(x, n);

    printf("--- rightrot 関数の実行例 ---\n");
    printf("元の値 x (16進): 0x%04X\n", x);
    printf("回転ビット数 n: %d\n", n);
    printf("---------------------------\n");
    printf("結果 (16進): 0x%04X\n", result);

    // 動作確認 (n=4):
    // 元:  [1010] 1011 0001 0010
    // 右シフト: 1010 1011 0001 [1010]
    // 押し出し: 0010
    // ORで戻す: 0010 1010 1011 0001
    // 結果: 0x2AB1

    return 0;
}

/**
 * @brief x の値を右に n ビット回転（ローテート）させる。
 * @param x ターゲットの数値
 * @param n 回転させるビット数
 * @return 回転後の x の値
 */
unsigned rightrot(unsigned x, int n) {
    // 1. n の値をビット幅の範囲内に正規化（回転のオーバーフローを防ぐ）
    n = n % INT_BITS;

    // 2. x の右端から押し出される n ビットを抽出する (マスクを右シフト)
    //    ~(~0U << n) は下位 n ビットが 1 のマスク
    //    (x & ~(~0U << n)) で x の下位 n ビットが抽出される
    unsigned int pushed_out_bits = x & ~(~0U << n);

    // 3. 押し出されたビットを INT_BITS - n だけ左シフトし、左端に戻す
    unsigned int carried_in_bits = pushed_out_bits << (INT_BITS - n);

    // 4. x を n ビット右シフトし、押し出されたビットを OR でセットする
    return (x >> n) | carried_in_bits;
}
