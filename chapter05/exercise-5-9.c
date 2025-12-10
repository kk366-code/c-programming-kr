/* C言語の配列とポインタの等価性（a[i] は *(a+i) と同じであること）を理解する */

#include <stdio.h>

/* daytab[i][j]: i=0なら平年、i=1ならうるう年。jは月 (1月〜12月) の日数 */
static char daytab[2][13] = {
    // 0行目: 平年
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    // 1行目: うるう年
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

/* 月と日から年内の通算日を求める (ポインタ版) */
int day_of_year(int year, int month, int day) {
    // 簡易バリデーション
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return -1;  // エラー
    }

    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    // daytab[leap][i] を *(daytab[leap] + i) または *(*(daytab + leap) + i) に置き換え
    for (int i = 1; i < month; i++) {
        // daytab[leap] は、選択された行（平年またはうるう年）の先頭アドレスを示す
        day += *(daytab[leap] + i);
        // または daytab[leap] は char * なので、 day += *(daytab[leap] + i);
    }

    return day;
}

/* 年の通算日から月と日を求める (ポインタ版) */
void month_day(int year, int yearday, int *pmonth, int *pday) {
    const char *p;  // 選択された行 (daytab[leap]) の先頭ポインタ

    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    // ポインタ p に選択された行の先頭アドレスを設定 (daytab[leap] と同じ)
    // daytab は char[2][13] なので、 daytab[leap] は char[13] に退行し、その先頭ポインタ (char *) となる
    p = daytab[leap];

    // p[0] (daytab[leap][0]) から p[12] まで
    int i = 1;
    // yearday > daytab[leap][i] を yearday > *(p + i) に置き換え
    // yearday > p[i] と書いてもポインタ演算になるが、ここでは *p を明示的に使う
    while (i <= 12 && yearday > *(p + i)) {
        // yearday -= daytab[leap][i] を yearday -= *(p + i) に置き換え
        yearday -= *(p + i);
        i++;
    }

    // yearday が不正な値の場合（例: 367日目）、iは13になる。
    if (i > 12) {
        // 不正な yearday の処理
        *pmonth = -1;
        *pday = -1;
        return;
    }

    *pmonth = i;
    *pday = yearday;
}

int main(void) {
    int year = 2024;   // うるう年
    int yearday = 60;  // 2月29日
    int month;
    int day;
    int result_month;
    int result_day;

    // day_of_year のテスト
    int dy = day_of_year(year, 2, 29);
    printf("%d/%d/%d は 年内通算日 %d 日目です。\n", year, 2, 29, dy);  // 結果: 60

    // month_day のテスト (正常系)
    month_day(year, yearday, &result_month, &result_day);
    printf("%d年の通算日 %d 日目は %d月%d日 です。\n", year, yearday, result_month, result_day);  // 結果: 2月29日

    // month_day のテスト (エラー系)
    month_day(2025, 366, &result_month, &result_day);  // 平年(2025)に366日目は不正

    return 0;
}
