#include <ctype.h>  // tolowerのために追加
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000
char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void qsort_kr(void *lineptr[], int left, int right, int (*comp)(void *, void *), int reverse);
int numcmp(const char *, const char *);

/* プロトタイプ宣言の追加 */
int ignore_case_cmp(const void *s1, const void *s2);

int main(int argc, char *argv[]) {
    freopen("input.txt", "r", stdin);  // 必要に応じて有効化

    int numeric = 0;
    int reverse = 0;
    int fold = 0;  // 大文字小文字無視フラグを追加

    // 引数の解析 (簡略的なループによる解析)
    while (--argc > 0 && (*++argv)[0] == '-') {
        char c;
        while ((c = *++argv[0])) {
            switch (c) {
                case 'n':
                    numeric = 1;
                    break;
                case 'r':
                    reverse = 1;
                    break;
                case 'f':
                    fold = 1;
                    break;
                default:
                    printf("sort: illegal option %c\n", c);
                    return 1;
            }
        }
    }

    int nlines = readlines(lineptr, MAXLINES);
    if (nlines < 0) {
        printf("input too big to sort\n");
        return 1;
    }

    // どの比較関数を使うか、優先順位を決めて選ぶ
    int (*comp)(void *, void *);
    if (numeric) {
        comp = (int (*)(void *, void *))numcmp;
    } else if (fold) {
        comp = (int (*)(void *, void *))ignore_case_cmp;
    } else {
        comp = (int (*)(void *, void *))strcmp;
    }
    qsort_kr((void **)lineptr, 0, nlines - 1, comp, reverse);
    writelines(lineptr, nlines);

    return 0;
}

void qsort_kr(void *v[], int left, int right, int (*comp)(void *, void *), int reverse) {
    int i, last;
    void swap(void *v[], int, int);

    if (left >= right) return;

    swap(v, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++) {
        // reverseが真なら、比較結果の符号を反転させる
        int compare_result = (*comp)(v[i], v[left]);

        if (reverse) {
            if (compare_result > 0) swap(v, ++last, i);
        } else {
            if (compare_result < 0) swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    qsort_kr(v, left, last - 1, comp, reverse);
    qsort_kr(v, last + 1, right, comp, reverse);
}

/* 大文字小文字を区別しない比較関数 */
int ignore_case_cmp(const void *s1, const void *s2) {
    /* 1. const char * へのキャスト (void* なので呼び出し側と型が一致する) */
    const char *p1 = (const char *)s1;
    const char *p2 = (const char *)s2;

    /* 2. \0 を終端として比較 */
    for (; tolower((unsigned char)*p1) == tolower((unsigned char)*p2); p1++, p2++) {
        if (*p1 == '\0') {
            return 0;
        }
    }

    /* 3. 差分を返す */
    return tolower((unsigned char)*p1) - tolower((unsigned char)*p2);
}

/* 以下、readlines, my_getline, writelines, numcmp, swap は変更なし（または既存の通り） */

int numcmp(const char *s1, const char *s2) {
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}

int readlines(char *lineptr[], int maxlines) {
    int len, nlines = 0;
    char *p, line[1000];
    int my_getline(char *, int);
    while ((len = my_getline(line, 1000)) > 0) {
        if (nlines >= maxlines || (p = malloc(len)) == NULL)
            return -1;
        else {
            line[len - 1] = '\0';
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

int my_getline(char s[], int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) s[i] = c;
    if (c == '\n') s[i++] = c;
    s[i] = '\0';
    return i;
}

void writelines(char *lineptr[], int nlines) {
    for (int i = 0; i < nlines; i++) printf("%s\n", lineptr[i]);
}

void swap(void *v[], int i, int j) {
    void *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
