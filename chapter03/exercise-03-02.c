#include <stdio.h>

/* escape: 文字列 t の改行とタブを \n と \t に変換し、s にコピーする */
void escape(char s[], char t[]) {
    int i, j;

    i = j = 0;
    while (t[i] != '\0') {
        switch (t[i]) {
            case '\n':
                s[j++] = '\\';
                s[j++] = 'n';
                break;
            case '\t':
                s[j++] = '\\';
                s[j++] = 't';
                break;
            default:
                s[j++] = t[i];
                break;
        }
        i++;
    }
    s[j] = '\0';  // 終端文字を忘れずに
}

#define MAXLINE 1000

int main() {
    char s[MAXLINE];
    char t[] = "Hello\tWorld!\nThis is a test.\n\t";

    printf("元の文字列:\n---\n%s---\n\n", t);

    escape(s, t);

    printf("変換後の文字列:\n---\n%s---\n", s);

    return 0;
}
