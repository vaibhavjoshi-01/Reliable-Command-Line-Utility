#pragma once
#include "Book.h"
#include <string>
#include <vector>
#include <unordered_map>

class Library {
    std::vector<Book> books_;
    std::unordered_map<std::string, std::size_t> index_;
    void rebuildIndex();
public:
    bool addBook(const Book& book, std::string* error = nullptr);
    bool updateBook(const std::string& id, const Book& updated, std::string* error = nullptr);
    bool deleteBook(const std::string& id, std::string* error = nullptr);
    bool borrowBook(const std::string& id, const std::string& borrower, const std::string& date, std::string* error = nullptr);
    bool returnBook(const std::string& id, const std::string& date, std::string* error = nullptr);
    const Book* findById(const std::string& id) const;
    std::vector<Book> search(const std::string& query) const;
    const std::vector<Book>& all() const noexcept { return books_; }
    std::size_t availableCount() const;
    std::size_t borrowedCount() const;
    std::unordered_map<std::string, std::size_t> categoryCounts() const;
    void replaceAll(std::vector<Book> books);
};
