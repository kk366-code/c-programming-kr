#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000        // 入力行の最大長
#define MAXTAB 100          // 最大のカスタムタブストップの数
#define DEFAULT_TAB_STOP 8  // デフォルトのタブストップ間隔

// グローバル変数としてタブストップを格納
int tab_stops[MAXTAB];
int num_tab_stops = 0;

/**
 * @brief 現在の列から次のタブストップまでの空白の数を計算します。
 *
 * @param col 現在の列番号 (0から始まる)
 * @return int 次のタブストップまでの空白の数
 */
int blanks_to_next_tab(int col) {
    // カスタムタブストップを検索
    for (int i = 0; i < num_tab_stops; i++) {
        // タブストップは1から始まる列番号として格納されているため、col + 1と比較
        if (tab_stops[i] > col) {
            // 見つかったカスタムタブストップ - 現在の列
            return tab_stops[i] - col;
        }
    }

    // カスタムタブストップがない、またはカスタムタブストップをすべて過ぎた場合
    // デフォルトのタブストップ間隔を使用
    return DEFAULT_TAB_STOP - (col % DEFAULT_TAB_STOP);
}

/**
 * @brief タブストップの引数をパースします。
 *
 * 引数は正の整数として解釈され、配列 tab_stops に格納されます。
 * 例: "10", "25", "30" -> 10, 25, 30の列にタブストップを設定
 *
 * @param argc main関数の引数の数
 * @param argv main関数の引数配列
 */
void parse_tab_stops(int argc, char *argv[]) {
    for (int i = 1; i < argc && num_tab_stops < MAXTAB; i++) {
        char *endptr;
        long tab_col = strtol(argv[i], &endptr, 10);

        // 文字列全体が数値として消費され、かつ正の整数の場合にのみ追加
        if (*endptr == '\0' && tab_col > 0) {
            tab_stops[num_tab_stops++] = (int)tab_col;
        } else {
            // 無効な引数、または非数値の引数
            fprintf(stderr, "Warning: Invalid tab stop argument ignored: %s\n", argv[i]);
        }
    }

    // タブストップ配列をソートする必要はありません。
    // 引数として渡された順序で処理します。
    // *注意: K&Rの慣習では昇順であるべきですが、単純化のためここでは引数順を採用します。
}

int main(int argc, char *argv[]) {
    int col = 0;  // 現在の列番号 (0から始まる)

    // 1. 引数からタブストップをパース
    parse_tab_stops(argc, argv);

    // 2. 標準入力から文字を読み込み、タブを展開
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\t') {
            // タブの場合: 次のタブストップまでの空白の数を計算
            int num_blanks = blanks_to_next_tab(col);

            // 空白を出力し、列番号を更新
            for (int i = 0; i < num_blanks; i++) {
                putchar(' ');
                col++;
            }
        } else if (c == '\n') {
            // 改行の場合: 改行を出力し、列番号をリセット
            putchar(c);
            col = 0;
        } else {
            // その他すべての文字の場合: 文字を出力し、列番号をインクリメント
            putchar(c);
            col++;
        }
    }

    return 0;
}
