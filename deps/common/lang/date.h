//
// Created by KaiserTT on 2024/03/29.
//
#include <string>

namespace common {
    bool is_leap_year(const int& year);

    bool check_date(const int& year, const int& month, const int& day);

    void intDate_to_strDate(const int& int_date, std::string& str_date);

    void strDate_to_intDate(const std::string& str_date, int& int_date);
}