#pragma once
#include <string>

struct Book {
    std::string id;
    std::string title;
    std::string author;
    std::string category;
    int publicationYear{};
    bool borrowed{false};
    std::string borrower;
    std::string borrowDate;
    std::string returnDate;
};
