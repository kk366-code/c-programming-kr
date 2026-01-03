#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

int getline_kr(char *line, int max);

int main(int argc, char *argv[]) {
    char line[MAXLINE];
    long lineno = 0;
    int except = 0;
    int number = 0;
    int found = 0;

    // コマンドライン引数の解析: -x および -n オプションを処理
    // ここでは引数の内容を順番に見ていき、ハイフンで始まるオプションである限りループを続ける。
    // --argc > 0: 引数の数をデクリメントして残りの引数をチェック。
    //.            イコールを含まないのは、argv[0]（実行ファイル名）を除いた引数を処理するため。
    // *++argv: (++argv)でポインタargvをインクリメントし、次の引数文字列を指し示す。
    //          (*argv)で引数文字列を取得。
    // [0] == '-': 取得した引数文字列の最初の文字がハイフンであるかをチェック。オプション引数であるかを判定。
    while (--argc > 0 && (*++argv)[0] == '-') {
        int c;
        // while ((c = *++argv[0]) != '\0') {
        while ((c = *++argv[0])) {
            // ポインタargv[0]をインクリメントしてハイフンの次の文字を見ていく。
            // * でポインタの値（オプション文字）を取得し、cに代入。
            // 代入された文字がNULL文字でない限りループする。
            // そのため、-xnのようなオプションをまずx、次にnという風に順番に処理できる。
            switch (c) {
                case 'x':
                    except = 1;
                    break;
                case 'n':
                    number = 1;
                    break;
                default:
                    printf("find: illegal option %c\n", c);
                    argc = 0;
                    found = -1;
                    break;
            }
        }
    }

    if (argc != 1) {
        printf("Usage: find -x -n pattern\n");
    } else {
        while (getline_kr(line, MAXLINE) > 0) {
            lineno++;

            // 1. - x オプションが指定されなかった場合（通常モード）
            // except の値: 0
            // 条件式: (strstr(...) != NULL) != 0
            // 評価: この式は、左辺の (strstr(...) != NULL) が 1(真)、
            //       つまり「パターンが含まれている」場合にのみ全体として真となり、printf が実行される。
            // 結果: 検索パターンを含む行のみが出力される。

            // 2. -x オプションが指定された場合（除外モード）
            // except の値: 1
            // 条件式: (strstr(...) != NULL) != 1
            // 評価: この式は、左辺の (strstr(...) != NULL) が 0(偽)、
            //       つまり「パターンが含まれていない」場合にのみ全体として真となり、printf が実行される。
            // 結果: 検索パターンを含まない行のみが出力される。

            if ((strstr(line, *argv) != NULL) != except) {
                if (number) {
                    printf("%ld:", lineno);
                }
                printf("%s", line);
                found++;
            }
        }
    }

    return found;
}

/* getline_kr: 行を読み込み、sに保存する。長さ (改行文字も含む) を返す */
// 標準Cライブラリのgetlineと衝突しないように、ここでは getline_kr とする。
int getline_kr(char *s, int lim) {
    int c;
    int i;

    // lim-1 (NULL終端のため) に達するか、EOF、または改行文字 '¥n' まで読み込む
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        s[i] = c;
    }

    // 改行文字 '¥n' が読み込まれた場合、それも文字列に含める
    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    // 文字列の終端としてNULL文字を設定する
    s[i] = '\0';

    // 読み込んだ文字数を返す (EOFの場合は0)
    return i;
}
