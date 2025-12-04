#include <ctype.h>
#include <stdio.h>

// getchとungetchのためのグローバルバッファ
#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;

// getch: プッシュバックバッファから文字を取得、なければ標準入力から
int getch(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }

// ungetch: 文字をプッシュバックバッファに戻す
void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        printf("ungetch: too many characters\n");
    } else {
        buf[bufp++] = c;
    }
}

// getint: 入力から次の整数を取り出して *pn に入れる
int getint(int* pn) {
    int c;
    int sign;

    // 空白を飛ばす
    while (isspace(c = getch())) {
        //
    }

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);
        return 0;  // 整数ではない
    }

    sign = (c == '-') ? -1 : 1;

    if (c == '+' || c == '-') {
        c = getch();  // 符号の後の文字を読み込む
        // 符号の直後に数字がない場合を考慮していないため、この時点で非数字であればエラー処理を追加するとより堅牢になりますが、
        // ここではK&Rのオリジナルの流れに従います。
    }

    // 読み取りと変換
    for (*pn = 0; isdigit(c); c = getch()) {
        *pn = 10 * *pn + (c - '0');
    }

    *pn *= sign;

    if (c != EOF) {
        ungetch(c);  // 数字の後に続いた文字を戻す
    }

    return c;  // 最後に読み取った文字
}

int main(void) {
    int number;  // 読み取った整数を格納する変数
    int result;  // getintの返り値

    printf("整数を入力してください (例: 123 456):\n");

    // 最初の整数を読み取る
    result = getint(&number);

    if (result > 0) {
        printf("1番目に読み取った整数: %d\n", number);
        printf("getintの返り値 (次の文字): %c (ASCII: %d)\n", result, result);
    } else if (result == EOF) {
        printf("入力の終わりに達しました (EOF)。\n");
    } else {  // result == 0 の場合
        printf("最初の入力は有効な整数ではありませんでした。\n");
        printf("読み取ろうとした最初の文字が入力ストリームに戻されました。\n");
    }

    printf("\n--- 2回目の読み取りを試みます ---\n");
    // 2番目の整数（または残りの入力）を読み取る
    // getint が実行されるたびに、変数 number の中身が変わる
    // number の中身は、getint 関数が標準入力から新しく読み取った整数値に置き換えられる
    result = getint(&number);

    if (result > 0) {
        printf("2番目に読み取った整数: %d\n", number);
        printf("getintの返り値 (次の文字): %c (ASCII: %d)\n", result, result);
    } else if (result == EOF) {
        printf("入力の終わりに達しました (EOF)。\n");
    } else {  // result == 0 の場合
        printf("2回目の入力は有効な整数ではありませんでした。\n");
    }

    return 0;
}
