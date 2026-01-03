// 線形探索 (O(n)): 要素が増えるほど時間がかかる。
// 二分木 (O(log n)): かなり速いが、木のバランスが崩れると遅くなる。
// ハッシュテーブル (O(1)): 原理上、要素の数に関わらず一瞬で目的のデータにアクセスできる。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* --- main関数 --- */
int main() {
    install("MAXWORD", "100");
    install("EOF", "-1");

    printf("Before undef:\n");
    if (lookup("MAXWORD")) printf("MAXWORD exists\n");

    undef("MAXWORD");

    printf("\nAfter undef:\n");
    if (lookup("MAXWORD") == NULL) {
        printf("MAXWORD deleted successfully\n");
    }

    return 0;
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
