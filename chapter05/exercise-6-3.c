#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

/* 行番号を保持する連結リスト */
struct linklist {
    int lnum;
    struct linklist *ptr;
};

/* 二分木のノード */
struct tnode {
    char *word;
    struct linklist *lines; /* 行番号リストへのポインタ */
    struct tnode *left;
    struct tnode *right;
};

/* 関数プロトタイプ */
struct tnode *addtree_63(struct tnode *, char *, int);
void addline(struct tnode *, int);
void treeprint_63(struct tnode *);
int getword(char *, int);
int is_noise(char *);

int linenum = 1; /* 現在の行番号を追跡 */

int main() {
    freopen("exercise-6-3.c", "r", stdin);  // 必要に応じて有効化

    char word[MAXWORD];
    struct tnode *root = NULL;
    int c;

    while ((c = getword(word, MAXWORD)) != EOF) {
        if (isalpha(word[0]) && !is_noise(word)) {
            root = addtree_63(root, word, linenum);
        } else if (c == '\n') {
            linenum++; /* 改行をカウント */
        }
    }
    treeprint_63(root);
    return 0;
}

/* ノイズとなる単語（冠詞や接続詞）を除外 */
int is_noise(char *w) {
    static char *nw[] = {"a", "an", "and", "be", "for", "from", "in", "is", "it", "of", "on", "the", "to", "with"};
    int cond;
    /* 本来はバイナリサーチが望ましいが、簡易的に線形探索 */
    for (int i = 0; i < sizeof(nw) / sizeof(char *); i++) {
        if (strcmp(w, nw[i]) == 0) return 1;
    }
    return 0;
}

/* 行番号を追加（重複チェック付き） */
void addline(struct tnode *p, int lnum) {
    struct linklist *temp;

    /* すでにリストにあるか確認 */
    for (temp = p->lines; temp != NULL; temp = temp->ptr) {
        if (temp->lnum == lnum) return;
        if (temp->ptr == NULL) break; /* 末尾に到達 */
    }

    struct linklist *new_node = (struct linklist *)malloc(sizeof(struct linklist));
    new_node->lnum = lnum;
    new_node->ptr = NULL;

    if (p->lines == NULL) {
        p->lines = new_node;
    } else {
        temp->ptr = new_node; /* 末尾に追加 */
    }
}

/* 二分木への追加 */
struct tnode *addtree_63(struct tnode *p, char *w, int lnum) {
    int cond;
    if (p == NULL) {
        p = (struct tnode *)malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->lines = NULL;
        addline(p, lnum);
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        addline(p, lnum);
    } else if (cond < 0) {
        p->left = addtree_63(p->left, w, lnum);
    } else {
        p->right = addtree_63(p->right, w, lnum);
    }
    return p;
}

/* 木と行番号の表示 */
void treeprint_63(struct tnode *p) {
    struct linklist *l;
    if (p != NULL) {
        treeprint_63(p->left);
        printf("%10s: ", p->word);
        for (l = p->lines; l != NULL; l = l->ptr) {
            printf("%d%s", l->lnum, (l->ptr == NULL) ? "" : ", ");
        }
        printf("\n");
        treeprint_63(p->right);
    }
}

#define BUFSIZE 100
char buf[BUFSIZE]; /* ungetch用のバッファ */
int bufp = 0;      /* buf内の次の空き位置 */

int getch(void) { /* （押し戻された可能性のある）文字を取得 */ return (bufp > 0) ? buf[--bufp] : getchar(); }

void ungetch(int c) { /* 文字を入力に押し戻す */
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

/* 修正版 getword: 改行 '\n' をスキップせずにそのまま返す */
int getword(char *word, int lim) {
    int c;
    char *w = word;

    /* 空白を飛ばすが、改行だけは特別扱いして返す */
    while (isspace(c = getch())) {
        if (c == '\n') return c;
    }

    if (c != EOF) *w++ = c;
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for (; --lim > 0; w++) {
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
}
