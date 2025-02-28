/*There is no error checking in day_of_year or month_day. Remedy this defect.*/
#include <stdio.h>
#define INVALID -1 
static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  
};

int day_of_year(int year, int month, int day) {
    if (month < 1 || month > 12) {
        printf("Error: Invalid month (%d). Must be 1-12.\n", month);
        return INVALID;
    }

    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    if (day < 1 || day > daytab[leap][month]) {
        printf("Error: Invalid day (%d) for month %d.\n", day, month);
        return INVALID;
    }

    for (int i = 1; i < month; i++)
        day += daytab[leap][i];

    return day;
}
int month_day(int year, int yearday, int *pmonth, int *pday) {
    if (!pmonth || !pday) { 
        printf("Error: NULL pointer passed.\n");
        return INVALID;
    }

    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    int max_days = leap ? 366 : 365;

    if (yearday < 1 || yearday > max_days) {
        printf("Error: Invalid day of year (%d). Must be 1-%d.\n", yearday, max_days);
        return INVALID;
    }

    int i;
    for (i = 1; yearday > daytab[leap][i]; i++)
        yearday -= daytab[leap][i];

    *pmonth = i;
    *pday = yearday;
    return 0;
}
int main() {
    int year = 2024;
    int doy = day_of_year(year, 2, 29); 
    if (doy != INVALID)
        printf("Feb 29, 2024 is the %dth day of the year.\n", doy);

    doy = day_of_year(year, 13, 5); 
    doy = day_of_year(year, 2, 30); 
    int month, day;
    if (month_day(year, 60, &month, &day) != INVALID)
        printf("Day 60 of 2024 is %d/%d.\n", month, day);

    if (month_day(year, 400, &month, &day) == INVALID) 
        printf("400 is an invalid day of the year.\n");

    return 0;
}
/*Feb 29, 2024 is the 60th day of the year.
Error: Invalid month (13). Must be 1-12.
Error: Invalid day (30) for month 2.
Day 60 of 2024 is 2/29.
Error: Invalid day of year (400). Must be 1-366.
400 is an invalid day of the year.
*/
