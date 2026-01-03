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
 * @brief 現在の列から次のタブストップの列番号を返します。
 */
int next_tab_col(int col) {
    // 1. 固定タブストップを検索
    for (int i = 0; i < num_tab_stops; i++) {
        if (tab_stops[i] > col) {
            return tab_stops[i];
        }
    }

    // 2. 固定タブストップがない、または全て過ぎた場合、デフォルト間隔を使用
    // ここでは、デフォルト間隔を8列目、16列目などとして扱う。
    return ((col / DEFAULT_TAB_STOP) + 1) * DEFAULT_TAB_STOP;
}

/**
 * @brief '-m +n' の形式でタブストップを生成します。
 *
 * @param start_col 開始列 (m)
 * @param interval 間隔 (n)
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
        // パターンでタブストップを生成し、終了
        generate_tab_stops(start_m, interval_n);
        // 生成されたタブストップ配列をソート（念のため）
        // (バブルソートなどの単純なソート関数を実装するか、ここでは引数のパース順序に依存すると仮定)
        // 実際には、generate_tab_stopsは昇順で生成するのでソートは不要。
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

    // (ここではソートを省略しますが、実際の運用では固定タブストップは昇順にソートされるべきです)
}

int main(int argc, char *argv[]) {
    int col = 0;      // 現在の列番号 (0から始まる)
    int nblanks = 0;  // 連続する空白の数

    // 1. 引数からタブストップをパース
    parse_args(argc, argv);

    // 2. 標準入力から文字を読み込み、空白をタブに変換
    int c;
    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            nblanks++;
        } else {
            // 空白の並びが途切れたら処理開始
            if (nblanks > 0) {
                int next_tab, needed_blanks;

                // 連続する空白がある限り、可能な限りタブに置換
                while (nblanks > 0) {
                    // 次のタブストップの列番号を取得
                    next_tab = next_tab_col(col);
                    needed_blanks = next_tab - col;

                    if (nblanks >= needed_blanks) {
                        // タブを出力
                        putchar('\t');
                        col = next_tab;
                        nblanks -= needed_blanks;
                    } else {
                        // タブに置換するのに必要な空白数に満たないため、ループ終了
                        break;
                    }
                }

                // タブに置き換えられなかった残りの空白を出力
                for (int i = 0; i < nblanks; i++) {
                    putchar(' ');
                    col++;
                }

                // 空白カウントをリセット
                nblanks = 0;
            }

            // 現在の文字を処理
            if (c == '\n') {
                putchar(c);
                col = 0;
            } else {
                putchar(c);
                col++;
            }
        }
    }

    // 最終行の終わりに残っている空白を出力 (もしあれば)
    for (int i = 0; i < nblanks; i++) {
        putchar(' ');
    }

    return 0;
}
