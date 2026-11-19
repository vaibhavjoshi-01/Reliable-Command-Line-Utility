#include "InputValidator.h"
#include <algorithm>
#include <cctype>
#include <cstdio>

namespace InputValidator {
std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return {};
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}
bool isNonEmpty(const std::string& value) { return !trim(value).empty(); }
std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}
bool isValidYear(int year) { return year >= 0 && year <= 9999; }
bool isValidDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    int y = 0, m = 0, d = 0;
    if (std::sscanf(date.c_str(), "%4d-%2d-%2d", &y, &m, &d) != 3) return false;
    if (y < 1 || m < 1 || m > 12) return false;
    static const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int maxDay = days[m - 1];
    const bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (m == 2 && leap) maxDay = 29;
    return d >= 1 && d <= maxDay;
}
}
