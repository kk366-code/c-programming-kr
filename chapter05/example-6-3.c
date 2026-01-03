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
int binsearch(char *, struct key *, int);

/* メイン関数 */
int main() {
    freopen("input.txt", "r", stdin);  // 必要に応じて有効化

    int n;
    char word[MAXWORD];

    /* 単語を1つずつ読み込み、表にあればカウントアップ */
    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) {
            if ((n = binsearch(word, keytab, NKEYS)) >= 0) {
                keytab[n].count++;
            }
        }
    }

    /* 結果の表示 */
    for (n = 0; n < NKEYS; n++) {
        if (keytab[n].count > 0) {
            printf("%4d %s\n", keytab[n].count, keytab[n].word);
        }
    }
    return 0;
}

/* バイナリサーチ：単語を検索する */
int binsearch(char *word, struct key tab[], int n) {
    int cond;
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if ((cond = strcmp(word, tab[mid].word)) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid; /* 見つかった */
    }
    return -1; /* 見つからなかった */
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

/* getword: 入力から次の単語または文字を取得する */
int getword(char *word, int lim) {
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    /* 空白（スペース、タブ、改行）を飛ばす */
    while (isspace(c = getch()));

    if (c != EOF) *w++ = c;

    /* アルファベットでない場合は、その文字を返して終了 */
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }

    /* アルファベットまたは数字が続く限り読み込む */
    for (; --lim > 0; w++) {
        if (!isalnum(*w = getch())) {
            ungetch(*w); /* 読みすぎた文字を戻す */
            break;
        }
    }
    *w = '\0';
    return word[0];
}
