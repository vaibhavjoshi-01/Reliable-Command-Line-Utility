#pragma once
#include "Book.h"
#include <string>
#include <vector>

class CSVManager {
public:
    static std::vector<Book> load(const std::string& path, std::vector<std::string>* errors = nullptr);
    static bool save(const std::string& path, const std::vector<Book>& books, std::string* error = nullptr);
    static std::vector<std::string> parseRow(const std::string& line);
};
