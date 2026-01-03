#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXTOKEN 100
enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);
int gettoken(void);
void errmsg(char *);  // エラー表示と復帰用

int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];
int prevtoken = 0;  // トークンの再利用フラグ（オプション）

int main() {
    while (gettoken() != EOF) {
        strcpy(datatype, token);
        out[0] = '\0';
        dcl();
        if (tokentype != '\n') {
            errmsg("syntax error");
        } else {
            printf("%s: %s %s\n", name, out, datatype);
        }
    }
    return 0;
}

// エラーを表示し、次の行まで入力を読み飛ばす
void errmsg(char *msg) {
    printf("error: %s\n", msg);
    if (tokentype != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // 改行までスキップ
        tokentype = '\n';
    }
}

void dcl(void) {
    int ns;
    for (ns = 0; gettoken() == '*';) ns++;
    dirdcl();
    while (ns-- > 0) strcat(out, " pointer to");
}

void dirdcl(void) {
    int type;

    if (tokentype == '(') {
        dcl();
        if (tokentype != ')') {
            errmsg("missing )");
            return;  // 呼び出し元へ戻る
        }
    } else if (tokentype == NAME) {
        strcpy(name, token);
    } else {
        errmsg("expected name or (dcl)");
        return;
    }

    while ((type = gettoken()) == PARENS || type == BRACKETS) {
        if (type == PARENS)
            strcat(out, " function returning");
        else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
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

            // 代入式の評価結果であるPARENSが返る
            // 2行に分けて書くと以下のようになる
            // tokentype = PARENS;
            // return PARENS;
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
