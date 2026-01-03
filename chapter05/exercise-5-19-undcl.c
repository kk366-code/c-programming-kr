#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

int gettoken(void);

int tokentype;            // 最後のトークンの型
char token[MAXTOKEN];     // 最後のトークンの文字列
char datatype[MAXTOKEN];  // データ型（char, intなど）
char out[1000];           // 出力文字列

// メインループ：宣言文を読み込み、変換する
int main() {
    int type;
    char temp[MAXTOKEN];

    while (gettoken() != EOF) {
        strcpy(out, token);

        while ((type = gettoken()) != '\n') {
            /* 1. 配列と関数（後ろに付けるだけのもの）を先に処理 */
            if (type == PARENS || type == BRACKETS) {
                strcat(out, token);
                continue;
            }

            /* 2. ポインタの処理 */
            if (type == '*') {
                int next = gettoken();

                if (next == PARENS || next == BRACKETS) {
                    sprintf(temp, "(*%s)%s", out, token);
                } else {
                    sprintf(temp, "*%s", out);
                    // 次のトークンを使い切ってしまったので、戻す必要がある
                    // (ここでは簡易的に、NAMEなら先頭に結合して処理)
                    if (next == NAME) {
                        char t2[MAXTOKEN];
                        sprintf(t2, "%s %s", token, temp);
                        strcpy(temp, t2);
                    }
                }
                strcpy(out, temp);
                continue;
            }

            /* 3. 型名の処理 */
            if (type == NAME) {
                sprintf(temp, "%s %s", token, out);
                strcpy(out, temp);
            }
        }
        printf("%s\n", out);
    }
    return 0;
}

// gettoken: 次のトークンをスキップして読み込む
int gettoken(void) {
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t');  // 空白飛ばし

    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    } else if (c == '[') {
        for (*p++ = c; (*p++ = getch()) != ']';);
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch());) *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else {
        return tokentype = c;
    }
}

// 入力バッファ用の関数
#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;

int getch(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }

void ungetch(int c) {
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
