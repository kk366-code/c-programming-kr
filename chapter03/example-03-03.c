#include <stdio.h>

// vはソート済みの整数配列
int binsearch(int x, int v[], int n) {
    int low = 0;
    int high = n - 1;
    int mid;

    while (low <= high) {
        // 中央のインデックスを求める
        mid = (low + high) / 2;

        // ターゲットが中央の値よりも小さければ探索範囲の右端を狭める
        if (x < v[mid]) {
            high = mid - 1;
            // ターゲットが中央の値よりも大きければ探索範囲の左端を狭める
        } else if (x > v[mid]) {
            low = mid + 1;
        } else {
            return mid;
        }
    }

    // lowがhighを超えた場合はターゲットが見つからなかったとみなして終了
    return -1;
}

int main(void) {
    // 重要な点: 配列は必ずソート（昇順）されている必要があります
    int sorted_array[] = {10, 20, 30, 40, 50, 60, 70, 80};
    int n = sizeof(sorted_array) / sizeof(sorted_array[0]);  // 要素数を計算
    int target = 40;
    int not_found = 99;

    // 存在する値の探索
    int index_found = binsearch(target, sorted_array, n);

    if (index_found != -1) {
        printf("値 %d はインデックス %d に見つかりました。\n", target, index_found);  // 出力: 3
    } else {
        printf("値 %d は見つかりませんでした。\n", target);
    }

    // 存在しない値の探索
    int index_not_found = binsearch(not_found, sorted_array, n);

    if (index_not_found != -1) {
        printf("値 %d はインデックス %d に見つかりました。\n", not_found, index_not_found);
    } else {
        printf("値 %d は見つかりませんでした。\n", not_found);  // 出力: 値 99 は見つかりませんでした。
    }

    return 0;
}
