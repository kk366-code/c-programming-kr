#include <ctype.h>
#include <stdio.h>
#include <string.h>

void expand(char s1[], char s2[]);

#define MAX_LEN 1000

int main() {
    char s2[MAX_LEN];

    // テストケースの配列
    char* tests[] = {"a-c",        // 小文字の展開
                     "A-D",        // 大文字の展開
                     "0-3",        // 数字の展開
                     "a-z0-9",     // 複数の展開
                     "-a-z",       // 先頭のハイフン
                     "a-z-",       // 末尾のハイフン
                     "a-z0-9A-C",  // 混在
                     "a-a",        // 1文字の展開
                     "a-B-c",      // 不正な展開（文字種が異なる）
                     NULL};

    printf("--- K&R 演習 3-3 expand 関数のテスト ---\n");

    for (int k = 0; tests[k] != NULL; k++) {
        expand(tests[k], s2);
        printf("入力 (s1): %-12s => 出力 (s2): %s\n", tests[k], s2);
    }

    return 0;
}

/* 演習 3-3: s1の省略表現（例: a-z）をs2に展開する */
void expand(char s1[], char s2[]) {
    int i, j, k;
    char start, end;

    i = j = 0;  // i: s1のインデックス, j: s2のインデックス

    // 1. s1の先頭のハイフンを処理
    if (s1[i] == '-') {
        s2[j++] = s1[i++];
    }

    while (s1[i] != '\0') {
        // 2. 現在の文字を保持
        start = s1[i];

        // 3. 次がハイフンで、その次が終端文字（\0）でない場合、省略表現の可能性がある
        if (s1[i + 1] == '-' && s1[i + 2] != '\0') {
            end = s1[i + 2];

            // 4. 文字種のチェック（小文字、大文字、数字のいずれかであること）
            //    かつ、開始文字 <= 終了文字 であること
            if ((islower(start) && islower(end) && start <= end) || (isupper(start) && isupper(end) && start <= end) ||
                (isdigit(start) && isdigit(end) && start <= end)) {
                // 省略表現をs2に展開
                for (k = start; k <= end; k++) {
                    s2[j++] = k;
                }

                // s1のインデックスをスキップ（ハイフンと終了文字を飛ばす）
                i += 3;

            } else {
                // 5. 不正な省略表現（例: a-C, 9-0 など）
                s2[j++] = s1[i++];  // 開始文字をコピー
                s2[j++] = s1[i++];  // ハイフンをコピー
                // s1[i]（end文字）は次のループで処理される
            }
        } else {
            // 6. 通常の文字、または末尾のハイフン
            s2[j++] = s1[i++];
        }
    }

    // 7. s2に終端文字（ナル文字）を追加
    s2[j] = '\0';
}
