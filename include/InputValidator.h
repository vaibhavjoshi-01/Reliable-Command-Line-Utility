#pragma once
#include <string>

namespace InputValidator {
bool isValidDate(const std::string& date);
bool isValidYear(int year);
bool isNonEmpty(const std::string& value);
std::string trim(const std::string& value);
std::string toLower(std::string value);
}
