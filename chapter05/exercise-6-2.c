#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

struct tnode {
    char *word;
    int count;
    int match; /* 一致フラグ */
    struct tnode *left;
    struct tnode *right;
};

/* 関数プロトタイプ */
struct tnode *addtree_62(struct tnode *, char *, int, int *);
void treeprint_62(struct tnode *p, int n);
int getword(char *, int);

// このプログラムを ex62 という名前でコンパイルしてtest.cというファイルを読ませる場合：
// ./ex62 3 < test.c
int main(int argc, char *argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 6; /* デフォルト6文字 */
    struct tnode *root = NULL;
    char word[MAXWORD];
    int found = 0;

    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0]) && strlen(word) >= n) {
            found = 0; /* 新しい単語ごとにリセット */
            root = addtree_62(root, word, n, &found);
        }
    }

    /* ここで n を渡す */
    treeprint_62(root, n);

    return 0;
}

/* 演習6-2用：一致判定機能付きノード追加 */
struct tnode *addtree_62(struct tnode *p, char *w, int n, int *found) {
    int cond;

    if (p == NULL) {
        p = (struct tnode *)malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->count = 1;
        p->match = *found; /* 既に比較対象が見つかっていたらフラグを立てる */
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;
    } else if (cond < 0) {
        /* 左の枝を探す前に、n文字一致するかチェック */
        if (strncmp(w, p->word, n) == 0) {
            p->match = 1;
            *found = 1;
        }
        p->left = addtree_62(p->left, w, n, found);
    } else {
        /* 右の枝を探す前に、n文字一致するかチェック */
        if (strncmp(w, p->word, n) == 0) {
            p->match = 1;
            *found = 1;
        }
        p->right = addtree_62(p->right, w, n, found);
    }
    return p;
}

/* グループの区切りを空行で示す */
void treeprint_62(struct tnode *p, int n) {
    static char last_match[MAXWORD] = ""; /* 直前に表示した単語の接頭辞を保持 */

    if (p == NULL) return;

    treeprint_62(p->left, n);

    if (p->match) {
        /* 前の単語と最初のn文字が異なる場合、グループの区切りとして空行を入れる */
        if (last_match[0] != '\0' && strncmp(last_match, p->word, n) != 0) {
            printf("\n");
        }
        printf("%s\n", p->word);
        strncpy(last_match, p->word, n); /* 現在の接頭辞を記憶 */
    }

    treeprint_62(p->right, n);
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
