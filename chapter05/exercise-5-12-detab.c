#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXTAB 100          // 最大のカスタムタブストップの数
#define DEFAULT_TAB_STOP 8  // デフォルトのタブストップ間隔
#define MAX_COL 1000        // タブストップの最大列

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
    // 1. 固定/生成されたカスタムタブストップを検索
    for (int i = 0; i < num_tab_stops; i++) {
        // タブストップは1から始まる列番号として格納されている
        if (tab_stops[i] > col) {
            // 見つかったカスタムタブストップ - 現在の列 = 必要な空白数
            return tab_stops[i] - col;
        }
    }

    // 2. カスタムタブストップがない、または全て過ぎた場合、デフォルト間隔を使用
    return DEFAULT_TAB_STOP - (col % DEFAULT_TAB_STOP);
}

/**
 * @brief '-m +n' の形式でタブストップを生成します。
 * (entabで使用したものと同じ関数)
 */
void generate_tab_stops(int start_col, int interval) {
    if (interval <= 0) {
        fprintf(stderr, "Error: Tab stop interval must be positive.\n");
        return;
    }

    // start_col 以降、interval ごとにタブストップを追加
    for (int col = start_col; col < MAX_COL && num_tab_stops < MAXTAB; col += interval) {
        if (col > 0) {
            tab_stops[num_tab_stops++] = col;
        }
    }
}

/**
 * @brief コマンドライン引数からタブストップのパターンをパースします。
 * (entabで使用したものとほぼ同じ関数)
 */
void parse_args(int argc, char *argv[]) {
    int start_m = -1;     // -m の値
    int interval_n = -1;  // +n の値

    // 1. まず -m と +n のパターンを探す
    for (int i = 1; i < argc; i++) {
        char *s = argv[i];
        char *endptr;
        long val = strtol(s + 1, &endptr, 10);

        // 文字列全体が数値として消費され、かつ有効な値であるかチェック
        if (*endptr == '\0' && val > 0) {
            if (s[0] == '-' && s[1] != '\0') {
                // -m 形式の引数
                start_m = (int)val;
            } else if (s[0] == '+' && s[1] != '\0') {
                // +n 形式の引数
                interval_n = (int)val;
            }
        }
    }

    // 2. '-m +n' パターンが見つかった場合
    if (start_m != -1 && interval_n != -1) {
        generate_tab_stops(start_m, interval_n);
        return;
    }

    // 3. '-m +n' パターンが見つからなかった場合、リスト形式の引数をチェック
    for (int i = 1; i < argc && num_tab_stops < MAXTAB; i++) {
        char *s = argv[i];
        char *endptr;
        long tab_col = strtol(s, &endptr, 10);

        // -m や +n ではない、純粋な数値引数のみを固定タブストップとして追加
        if (*endptr == '\0' && tab_col > 0 && s[0] != '-' && s[0] != '+') {
            tab_stops[num_tab_stops++] = (int)tab_col;
        }
    }
}

int main(int argc, char *argv[]) {
    int col = 0;  // 現在の列番号 (0から始まる)

    // 1. 引数からタブストップをパース
    parse_args(argc, argv);

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
