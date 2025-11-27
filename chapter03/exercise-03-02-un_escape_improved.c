#include <stdio.h>

/* un_escape: 文字列 t の \n と \t を改行とタブに変換し、s にコピーする */
void un_escape_improved(char s[], char t[]) {
    int i, j;

    for (i = 0, j = 0; t[i] != '\0'; i++) {
        // 1. バックスラッシュ以外の通常の文字は即座にコピーして次のループへ
        if (t[i] != '\\') {
            s[j++] = t[i];
            continue;  // ここでループの先頭に戻り、ネストを防ぐ
        }

        // 2. バックスラッシュが発見された場合 (t[i] == '\\')

        // 入力インデックスを次の文字へ進める
        i++;

        // 文字列の終端チェック (バックスラッシュが最後の場合に対応)
        if (t[i] == '\0') {
            s[j++] = '\\';
            break;  // 終端なのでループを終了
        }

        // 3. エスケープ文字の変換
        switch (t[i]) {
            case 'n':
                s[j++] = '\n';
                break;
            case 't':
                s[j++] = '\t';
                break;
            default:
                // 未知のエスケープシーケンス（例: \\ や \" など）は2文字ともコピー
                s[j++] = '\\';
                s[j++] = t[i];
                break;
        }
        // ループの終わりに i が自動でインクリメントされる
    }
    s[j] = '\0';  // 終端文字
}

/* テスト用 main 関数 */
#define MAXLINE 1000

int main() {
    char t[] = "Test\\tString\\nwith\\tback\\slash\\\\.";
    char s[MAXLINE];

    printf("元の文字列:\n---\n%s---\n\n", t);

    un_escape_improved(s, t);

    printf("変換後の文字列（改行とタブが反映される）:\n---\n%s---\n", s);

    return 0;
}
