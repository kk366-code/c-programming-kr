// 関数の引数にポインタを使う（効率化）
// 構造体を関数に渡す際、ポインタを使うと「コピー」が発生しないためメモリを節約できる

#include <stdio.h>

struct point {
    int x;
    int y;
};

/* 構造体そのものではなく、その「住所(ポインタ)」を受け取る */
void move_point(struct point *p, int dx, int dy) {
    p->x += dx; /* 指された先の値を直接書き換える */
    p->y += dy;
}

int main() {
    struct point pt = {10, 20};

    /* アドレス(&)を渡す */
    move_point(&pt, 5, 5);

    /* 元のptが書き換わっている */
    printf("Moved point: (%d, %d)\n", pt.x, pt.y);  // (15, 25)

    return 0;
}
