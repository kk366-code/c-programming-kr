// 線形探索 (O(n)): 要素が増えるほど時間がかかる。
// 二分木 (O(log n)): かなり速いが、木のバランスが崩れると遅くなる。
// ハッシュテーブル (O(1)): 原理上、要素の数に関わらず一瞬で目的のデータにアクセスできる。

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

struct nlist {          /* テーブルの要素 */
    struct nlist *next; /* 次の要素へのポインタ */
    char *name;         /* 定義名（例: IN） */
    char *defn;         /* 定義内容（例: 1） */
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE]; /* ポインタの配列（ハッシュテーブル） */

/* 関数プロトタイプ */
unsigned hash(char *s);
struct nlist *lookup(char *s);
struct nlist *install(char *name, char *defn);
void undef(char *s);
int getword(char *, int);
void skip_spaces(void);

/* --- main関数 --- */
int main() {
    freopen("input.txt", "r", stdin);  // 必要に応じて有効化

    char word[MAXWORD];
    struct nlist *np;

    while (getword(word, MAXWORD) != EOF) {
        if (strcmp(word, "#") == 0) { /* ディレクティブの開始 */
            getword(word, MAXWORD);
            if (strcmp(word, "define") == 0) {
                char name[MAXWORD], defn[MAXWORD];

                skip_spaces();
                getword(name, MAXWORD); /* 名前を取得 */
                skip_spaces();

                /* 定義内容（行末まで）を取得 */
                int i = 0, c;
                while ((c = getchar()) != '\n' && c != EOF)
                    if (i < MAXWORD - 1) defn[i++] = c;
                defn[i] = '\0';

                install(name, defn);
            } else if (strcmp(word, "undef") == 0) {
                skip_spaces();
                getword(word, MAXWORD);
                undef(word);
            }
        } else if (isalpha(word[0])) {
            /* 通常の単語：ハッシュテーブルにあれば置換 */
            if ((np = lookup(word)) != NULL)
                printf("%s", np->defn);
            else
                printf("%s", word);
        } else {
            /* 記号や空白はそのまま出力 */
            printf("%s", word);
        }
    }
    return 0;
}

/* --- 補助関数 --- */

/* getwordの後の余分な空白を飛ばす */
void skip_spaces(void) {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    ungetc(c, stdin);
}

/* --- ハッシュテーブルの実装部分 --- */

/* 文字列からハッシュ値を生成する */
unsigned hash(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}
/* 名前からノードを探す */
struct nlist *lookup(char *s) {
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0) return np;
    return NULL;
}

/* 名前と定義を登録・更新する */
struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) { /* 見つからなかった */
        np = (struct nlist *)malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL) return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else { /* すでに存在する */
        free((void *)np->defn);
    }
    if ((np->defn = strdup(defn)) == NULL) return NULL;
    return np;
}

/* undef: 名前と定義をテーブルから削除する */
void undef(char *s) {
    struct nlist *np, *prev;
    unsigned hashval;

    hashval = hash(s);
    prev = NULL;

    /* 該当するハッシュバケットの連結リストを走査 */
    for (np = hashtab[hashval]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0) {
            /* 見つかった場合：リストから切り離す */
            if (prev == NULL) {
                /* リストの先頭だった場合 */
                hashtab[hashval] = np->next;
            } else {
                /* リストの途中または末尾だった場合 */
                prev->next = np->next;
            }

            /* メモリの解放 */
            free((void *)np->name);
            free((void *)np->defn);
            free((void *)np);
            return;
        }
        prev = np; /* 前の要素を記録しておく */
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
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    /* 空白を飛ばすのではなく、そのまま出力して次の文字へ */
    while (isspace(c = getch())) {
        putchar(c);
        if (c == '\n') {
            /* 改行後の # 処理などのために必要ならここで行番号管理も可能 */
        }
    }

    if (c != EOF) *w++ = c;

    if (!isalpha(c) && c != '#') { /* # も単語の開始として扱う */
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
