int binsearch2(int x, int v[], int n) {
    int low = 0;
    int high = n - 1;
    int mid;

    // ターゲットxがv[mid]以下である限り、highをmidに設定し続ける
    while (low < high) {
        mid = (low + high) / 2;

        // 比較は1回: xが中央より左側（または中央そのもの）にあるか？
        if (x <= v[mid]) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    // ループ終了時 (low == high) に、見つかったか確認する
    if (v[low] == x) {
        return low;  // 見つかった
    } else {
        return -1;  // 見つからなかった
    }
}
