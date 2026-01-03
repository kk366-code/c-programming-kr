#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

struct tnode {
    char *word;          /* 単語のテキストへのポインタ */
    int count;           /* 出現回数 */
    struct tnode *left;  /* 左の子へのポインタ (自分自身の型を指すポインタを持つ) */
    struct tnode *right; /* 右の子へのポインタ */
};

struct tnode *addtree(struct tnode *p, char *w);
void treeprint(struct tnode *p);
int getword(char *word, int lim);

int main() {
    freopen("input.txt", "r", stdin);  // 必要に応じて有効化

    struct tnode *root;
    char word[MAXWORD];

    root = NULL;
    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) root = addtree(root, word);
    }
    treeprint(root);
    return 0;
}

/* talloc: tnodeを作る */
struct tnode *talloc(void) { return (struct tnode *)malloc(sizeof(struct tnode)); }

/* strdup_kr: 文字列の複製を作る（名前の衝突を避けるため_krを付与） */
char *strdup_kr(char *s) {
    char *p;
    p = (char *)malloc(strlen(s) + 1); /* +1 は '\0' の分 */
    if (p != NULL) strcpy(p, s);
    return p;
}

/* addtree: wの位置、またはその下にpを持つノードを追加する */
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;

    if (p == NULL) { /* 新しい単語が到着：新しいノードを作る */
        p = talloc();
        p->word = strdup_kr(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++; /* すでに存在する単語：カウントアップ */
    } else if (cond < 0) {
        p->left = addtree(p->left, w); /* 小さければ左の枝へ */
    } else {
        p->right = addtree(p->right, w); /* 大きければ右の枝へ */
    }
    return p;
}

/* treeprint: 木pを順序通りに印字する（中順走査） */
void treeprint(struct tnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treeprint(p->right);
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

int getword(char *word, int lim) {
    int c, d;
    char *w = word;

    while (isspace(c = getch()));

    if (c != EOF) *w++ = c;

    if (c == EOF) {
        return EOF;
    } else if (c == '"') {
        /* 文字列定数の読み飛ばし */
        for (; --lim > 0; w++) {
            if ((*w = getch()) == '\\') { /* エスケープ文字対策 */
                *++w = getch();
            } else if (*w == '"') {
                w++;
                break;
            } else if (*w == EOF) {
                break;
            }
        }
    } else if (c == '#') {
        /* プリプロセッサ・ディレクティブの読み飛ばし */
        for (; --lim > 0; w++) {
            if ((*w = getch()) == '\n') {
                break;
            }
        }
    } else if (c == '/') {
        /* コメントの処理 */
        if ((d = getch()) == '*') {
            /* コメントの中身を読み飛ばす */
            for (; --lim > 0;) {
                if ((c = getch()) == '*') {
                    if ((d = getch()) == '/')
                        break;
                    else
                        ungetch(d);
                } else if (c == EOF)
                    break;
            }
        } else {
            ungetch(d);
        }
    }

    /* 単語（アルファベット、アンダースコアで始まる）の処理 */
    if (isalpha(c) || c == '_') {
        for (; --lim > 0; w++) {
            if (!isalnum(*w = getch()) && *w != '_') {
                ungetch(*w);
                break;
            }
        }
    }

    *w = '\0';
    return word[0];
}
