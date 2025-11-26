#include <stdio.h>

/* un_escape: 文字列 t の \n と \t を改行とタブに変換し、s にコピーする */
void un_escape(char s[], char t[]) {
    int i, j;

    i = j = 0;
    while (t[i] != '\0') {
        // バックスラッシュが見つかった場合
        if (t[i] == '\\') {
            i++;  // 次の文字へ進む

            // 次の文字が '\0' でないことを確認し、変換処理を行う
            if (t[i] != '\0') {
                switch (t[i]) {
                    case 'n':
                        s[j++] = '\n';  // 改行文字に変換
                        break;
                    case 't':
                        s[j++] = '\t';  // タブ文字に変換
                        break;
                    default:
                        // それ以外のエスケープシーケンス（例: \\, \"など）はそのままコピー
                        s[j++] = '\\';
                        s[j++] = t[i];
                        break;
                }
            } else {
                // 文字列の最後にバックスラッシュがあった場合（\0 が来るため）
                s[j++] = '\\';
            }
        } else {
            // バックスラッシュ以外の通常の文字はそのままコピー
            s[j++] = t[i];
        }
        i++;
    }
    s[j] = '\0';  // 終端文字を忘れずに
}

/* テスト用 main 関数 */
#define MAXLINE 1000

int main() {
    // escape関数で生成されたような文字列をシミュレート
    char t[] = "Hello\\tWorld!\\nThis is a test.\\n\\t";
    char s[MAXLINE];

    printf("元の文字列:\n---\n%s---\n\n", t);

    un_escape(s, t);

    printf("変換後の文字列（改行とタブが反映される）:\n---\n%s---\n", s);

    return 0;
}
