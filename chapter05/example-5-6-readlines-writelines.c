/* ポインタ配列を用いたソート: 文字列全体ではなく、そのポインタを操作することで、効率的なソートを実現する */
#include <stdio.h>
#include <string.h>

#define MAXLINES 5000 /* ソートする最大の行数 */

/* ソート対象のアドレスを格納する配列をグローバル変数として宣言 */
/* 行の先頭を指しているから、インクリメントすると次の行の先頭を指し示すようになる */
char *lineptr[MAXLINES];

/* 関数プロトタイプ宣言 */
int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void qsort_kr(char *lineptr[], int left, int right);
int kr_getline(char *s, int lim);

/* 独自のメモリ管理関数（簡易的なスタティック配列版） */
#define ALLOCSIZE 10000          /* 使用可能なスペースのサイズ */
static char allocbuf[ALLOCSIZE]; /* allocとafree用ストレージ */
static char *allocp = allocbuf;  /* 次の空き位置へのポインタ */

/* 独自のメモリ割り当て関数 */
/* 標準ライブラリのmalloc/freeを使わずに、
   大きな配列をバッファとして使い、独自の単純なメモリ管理（alloc/afree）を実装する
 */
char *alloc(int n) {
    /* allocbufの終端内に入るか確認 */
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n; /* 以前のpを返す */
    } else {
        return 0; /* 空きがない場合はNULLを返す */
    }
}

/* 独自のメモリ解放関数 (実際はallocpを巻き戻すだけ) */
void afree(char *p) {
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE) allocp = p;
}

/* 入力されたテキストの行を読み込み、それらをアルファベット順にソートして出力する */
/* コンソールにCtrl+DでEOFを入力できない場合は入力内容をinput.txtに書いて以下のコマンドを実行
   gcc ./example-5-6-readlines-writelines.c
   ./a.out < input.txt
*/
int main() {
    int nlines;

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        /* 標準ライブラリのqsortと区別するため、qsort_krという名前に変更 */
        qsort_kr(lineptr, 0, nlines - 1);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("error: input too big to sort\n");
        return 1;
    }
}

#define MAXLEN 1000
char *alloc(int); /* allocのプロトタイプは上に移動 */

/* 入力行を読み込む */
int readlines(char *lineptr[], int maxlines) {
    int len;
    int nlines;
    char *p;
    char line[MAXLEN];

    nlines = 0;
    while ((len = kr_getline(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || (p = alloc(len)) == NULL) {
            return -1;
        } else {
            /* 読み込んだ行の最後に改行が含まれているため、それを削除 */
            /* K&Rのgetlineは改行を残すため、ここでlenの代わりにlen-1を使用 */
            line[len - 1] = '\0'; /* 改行をNULL文字に置き換える */

            /* 行のポインタをlineptrに格納し、バッファからメモリを確保 */
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

/* 出力行を書き出す */
void writelines(char *lineptr[], int nlines) {
    int i;

    for (i = 0; i < nlines; i++) {
        printf("%s\n", lineptr[i]);
    }
}

/* ---------------------------------------------------- */
/* 不足していた関数の定義をK&Rから補完 */
/* ---------------------------------------------------- */

/* stdio.hのgetlineとの衝突を避ける目的でkr_プレフィックスを追加 */
/* kr_getline: sに1行を読み込み、長さを返す */
int kr_getline(char *s, int lim) {
    int c, i;

    i = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n') s[i++] = c;
    if (c == '\n') s[i++] = c;
    s[i] = '\0';
    return i;
}

/* stdlib.hを使わずに独自で実装している関数には_krサフィックスを追加 */
/* qsort_kr: ポインタ配列v[left]...v[right]をソートする */
/* ソートするのは文字列の内容そのものではなく、lineptr配列に格納されているポインタ（アドレス）。
   これにより、長い文字列全体を移動させることなく、ポインタの入れ替えだけで効率的にソートが行える。
 */
void qsort_kr(char *v[], int left, int right) {
    int i, last;
    void swap(char *v[], int i, int j);

    if (left >= right) /* 要素が2つ未満なら何もしない */
        return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
        if (strcmp(v[i], v[left]) < 0) swap(v, ++last, i);
    swap(v, left, last);
    qsort_kr(v, left, last - 1);
    qsort_kr(v, last + 1, right);
}

/* swap: iとjの要素を入れ替える */
void swap(char *v[], int i, int j) {
    char *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
