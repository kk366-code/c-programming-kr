#include <ctype.h>  // tolowerのために追加
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000
char *lineptr[MAXLINES];

int fold = 0;
int directory = 0;
int numeric = 0;
int reverse = 0;

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void qsort_kr(void *lineptr[], int left, int right, int (*comp)(void *, void *), int reverse);
int numcmp(void *s1, void *s2);
int ignore_case_cmp(void *s1, void *s2);

/* プロトタイプ宣言の追加 */
int char_cmp(const void *s1, const void *s2, int fold, int dir);

// 帳尻合わせ用のラッパー関数
int char_cmp_wrapper(void *s1, void *s2) { return char_cmp(s1, s2, fold, directory); }

int main(int argc, char *argv[]) {
    freopen("input.txt", "r", stdin);  // 必要に応じて有効化

    // 1. 引数解析
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
                case 'd':
                    directory = 1;
                    break;
                default:
                    fprintf(stderr, "sort: illegal option %c\n", c);
                    return 1;
            }
        }
    }

    // 2. 入力読み込み
    int nlines = readlines(lineptr, MAXLINES);
    if (nlines < 0) {
        printf("input too big to sort\n");
        return 1;
    }

    // 3. ソートの実行
    // 数値比較の場合は numeric 用の関数、それ以外は汎用の char_cmp を使用
    qsort_kr((void **)lineptr, 0, nlines - 1, (int (*)(void *, void *))(numeric ? numcmp : char_cmp_wrapper), reverse);

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
int ignore_case_cmp(void *s1, void *s2) {
    const char *p1 = (const char *)s1;
    const char *p2 = (const char *)s2;

    for (; tolower((unsigned char)*p1) == tolower((unsigned char)*p2); p1++, p2++) {
        if (*p1 == '\0') {
            return 0;
        }
    }

    return tolower((unsigned char)*p1) - tolower((unsigned char)*p2);
}

int char_cmp(const void *s1, const void *s2, int fold, int dir) {
    const char *p1 = (const char *)s1;
    const char *p2 = (const char *)s2;
    char c1, c2;

    do {
        // --- p1 のスキップ処理 (directory順の場合) ---
        if (dir) {
            while (*p1 != '\0' && !isalnum((unsigned char)*p1) && !isspace((unsigned char)*p1)) {
                p1++;
            }
        }
        // --- p2 のスキップ処理 (directory順の場合) ---
        if (dir) {
            while (*p2 != '\0' && !isalnum((unsigned char)*p2) && !isspace((unsigned char)*p2)) {
                p2++;
            }
        }

        c1 = fold ? tolower((unsigned char)*p1) : *p1;
        c2 = fold ? tolower((unsigned char)*p2) : *p2;

        if (c1 == '\0' && c1 == c2) {
            return 0;
        }

        if (c1 == c2) {
            p1++;
            p2++;
        }
    } while (c1 == c2);

    return (unsigned char)c1 - (unsigned char)c2;
}

/* 引数を const void * に変更 */
int numcmp(void *s1, void *s2) {
    double v1, v2;

    /* 内部で char * にキャストして atof に渡す */
    v1 = atof((const char *)s1);
    v2 = atof((const char *)s2);

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
