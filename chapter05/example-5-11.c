#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000     // 処理できるテキストの最大行
char *lineptr[MAXLINES];  // テキスト行へのポインタ

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

// 第4引数 int (*comp)(void *, void *) が「比較という仕事を請け負う関数のアドレス（ポインタ）」
// - 文字列として比べたいなら strcmp のアドレスを渡す。
// - 数値として比べたいなら numcmp のアドレスを渡す。
// 引数が char * ではなく void * の配列になっているため、文字列でも数値でもソートできるようになっている。
void qsort_kr(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int numcmp(const char *, const char *);

void swap(void *v[], int i, int j);

// -n オプションを渡して実行すると数値順、渡さないと辞書順にソートする
// 各行が 1 2 100 10 などから始まるテキストファイルを渡して動きを見ていく
int main(int argc, char *argv[]) {
    // 👇 デバッグ中だけこの一行を追加する（ビルド時に input.txt を読み込むようになる）
    freopen("input.txt", "r", stdin);

    int nlines;       // 読み込まれた入力行の数
    int numeric = 0;  // 0なら辞書順ソート、1なら数値順ソートになる

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        numeric = 1;
    }

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        // ユーザーが-nオプションを指定したかによって、qsort_krに渡す関数のアドレスを切り替えている。
        // ステップ実行で第4引数compに関数のアドレスが入ることを確認する。
        qsort_kr((void **)lineptr, 0, nlines - 1, (int (*)(void *, void *))(numeric ? numcmp : strcmp));
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("input too big to sort\n");
    }
}

#define MAXLEN 1000 /* 1行の最大長 */
int my_getline(char *, int);

int readlines(char *lineptr[], int maxlines) {
    int len, nlines;
    char *p, line[MAXLEN];

    nlines = 0;
    while ((len = my_getline(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || (p = malloc(len)) == NULL) {
            return -1;
        } else {
            line[len - 1] = '\0'; /* 改行文字を消す */
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

int my_getline(char s[], int lim) {
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) s[i] = c;
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

void writelines(char *lineptr[], int nlines) {
    for (int i = 0; i < nlines; i++) {
        printf("%s\n", lineptr[i]);
    }
}

// ソートと比較は独立しているので、ソートアルゴリズムと比較関数を分離することができる

// (*comp): compはポインタである
// int (...) (...): そのポインタが指す先は関数である
// (void *, void *): その関数は2つの汎用ポインタを引数として受け取る
// int: その関数は結果としてint型の整数を返す

// *comp を()で囲まないとどうなるか？
//  - int (*comp)(void *, void *): 関数を指すポインタ変数
//  - int *comp(void *, void *): intへのポインタ（int *）を返す関数

// *（間接参照）の役割
// 通常のポインタ変数で *p と書くと「メモリから値を読み出す」という意味になる(Load)が、
// 関数ポインタで *comp と書くと、「その番地へジャンプして、そこに書いてある処理を開始せよ」という意味になる(Jump/Call)
void qsort_kr(void *v[], int left, int right, int (*comp)(void *, void *)) {
    int i;
    int last;
    void swap(void *v[], int, int);

    if (left >= right) return;  // 要素が2つより少なければ何もしない

    swap(v, left, (left + right) / 2);

    last = left;

    for (i = left + 1; i <= right; i++) {
        // comp という番地に格納されている関数（strcmp か numcmp のどちらか）をここで呼び出して、
        // その結果が 0 より小さいか判定
        if ((*comp)(v[i], v[left]) < 0) {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    qsort_kr(v, left, last - 1, comp);
    qsort_kr(v, last + 1, right, comp);
}

int numcmp(const char *s1, const char *s2) {
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2) {
        return -1;
    } else if (v1 > v2) {
        return 1;
    } else {
        return 0;
    }
}

void swap(void *v[], int i, int j) {
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
