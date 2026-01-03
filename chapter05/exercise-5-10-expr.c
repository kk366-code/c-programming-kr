#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>  // for strtod and exit

// スタック操作（簡略版）
#define MAXVAL 100
int sp = 0;          // stack pointer
double val[MAXVAL];  // stack

void push(double f) {
    // 処理できない状態（スタックフル）の場合、エラー処理
    if (sp >= MAXVAL) {
        printf("error: stack full, can't push %g\n", f);
        return;
    }

    val[sp++] = f;
}

double pop(void) {
    // 処理できない状態（スタックエンプティ）の場合、エラー処理
    if (sp <= 0) {
        printf("error: stack empty\n");
        return 0.0;
    }

    return val[--sp];
}
// --- スタック操作終わり ---

int main(int argc, char *argv[]) {
    int i;
    double op2;  // 2番目のオペランド

    for (i = 1; i < argc; i++) {
        char *s = argv[i];

        // 1. 最初に単一文字の演算子かどうかをチェックする

        if (s[1] == '\0') {
            switch (s[0]) {
                case '+':
                    push(pop() + pop());
                    continue;  // 演算子として処理完了
                case '*':
                    push(pop() * pop());
                    continue;  // 演算子として処理完了
                case '-':
                    op2 = pop();
                    push(pop() - op2);
                    continue;
                case '/':
                    op2 = pop();
                    if (op2 != 0.0)
                        push(pop() / op2);
                    else
                        printf("error: zero divisor\n");
                    continue;
            }
        }

        // 2. 演算子でなければ、すべて数値として解釈する

        char *endptr;
        double number = strtod(s, &endptr);

        // strtod を使って、文字列全体が数値として消費されたかを確認
        // *endptr が '\0' でない場合、それは数値ではない部分が残っていることを示す
        if (*endptr == '\0') {
            push(number);
        } else {
            // ここに到達した場合、それは単一文字の演算子でも数値でもない
            printf("error: unknown token %s\n", s);
        }
    }

    // 最終結果を出力（forループの外に移動）
    if (sp == 1) {  // スタックポインタが1: すべての計算が正しく行われ、スタックの最上位に最終結果がただ1つ残っている。
        printf("Result: %.8g\n", pop());
    } else if (sp > 1) {  // スタックポインタが2以上: 数値（オペランド）が多すぎる。
                          // すべての数値をスタックに積んだ後、それらを結合する演算子が足りなかった。
        printf("error: too many operands left on stack (sp=%d)\n", sp);
    } else {  // sp == 0
              // スタックポインタが0: 演算子が多すぎる（例: 5 +）。
              // 最後の演算子がスタックから2つポップしようとしたが、値が1つしかなかった、あるいはスタックが空だった。
        printf("error: no result calculated (stack empty)\n");
    }

    return 0;
}
