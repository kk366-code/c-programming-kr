#include <stdio.h>

/* C言語では、文字列リテラルや文字の配列は、その**先頭アドレス（ポインタ）**によって識別される。 */
/* この関数は、月名が格納されているメモリ上の場所（アドレス）を返すことで、その文字列全体を呼び出し元に伝える。 */
char *month_name(int n) {
    /* static を使うことで、この文字列配列がプログラム実行中ずっと存在することを保証する。
       関数終了後に文字列へのポインタを安全に返すために必須。
       配列nameの大きさは指定されていないから、コンパイラが配列の要素を数えて大きさを決定する。
    */
    static char *name[] = {"Illegal month", "January", "February",  "March",   "April",    "May",     "June",
                           "July",          "August",  "September", "October", "November", "December"};

    /* 月の番号が 1 から 12 の範囲外であれば name[0] (エラーメッセージ) を返す。
       範囲内であれば name[n] (対応する月名) を返す。
    */
    return (n < 1 || n > 12) ? name[0] : name[n];
}

int main(void) {
    int valid_month = 3;
    int invalid_month = 15;

    // 正常な場合 (3月)
    printf("Month %d: %s\n", valid_month, month_name(valid_month));

    // 正常な場合 (12月)
    printf("Month 12: %s\n", month_name(12));

    // 不正な入力の場合 (15)
    printf("Month %d: %s\n", invalid_month, month_name(invalid_month));

    // 不正な入力の場合 (0)
    printf("Month 0: %s\n", month_name(0));

    return 0;
}
