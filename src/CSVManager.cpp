#include "CSVManager.h"
#include "InputValidator.h"
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <cstdio>

std::vector<std::string> CSVManager::parseRow(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool quoted = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (quoted) {
            if (c == '"' && i + 1 < line.size() && line[i + 1] == '"') { field += '"'; ++i; }
            else if (c == '"') quoted = false;
            else field += c;
        } else if (c == ',' && !quoted) { fields.push_back(field); field.clear(); }
        else if (c == '"' && field.empty()) quoted = true;
        else field += c;
    }
    if (quoted) return {};
    fields.push_back(field);
    return fields;
}

std::vector<Book> CSVManager::load(const std::string& path, std::vector<std::string>* errors) {
    std::ifstream in(path);
    std::vector<Book> books;
    if (!in) { if (errors) errors->push_back("Could not open file: " + path); return books; }
    std::string line;
    std::size_t row = 0;
    std::unordered_set<std::string> ids;
    while (std::getline(in, line)) {
        ++row;
        if (row == 1 && line.rfind("id,", 0) == 0) continue;
        if (line.empty()) continue;
        auto f = parseRow(line);
        if (f.size() != 9) { if (errors) errors->push_back("Row " + std::to_string(row) + ": expected 9 columns"); continue; }
        try {
            Book b;
            b.id = InputValidator::trim(f[0]); b.title = f[1]; b.author = f[2]; b.category = f[3];
            std::size_t used = 0; int year = std::stoi(f[4], &used);
            if (used != f[4].size() || !InputValidator::isValidYear(year)) throw std::invalid_argument("year");
            b.publicationYear = year;
            if (f[5] != "0" && f[5] != "1") throw std::invalid_argument("borrowed flag");
            b.borrowed = f[5] == "1"; b.borrower = f[6]; b.borrowDate = f[7]; b.returnDate = f[8];
            if (b.id.empty() || b.title.empty() || b.author.empty() || b.category.empty() || ids.count(b.id)) throw std::invalid_argument("required field or duplicate ID");
            if (b.borrowed && (!InputValidator::isNonEmpty(b.borrower) || !InputValidator::isValidDate(b.borrowDate))) throw std::invalid_argument("borrow details");
            ids.insert(b.id); books.push_back(std::move(b));
        } catch (...) { if (errors) errors->push_back("Row " + std::to_string(row) + ": malformed record skipped"); }
    }
    return books;
}

bool CSVManager::save(const std::string& path, const std::vector<Book>& books, std::string* error) {
    const std::string temp = path + ".tmp";
    std::ofstream out(temp, std::ios::trunc);
    if (!out) { if (error) *error = "Cannot write temporary file: " + temp; return false; }
    auto quote = [](const std::string& s) { std::string r = "\""; for (char c : s) { if (c == '"') r += '"'; r += c; } return r + "\""; };
    out << "id,title,author,category,publicationYear,borrowed,borrower,borrowDate,returnDate\n";
    for (const auto& b : books) out << quote(b.id) << ',' << quote(b.title) << ',' << quote(b.author) << ',' << quote(b.category) << ',' << b.publicationYear << ',' << (b.borrowed ? 1 : 0) << ',' << quote(b.borrower) << ',' << quote(b.borrowDate) << ',' << quote(b.returnDate) << '\n';
    out.close();
    if (!out) { if (error) *error = "Failed while writing CSV"; return false; }
    std::remove(path.c_str());
    if (std::rename(temp.c_str(), path.c_str()) != 0) { if (error) *error = "Could not replace destination file"; return false; }
    return true;
}
