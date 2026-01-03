#include <stdio.h>

/* 座標を表す構造体 */
struct point {
    int x;
    int y;
};

/* 長方形を表す構造体（2つのpointを内包） */
struct rect {
    struct point pt1;
    struct point pt2;
};

/* 2つの値を比較するマクロ */
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/* pointを作る関数：x, yからstruct pointを組み立てて返す */
struct point makepoint(int x, int y) {
    struct point temp;
    temp.x = x;
    temp.y = y;
    return temp;
}

/* pointを加算する関数：2つの座標を足し合わせる */
// 値渡しなので、p1の内容は変更されない
struct point addpoint(struct point p1, struct point p2) {
    p1.x += p2.x;
    p1.y += p2.y;
    return p1;
}

/* 長方形を標準形にする関数：rがどのようなpt1を左下、pt2を右上に揃える */
// 座標がどんな順番で与えられても、pt1を左下（最小値）、pt2を右上（最大値）に揃えるための関数
// （例） 長方形の幅を計算するとき：
// 標準化前: abs(r.pt1.x - r.pt2.x) のように絶対値が必要。
// 標準化後: r.pt2.x - r.pt1.x だけで必ず正の値が得られる。
struct rect canonrect(struct rect r) {
    struct rect temp;
    temp.pt1.x = min(r.pt1.x, r.pt2.x);
    temp.pt1.y = min(r.pt1.y, r.pt2.y);
    temp.pt2.x = max(r.pt1.x, r.pt2.x);
    temp.pt2.y = max(r.pt1.y, r.pt2.y);
    return temp;
}

/* 点が長方形の中にあるか判定する関数 */
int ptinrect(struct point p, struct rect r) {
    return p.x >= r.pt1.x && p.x < r.pt2.x && p.y >= r.pt1.y && p.y < r.pt2.y;
}

int main() {
    /* 1. makepointを使って構造体を作成 */
    struct point p1 = makepoint(10, 20);
    struct point p2 = makepoint(5, 5);

    /* 2. addpointで座標を足し算 */
    struct point p3 = addpoint(p1, p2);
    printf("Add: (%d, %d) + (%d, %d) = (%d, %d)\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

    /* 3. 逆転した座標で長方形を作成 */
    struct rect screen;
    screen.pt1 = makepoint(100, 100);
    screen.pt2 = makepoint(10, 20);  // pt1より小さい値（逆転状態）

    /* 4. canonrectで標準形に修正 */
    struct rect net = canonrect(screen);
    printf("Canonical Rect: pt1(%d, %d), pt2(%d, %d)\n", net.pt1.x, net.pt1.y, net.pt2.x, net.pt2.y);

    /* 5. ptinrectで内外判定 */
    struct point test_pt = makepoint(50, 50);
    if (ptinrect(test_pt, net)) {
        printf("Point (%d, %d) is INSIDE the rectangle.\n", test_pt.x, test_pt.y);
    } else {
        printf("Point (%d, %d) is OUTSIDE.\n", test_pt.x, test_pt.y);
    }

    return 0;
}
