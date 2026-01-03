#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);
int gettoken(void);

int tokentype;            // 最後のトークンの型
char token[MAXTOKEN];     // 最後のトークンの文字列
char name[MAXTOKEN];      // 識別子の名前
char datatype[MAXTOKEN];  // データ型（char, intなど）
char out[1000];           // 出力文字列

// C言語の宣言（Declaration）を、言葉で説明した形に変換（パース）するプログラム

// 以下の優先度でパースする。
// 1. 括弧 (): 括弧で囲まれた部分は最優先。
// 2. 後置演算子 () と []: 関数呼び出し () と配列 [] は、ポインタ * よりも優先度が高い。
// 3. 前置演算子 *: ポインタ。

// 例：*p[]
// 1. dcl が呼ばれ、* を見つける。
// 2. さらに dirdcl を呼び出す。
// 3. dirdcl が名前 p を見つける。
// 4. dirdcl が続きの [] を見つけ、「array of」という情報を追加する。
// 5. dcl に戻り、最初に見た * を「pointer to」として追加する。
// 結果：「p is array of pointer to...」

// 例2: int (*p)[] （配列へのポインタ）
/*
1. main: int を読み取り、datatype に保存。その後 dcl() を呼び出す。

2. dcl():
* があるかチェック。まだ見当たらない（「(」 が先に来る）ので、そのまま dirdcl() を呼び出す。

3. dirdcl():
最初が ( なので、カッコ内を解析するために dcl() を再帰呼び出しする。

4. dcl() (再帰1回目):
今度は * を発見！ ns（ポインタの数）をカウントし、さらに dirdcl() を呼ぶ。

5. dirdcl() (再帰2回目):
名前 p を発見！ name 変数に "p" をコピーして戻る。

6. dcl() (再帰1回目に戻る):
先ほどカウントした * に基づき、out に "pointer to" を書き込む。

7. dirdcl() (最初に戻る):
閉じカッコ ) を処理。
その直後に [] を発見！ out に "array [] of" を追記。

8. main に戻る:
最終的な結果「p: array [] of pointer to int」を表示。
*/

// メインループ：宣言文を読み込み、変換する
int main() {
    freopen("input2.txt", "r", stdin);  // 必要に応じて有効化

    while (gettoken() != EOF) {  // 行の最初のトークン（データ型）を取得
        strcpy(datatype, token);
        out[0] = '\0';
        dcl();  // 残りの部分を解析
        if (tokentype != '\n') printf("syntax error\n");
        printf("%s: %s %s\n", name, out, datatype);
    }
    return 0;
}

// dcl: ポインタ（*）を処理する　declarator
void dcl(void) {
    printf("Entering dcl\n");

    int ns;
    for (ns = 0; gettoken() == '*';)  // アスタリスクの数を数える
        ns++;
    dirdcl();
    while (ns-- > 0) strcat(out, " pointer to");

    printf("Exiting dcl\n");
}

// dirdcl: 直接宣言子（名前、(dcl)、[]、()）を処理する direct declarator
void dirdcl(void) {
    printf("Entering dirdcl (token: %s)\n", token);

    int type;

    if (tokentype == '(') {  // ( dcl ) の形
        dcl();
        if (tokentype != ')') printf("error: missing )\n");
    } else if (tokentype == NAME) {  // 変数名
        strcpy(name, token);
    } else {
        printf("error: expected name or (dcl)\n");
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

    printf("Exiting dirdcl\n");
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
