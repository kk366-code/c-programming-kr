#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXWORD 100
#define NKEYS (sizeof(keytab) / sizeof(struct key))

struct key {
    char *word; /* キーワード（文字列へのポインタ） */
    int count;  /* 出現回数 */
};

/* 構造体の配列を定義し、初期化する */
struct key keytab[] = {
    {"auto", 0},     {"break", 0},  {"case", 0},     {"char", 0},   {"const", 0},    {"continue", 0}, {"default", 0},
    {"do", 0},       {"double", 0}, {"else", 0},     {"enum", 0},   {"extern", 0},   {"float", 0},    {"for", 0},
    {"goto", 0},     {"if", 0},     {"int", 0},      {"long", 0},   {"register", 0}, {"return", 0},   {"short", 0},
    {"signed", 0},   {"sizeof", 0}, {"static", 0},   {"struct", 0}, {"switch", 0},   {"typedef", 0},  {"union", 0},
    {"unsigned", 0}, {"void", 0},   {"volatile", 0}, {"while", 0},
};  // バイナリサーチを機能させるために、アルファベット順に並べる

/* 関数プロトタイプ宣言 */
int getword(char *, int);
struct key *binsearch(char *, struct key *, int);

/* メイン関数：ポインタ版 */
int main() {
    freopen("input.txt", "r", stdin);  // 必要に応じて有効化

    char word[MAXWORD];
    struct key *p;

    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) {
            /* binsearchが「構造体へのポインタ」を返すように変更 */
            if ((p = binsearch(word, keytab, NKEYS)) != NULL) {
                p->count++; /* ポインタ経由でカウントアップ */
            }
        }
    }

    /* 配列の走査もポインタで行う */
    for (p = keytab; p < keytab + NKEYS; p++) {
        if (p->count > 0) printf("%4d %s\n", p->count, p->word);
    }
    return 0;
}
/* バイナリサーチ：単語を検索する */
struct key *binsearch(char *word, struct key *tab, int n) {
    int cond;
    struct key *low = &tab[0];
    struct key *high = &tab[n]; /* 配列の最後の要素(n-1)の「次」を指す */
    struct key *mid;

    while (low < high) {
        // ポインタ同士の引き算 high - low は、その間にある要素の個数を返す
        // それを2で割り、low に足すことで、中間地点のポインタを算出
        // 直接 (low + high) / 2 と書くことはできない（ポインタ同士の足し算は禁止されているため）
        mid = low + (high - low) / 2; /* ポインタの演算 */
        if ((cond = strcmp(word, mid->word)) < 0)
            high = mid;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid; /* 見つかった構造体のポインタを返す */
    }
    return NULL;
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

/* 演習6-1: 改良版 getword */
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
