#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXTAB 100          // 最大のカスタムタブストップの数
#define DEFAULT_TAB_STOP 8  // デフォルトのタブストップ間隔

// グローバル変数としてタブストップを格納
int tab_stops[MAXTAB];
int num_tab_stops = 0;

/**
 * @brief 現在の列から次のタブストップまでの空白の数を計算します。
 *
 * @param col 現在の列番号 (0から始まる)
 * @return int 次のタブストップの列番号 (1から始まる)
 */
int next_tab_col(int col) {
    // カスタムタブストップを検索
    for (int i = 0; i < num_tab_stops; i++) {
        // タブストップは1から始まる列番号として格納されている
        if (tab_stops[i] > col) {
            // 見つかったカスタムタブストップの列番号を返す
            return tab_stops[i];
        }
    }

    // カスタムタブストップがない、またはカスタムタブストップをすべて過ぎた場合
    // デフォルトのタブストップ間隔を使用
    // col = 0 (1列目) -> 8, col = 8 (9列目) -> 16
    return ((col / DEFAULT_TAB_STOP) + 1) * DEFAULT_TAB_STOP;
}

/**
 * @brief タブストップの引数をパースします。
 *
 * (detabで使用したものと同じ関数)
 */
void parse_tab_stops(int argc, char *argv[]) {
    for (int i = 1; i < argc && num_tab_stops < MAXTAB; i++) {
        char *endptr;
        long tab_col = strtol(argv[i], &endptr, 10);

        if (*endptr == '\0' && tab_col > 0) {
            tab_stops[num_tab_stops++] = (int)tab_col;
        } else {
            fprintf(stderr, "Warning: Invalid tab stop argument ignored: %s\n", argv[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    int col = 0;      // 現在の列番号 (0から始まる)
    int nblanks = 0;  // 連続する空白の数

    // 1. 引数からタブストップをパース
    parse_tab_stops(argc, argv);

    // 2. 標準入力から文字を読み込み、空白をタブに変換
    int c;
    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            // 空白が見つかったらカウントアップ
            nblanks++;
        } else {
            // 空白の並びが途切れたら処理開始
            if (nblanks > 0) {
                // 連続する空白がある場合、タブと空白に置き換える処理を行う

                // 次のタブストップの列番号を取得 (1から始まる)
                int next_tab = next_tab_col(col);

                // 次のタブストップの位置まで到達するのに必要な空白の数
                int needed_blanks = next_tab - col;

                // タブを使うことができるか？
                // 必要な空白の数が現在の連続空白数 nblanks よりも少なくない、
                // かつ、タブ1つでその位置をカバーできる場合にタブを使用
                while (nblanks >= needed_blanks) {
                    if (nblanks > 0) {
                        // タブを出力
                        putchar('\t');
                        // 列番号をタブストップの位置に更新
                        col = next_tab;
                        // 消費した空白を引く
                        nblanks -= needed_blanks;

                        // 次のタブストップの列番号を再計算
                        next_tab = next_tab_col(col);
                        needed_blanks = next_tab - col;
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
                col = 0;  // 改行で列をリセット
            } else {
                putchar(c);
                col++;  // それ以外の文字は列をインクリメント
            }
        }
    }

    return 0;
}
