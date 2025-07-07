/*Rewrite the routines day_of_year and month_day with pointers instead of
indexing.*/
#include <stdio.h>
static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},  
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int day_of_year(int year, int month, int day) {
    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    char *p = daytab[leap]; 

    while (--month) 
        day += *(++p);

    return day;
}
void month_day(int year, int yearday, int *pmonth, int *pday) {
    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    char *p = daytab[leap];

    while (yearday > *(++p))
        yearday -= *p;

    *pmonth = p - daytab[leap];
    *pday = yearday;
}
int main() {
    int year = 2024;
    int doy = day_of_year(year, 5, 1); 
    printf("%d\n",doy);

    int month, day;
    month_day(year, 122, &month, &day);
    printf("%d/%d.\n", day,month);

    return 0;
}
/*122
1/5.*/
