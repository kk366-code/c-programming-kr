#include <stdio.h>

// 配列vの要素を昇順にソートする。nは要素数。
void shellsort(int v[], int n) {
    int gap;
    int i;
    int j;
    int temp;

    // 1. ギャップ（間隔）の決定と縮小
    for (gap = n / 2; gap > 0; gap /= 2) {
        // 2. 現在のギャップiから配列を走査
        for (i = gap; i < n; i++) {
            // 3. ギャップ挿入ソートの実行
            // v[j]とv[j + gap]を比較し、v[j]が大きい場合に交換
            for (j = i - gap; j >= 0 && v[j] > v[j + gap]; j -= gap) {
                // 要素の交換
                temp = v[j];
                v[j] = v[j + gap];
                v[j + gap] = temp;
            }
        }
    }
}

// 配列を出力するための補助関数
void print_array(const char* label, int arr[], int n) {
    printf("%s [", label);
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main() {
    // ソート対象の配列
    int data[] = {7, 12, 4, 9, 3, 1, 10, 5, 8, 2};
    int n = sizeof(data) / sizeof(data[0]);

    printf("--- K&Rのシェルソート関数による整列 ---\n\n");

    // ソート前の配列を出力
    print_array("ソート前:", data, n);

    // シェルソートを実行
    shellsort(data, n);

    printf("\n");

    // ソート後の配列を出力
    print_array("ソート後:", data, n);

    return 0;
}
