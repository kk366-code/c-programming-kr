#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);
void parmdcl(void);
void dcl_spec(void);
int gettoken(void);
int typespec(void);
int typequal(void);
void errmsg(char *);
int getch(void);
void ungetch(int);

int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

int main() {
    while (gettoken() != EOF) {
        /* datatype を初期化 */
        datatype[0] = '\0';

        /* 1. 型名や修飾子(const, intなど)が続く限り読み込む */
        while (tokentype == NAME && (typespec() || typequal())) {
            if (datatype[0] != '\0') strcat(datatype, " ");
            strcat(datatype, token);
            gettoken();  // 次のトークンへ
        }

        /* 2. 解析の準備 */
        out[0] = '\0';
        name[0] = '\0';

        /* 3. 残りの宣言部分を解析 */
        dcl();

        if (tokentype != '\n') printf("syntax error\n");

        printf("%s: %s %s\n", name, out, datatype);
    }
    return 0;
}

/* dcl: ポインタの処理 */
void dcl(void) {
    int ns;
    for (ns = 0; gettoken() == '*';) ns++;
    dirdcl();
    while (ns-- > 0) strcat(out, " pointer to");
}

/* dirdcl: 名前、カッコ、配列、および関数引数の処理 */
void dirdcl(void) {
    int type;

    if (tokentype == '(') {
        dcl();
        if (tokentype != ')') errmsg("error: missing )");
    } else if (tokentype == NAME) {
        strcpy(name, token);
    } else {
        /* 抽象宣言（名前なし）を許容するため、ここではエラーにしない */
    }

    while ((type = gettoken()) == PARENS || type == BRACKETS || type == '(') {
        if (type == PARENS) {
            strcat(out, " function returning");
        } else if (type == BRACKETS) {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        } else if (type == '(') {
            /* 関数の引数リストの解析開始 */
            strcat(out, " function taking");
            parmdcl();
            strcat(out, " and returning");
        }
    }
}

/* parmdcl: 引数リストを解析する */
void parmdcl(void) {
    do {
        dcl_spec();  // 各引数の型と宣言を解析
    } while (tokentype == ',');

    if (tokentype != ')') errmsg("error: missing ) in parameter list");
}

/* 引数用の解析（dclを簡略化したもの） */
void dcl_spec(void) {
    char temp[MAXTOKEN];
    gettoken();
    strcat(out, " ");
    strcat(out, token);  // 型名

    /* 本来はここで再帰的に dcl() を呼ぶことで
       int f(char *s) のような複雑な引数にも対応できる */
}

/* 型名かどうか判定 */
int typespec(void) {
    static char *types[] = {"char", "int", "void", "double", "float"};
    for (int i = 0; i < 5; i++)
        if (strcmp(token, types[i]) == 0) return 1;
    return 0;
}

/* 型修飾子かどうか判定 */
int typequal(void) {
    static char *quals[] = {"const", "volatile"};
    for (int i = 0; i < 2; i++)
        if (strcmp(token, quals[i]) == 0) return 1;
    return 0;
}

void errmsg(char *msg) { printf("%s\n", msg); }

/* --- gettoken, getch, ungetch は以前のコードと同じ --- */

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
