#include <stdio.h>

int main(int argc, char *argv[]) {
    // デバッグ実行が成功すれば、必ず出力される
    printf("--- Start Debugging ---\n");
    printf("hoge\n");

    // 引数が正しく渡っているか確認
    if (argc > 1) {
        printf("Received arguments:\n");
        for (int i = 1; i < argc; i++) {
            printf("  Argument %d: %s\n", i, argv[i]);
        }
    }

    // ブレークポイントを設定して一時停止させるためのコード
    int debug_check = 1;  // ここにブレークポイントを設定

    printf("--- End Debugging ---\n");
    return 0;
}
