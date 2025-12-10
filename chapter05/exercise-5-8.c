#include <stdio.h>

/*
C言語におけるchar型の本来の役割と二次的な役割
 - 主要な役割: 文字（Character）を格納する。ASCIIやUTF-8などの文字コードの値を保持する。
 - 二次的な役割: 多くの環境で1バイト（8ビット）の最も小さな整数型として扱われる。
  - 8ビットで最大 2^8 - 1 = 255 まで格納可能。
  - 扱う数値が0から255までの範囲に収まるならint型よりもchar型（またはunsigned char型）を使った方がメモリ効率が良い。
*/

/* daytab[i][j]: i=0なら平年、i=1ならうるう年。jは月 (1月〜12月) の日数 */
static char daytab[2][13] = {
    // 0行目: 平年 (うるう年ではない) の場合
    // インデックスを 1から12（1月〜12月） で使うため、[0]にダミーとして0を入れている
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},

    // 1行目: うるう年の場合 (2月が29日)
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

/* 月と日から年内の通算日を求める */
int day_of_year(int year, int month, int day) {
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        printf("Error: Invalid month or day input.\n");
        return -1;
    }

    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    for (int i = 1; i < month; i++) {
        day += daytab[leap][i];
    }

    return day;
}

/* 年の通算日から月と日を求める */
void month_day(int year, int yearday, int *pmonth, int *pday) {
    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    // 1. その年の総日数を計算
    // daytab[leap][1]からdaytab[leap][12]までの合計
    int max_days = leap ? 366 : 365;

    // 2. 入力値 yearday のバリデーション (1 <= yearday <= max_days)
    if (yearday < 1 || yearday > max_days) {
        // エラー処理：月と日を不正な値に設定して終了
        *pmonth = -1;  // -1をエラーとして返す
        *pday = -1;
        printf("Error: Invalid yearday (%d) for year %d.\n", yearday, year);
        return;
    }

    // yearday から月を求める
    int i;
    for (i = 1; yearday > daytab[leap][i]; i++) {
        yearday -= daytab[leap][i];
    }

    // iは目的の日付が含まれる月を示す
    *pmonth = i;

    // yearday にはその月の残り日数（その月の何日目か）が入っている
    *pday = yearday;
}

int main(void) {
    int year = 2024;  // うるう年
    int month;
    int day;
    int yearday;
    int result_month;
    int result_day;

    printf("--- Function day_of_year (日付 -> 通算日) ---\n");

    // (1) うるう年の日付 (2024年2月29日)
    month = 2;
    day = 29;
    yearday = day_of_year(year, month, day);
    printf("%d/%d/%d は 年内通算日 %d 日目です。\n", year, month, day, yearday);

    // (2) 平年の日付 (2025年3月1日)
    month = 3;
    day = 1;
    yearday = day_of_year(2025, month, day);
    printf("%d/%d/%d は 年内通算日 %d 日目です。\n", 2025, month, day, yearday);

    // (3) 年末の日付 (2024年12月31日)
    yearday = day_of_year(2024, 12, 31);
    printf("%d/%d/%d は 年内通算日 %d 日目です。\n", 2024, 12, 31, yearday);

    printf("\n--- Function month_day (通算日 -> 日付) ---\n");

    // (4) 2024年の60日目 (うるう年なので2月29日)
    yearday = 60;
    month_day(year, yearday, &result_month, &result_day);
    printf("%d年の通算日 %d 日目は %d月%d日 です。\n", year, yearday, result_month, result_day);

    // (5) 2025年の60日目 (平年なので3月1日)
    yearday = 60;
    month_day(2025, yearday, &result_month, &result_day);
    printf("%d年の通算日 %d 日目は %d月%d日 です。\n", 2025, yearday, result_month, result_day);

    // (6) 不正な入力 (2024年の367日目) - エラーチェックの確認
    yearday = 367;
    month_day(year, yearday, &result_month, &result_day);

    return 0;
}
