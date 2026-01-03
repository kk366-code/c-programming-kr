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
        strcpy(out, token);  // 最初のトークンは名前（識別子）
        while ((type = gettoken()) != '\n') {
            if (type == PARENS || type == BRACKETS) {
                strcat(out, token);
            } else if (type == '*') {
                // * が来たら、現在の out を括弧で囲んで前に * をつける
                sprintf(temp, "(*%s)", out);
                strcpy(out, temp);
            } else if (type == NAME) {
                // 型名（int, charなど）が最後に来た場合
                sprintf(temp, "%s %s", token, out);
                strcpy(out, temp);
            } else {
                printf("invalid input at %s\n", token);
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
