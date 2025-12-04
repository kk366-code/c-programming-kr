#include <stdio.h>
#include <string.h>  // strcpyを使用するために必要

#define ALLOCSIZE 10000 /* サイズを定義 */

static char allocbuf[ALLOCSIZE]; /* 管理対象のメモリ領域 */
static char *allocp = allocbuf;  /* 次の空き領域を指すポインタ */
// static char *allocp = &allocbuf[0]; /* 配列名はゼロ番目の要素のアドレスなので、こう書くこともできる */

/* メモリを割り当てる関数 */
char *alloc(int n) {
    // 同じ配列のアドレスは比較可能
    if (allocbuf + ALLOCSIZE - allocp >= n) {  // 入りきるか確認
        allocp += n;                           // ポインタをnだけ進める
        return allocp - n;                     // 古いポインタ（確保した領域の先頭）を返す
    } else {
        return 0;  // 領域不足
    }
}

/* メモリを解放する関数 */
void afree(char *p) {
    // pがバッファの範囲内であれば、allocpをpに戻す（リセット）
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE) {
        allocp = p;
    }
}

int main() {
    printf("--- K&R Alloc/Afree Test ---\n");
    printf("Buffer Start Address: %p\n\n", (void *)allocbuf);

    // 1. 10バイト割り当ててみる
    char *p1 = alloc(10);
    if (p1 != NULL) {
        strcpy(p1, "Hello");  // 文字列をコピー
        printf("[Alloc 1] Address: %p, Content: %s\n", (void *)p1, p1);
    }

    // 2. 続いて20バイト割り当ててみる
    char *p2 = alloc(20);
    if (p2 != NULL) {
        strcpy(p2, "World");
        printf("[Alloc 2] Address: %p, Content: %s\n", (void *)p2, p2);
    }

    // アドレスの差を確認（p2はp1の直後にあるはず）
    printf("   -> Diff: p2 is %ld bytes after p1\n\n", p2 - p1);

    // 3. メモリを解放する（p2を解放）
    // これにより、allocp が p2 の位置まで戻ります
    printf("[Free] Releasing p2...\n");
    afree(p2);

    // 4. 再度割り当ててみる（15バイト）
    // p2が解放されているので、同じ場所（p2のアドレス）が再利用されるはずです
    char *p3 = alloc(15);
    if (p3 != NULL) {
        strcpy(p3, "New Data");
        printf("[Alloc 3] Address: %p, Content: %s\n", (void *)p3, p3);
    }

    // 結果の検証
    if (p3 == p2) {
        printf("   -> Success: Memory address was reused!\n");
    } else {
        printf("   -> Note: Memory address was NOT reused.\n");
    }

    // 5. 巨大なメモリを要求して失敗させてみる
    char *p_fail = alloc(20000);  // ALLOCSIZE(10000)より大きい
    if (p_fail == 0) {
        printf("\n[Alloc 4] Too large allocation returned NULL as expected.\n");
    }

    return 0;
}
