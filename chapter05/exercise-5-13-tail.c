#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_N 10  // デフォルトの表示行数
#define MAXLINE 1000  // 1行の最大長

/* getline: 行を読み込み、長さを返す (K&R 第1章より) */
int getline_kr(char *s, int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) s[i] = c;
    if (c == '\n') {
        s[i] = '\n';
        ++i;
    }
    s[i] = '\0';
    return i;
}

int main(int argc, char *argv[]) {
    int n = DEFAULT_N;
    char line[MAXLINE];
    char **line_ptr;  // 行を保持するポインタ配列（動的に確保）
    int nlines = 0;   // 読み込んだ合計行数
    int last = 0;     // 書き込み位置（環状バッファ用）

    // 1. 引数のパース (例: tail -20)
    // "-20"という文字列から20という数値を取り出して変数nにセット
    if (argc > 1 && argv[1][0] == '-') {
        // &argv[1][1]は先頭のハイフンをスキップした2番目の文字のアドレスを意味する。
        n = atoi(&argv[1][1]);
        // 0以下の場合にプログラムが動かなくなるのを防ぐため、デフォルト値をセット
        if (n <= 0) n = DEFAULT_N;
    }

    // 最新のn行を指すためのポインタ配列を確保
    line_ptr = (char **)malloc(n * sizeof(char *));
    if (line_ptr == NULL) {
        fprintf(stderr, "error: memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < n; i++) line_ptr[i] = NULL;

    // 2. 入力を読み込み、環状バッファ（Circular Buffer）に格納
    while (getline_kr(line, MAXLINE) > 0) {
        // すでに格納されているメモリがあれば解放
        if (line_ptr[last] != NULL) free(line_ptr[last]);

        // 新しい行のためにメモリを確保してコピー
        line_ptr[last] = (char *)malloc(strlen(line) + 1);
        if (line_ptr[last] == NULL) {
            fprintf(stderr, "error: memory allocation failed\n");
            return 1;
        }
        strcpy(line_ptr[last], line);

        last = (last + 1) % n;  // インデックスを 0, 1, ..., n-1 と回す
        nlines++;
    }

    // 3. 結果の出力
    // 実際に表示すべき行数を決定（入力がn行未満の場合を考慮）
    int count = (nlines < n) ? nlines : n;
    // 出力開始位置の計算
    int first = (nlines < n) ? 0 : last;

    for (int i = 0; i < count; i++) {
        printf("%s", line_ptr[(first + i) % n]);
        free(line_ptr[(first + i) % n]);  // 出力後にメモリ解放
    }

    free(line_ptr);

    return 0;
}
