#include "date.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "common/log/log.h"

namespace common {

bool is_leap_year(const int& year)
{
    return ((year % 100 != 0 && year % 4 == 0) || (year % 400 == 0));
}

bool check_date(const int& year, const int& month, const int& day)
{
    
    int leap_flag = is_leap_year(year);
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }

    int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (leap_flag)
        days_in_month[2 - 1] = 29;
    
    if (day > days_in_month[month - 1]) {
        return false;
    }
    
    return true;
}

bool check_date(const int& int_date) {
    int year = int_date / 10000;
    int month = (int_date % 10000) / 100;
    int day = int_date % 100;

    return check_date(year, month, day);
}

void intDate_to_strDate(const int& int_date, std::string& str_date)
{
    int year = int_date / 10000;
    int month = (int_date % 10000) / 100;
    int day = int_date % 100;

    std::ostringstream ss;
    ss << year << '-'
       << std::setw(2) << std::setfill('0') << month << '-'
       << std::setw(2) << std::setfill('0') << day;

    str_date = ss.str();
}

void strDate_to_intDate(const std::string& str_date, int& int_date)
{
    std::stringstream ss(str_date);
    int year = 0, month = 0, day = 0;
    char delimeter;

    ss >> year >> delimeter >> month >> delimeter >> day;
    
    if (check_date(year, month, day))
        int_date = year * 10000 + month * 100 + day;
    else
        return;
}

}