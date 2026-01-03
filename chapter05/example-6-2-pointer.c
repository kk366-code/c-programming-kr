#include <stdio.h>

struct point {
    int x;
    int y;
};

int main() {
    struct point origin, *pp;

    /* 1. 普通の変数としてアクセス */
    origin.x = 0;
    origin.y = 0;

    /* 2. ポインタにアドレスを代入 */
    pp = &origin;

    /* 3. -> 演算子を使ったアクセス */
    printf("origin is (%d,%d)\n", pp->x, pp->y);

    /* ポインタ経由で値を書き換える（元の変数も変わる） */
    pp->x = 100;
    printf("Updated origin.x: %d\n", origin.x);

    return 0;
}
