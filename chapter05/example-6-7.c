#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 1. 構造体とそのポインタ型を typedef で定義 */
typedef struct tnode *Treeptr;

typedef struct tnode {
    char *word;
    int count;
    Treeptr left; /* struct tnode * の代わりに Treeptr を使用 */
    Treeptr right;
} Treenode;

/* 2. プロトタイプ宣言でも typedef を使用 */
Treeptr addtree(Treeptr p, char *w);
void treeprint(Treeptr p);

int main() {
    /* struct tnode *root ではなく Treeptr root と書ける */
    Treeptr root = NULL;

    root = addtree(root, "apple");
    root = addtree(root, "banana");
    root = addtree(root, "apple");
    root = addtree(root, "cherry");

    printf("--- Tree Contents ---\n");
    treeprint(root);

    return 0;
}

/* --- 関数の定義 --- */

/* 新しいノードを作成し、Treeptr 型として返す */
Treeptr addtree(Treeptr p, char *w) {
    int cond;

    if (p == NULL) {
        /* sizeof(Treenode) と書けるのでサイズ指定も分かりやすい */
        p = (Treeptr)malloc(sizeof(Treenode));
        p->word = strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;
    } else if (cond < 0) {
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }
    return p;
}

void treeprint(Treeptr p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treeprint(p->right);
    }
}
