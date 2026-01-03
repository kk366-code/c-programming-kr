#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100
#define MAXNODES 10000 /* 扱う単語の最大種類数 */

/* 行番号を保持する連結リスト */
struct linklist {
    int lnum;
    struct linklist *ptr;
};

/* 二分木のノード */
struct tnode {
    char *word;
    int count;
    struct tnode *left;
    struct tnode *right;
};

/* 関数プロトタイプ */
struct tnode *addtree(struct tnode *, char *);
void store_nodes(struct tnode *);
int compare_counts(const void *, const void *);
int getword(char *, int);

/* グローバル変数（ノードを配列に集める用） */
struct tnode *node_list[MAXNODES];
int node_count = 0;

int main() {
    freopen("exercise-6-3.c", "r", stdin);  // 必要に応じて有効化

    char word[MAXWORD];
    struct tnode *root = NULL;

    /* 1. 木を作って単語をカウント */
    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) {
            root = addtree(root, word);
        }
    }

    /* 2. 木のノードを配列にコピー */
    store_nodes(root);

    /* 3. 配列を「出現回数の多い順」にソート */
    qsort(node_list, node_count, sizeof(struct tnode *), compare_counts);

    /* 4. 結果を表示 */
    printf("\n--- Sorted by Frequency ---\n");
    for (int i = 0; i < node_count; i++) {
        printf("%4d %s\n", node_list[i]->count, node_list[i]->word);
    }

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

struct tnode *addtree(struct tnode *p, char *w) {
    int cond;

    if (p == NULL) { /* 新しい単語：ノードを作成 */
        p = (struct tnode *)malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->count = 1; /* 最初に出現したので 1 */
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++; /* 既存の単語：カウントを増やす */
    } else if (cond < 0) {
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }
    return p;
}

/* 木の全ノードを配列 node_list に格納する（再帰） */
void store_nodes(struct tnode *p) {
    if (p != NULL && node_count < MAXNODES) {
        store_nodes(p->left);
        node_list[node_count++] = p;
        store_nodes(p->right);
    }
}

/* qsort用の比較関数：countの降順 */
int compare_counts(const void *a, const void *b) {
    struct tnode *p1 = *(struct tnode **)a;
    struct tnode *p2 = *(struct tnode **)b;

    if (p1->count > p2->count) return -1;
    if (p1->count < p2->count) return 1;
    return 0; /* 同じ回数ならそのまま */
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
