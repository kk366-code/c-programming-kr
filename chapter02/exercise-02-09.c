#include <limits.h>
#include <stdio.h>

/*
 * オリジナルの K&R bitcount: 全てのビットをチェックするため、
 * ループ回数は整数のビット幅 (例: 32回) に依存する。
 */
int bitcount_original(unsigned x) {
    int b;
    // ループ条件: x が 0 でなくなるまで (全てのビットをチェックする)
    for (b = 0; x != 0; x >>= 1) {
        if (x & 01) {
            b++;
        }
    }
    return b;
}

/*
 * 効率的な K&R bitcount (x = x & (x - 1) を利用):
 * ループ回数は、セットされたビット ('1' の数) の数に等しい。
 */
int bitcount_fast(unsigned x) {
    int b;
    // ループ条件: x が 0 でなくなるまで
    // 更新式: x &= (x - 1) により、最下位の 1 ビットがクリアされる。
    for (b = 0; x != 0; x &= (x - 1)) {
        b++;  // 1 ビットがクリアされるたびにカウント
    }
    return b;
}

int main() {
    // テスト用の数値 (例: 32ビット環境で 10個の '1' を持つ)
    unsigned int number = 0x000003FF;         // 10進数で 1023 (下位 10ビットが全て 1)
    unsigned int number_sparse = 0x80000001;  // 10進数で 2147483649 (2つの '1' を持つ)

    printf("--- bitcount 関数の比較 ---\n");
    printf("計算対象の型: unsigned int (通常 %zu ビット)\n", sizeof(unsigned int) * CHAR_BIT);

    // --- 密なビットのテスト ---
    printf("\n## 密なビット (10個の '1') のテスト (数値: %u)\n", number);
    int count_orig = bitcount_original(number);
    printf("1. オリジナル版の結果: %d\n", count_orig);

    int count_fast = bitcount_fast(number);
    printf("2. 効率的な版の結果:   %d\n", count_fast);
    // オリジナル版: ループ回数 ≈ 32回 | 効率的な版: ループ回数 = 10回

    // --- スパースなビットのテスト ---
    printf("\n## スパースなビット (2個の '1') のテスト (数値: %u)\n", number_sparse);
    count_orig = bitcount_original(number_sparse);
    printf("1. オリジナル版の結果: %d\n", count_orig);

    count_fast = bitcount_fast(number_sparse);
    printf("2. 効率的な版の結果:   %d\n", count_fast);
    // オリジナル版: ループ回数 ≈ 32回 | 効率的な版: ループ回数 = 2回

    return 0;
}
