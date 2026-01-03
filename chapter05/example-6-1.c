#include <math.h> /* sqrt関数（平方根）を使うため */
#include <stdio.h>

/* 1. 構造体の定義（ひな形を作る） */
struct point {
    int x;
    int y;
};

/* 2. 構造体変数の宣言と初期化 */
struct point pt = {320, 200};

/* 3. 構造体を入れ子（ネスト）にする定義 */
struct rect {
    struct point pt1;
    struct point pt2;
};

int main() {
    /* 構造体変数の宣言 */
    struct point pt;
    struct point maxpt = {640, 480};

    /* メンバーへのアクセス（ドット演算子） */
    pt.x = 10;
    pt.y = 10;

    /* 構造体の値を使った計算（原点からの距離） */
    double dist;
    dist = sqrt((double)pt.x * pt.x + (double)pt.y * pt.y);

    printf("Point coordinate: (%d, %d)\n", pt.x, pt.y);
    printf("Distance from origin: %.2f\n", dist);
    printf("Max screen size: %d x %d\n", maxpt.x, maxpt.y);

    /* ネストされた構造体の操作 */
    struct rect screen;
    screen.pt1.x = 0; /* rectの中のpointの中のx */
    screen.pt1.y = 0;
    screen.pt2 = maxpt; /* 構造体ごとの代入も可能 */

    printf("Screen: (%d,%d) to (%d,%d)\n", screen.pt1.x, screen.pt1.y, screen.pt2.x, screen.pt2.y);

    return 0;
}
