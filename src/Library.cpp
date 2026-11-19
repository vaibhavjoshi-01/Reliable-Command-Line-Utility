#include "Library.h"
#include "InputValidator.h"
#include <algorithm>

void Library::rebuildIndex() { index_.clear(); for (std::size_t i = 0; i < books_.size(); ++i) index_[books_[i].id] = i; }
bool Library::addBook(const Book& b, std::string* e) {
    if (!InputValidator::isNonEmpty(b.id) || !InputValidator::isNonEmpty(b.title) || !InputValidator::isNonEmpty(b.author) || !InputValidator::isNonEmpty(b.category) || !InputValidator::isValidYear(b.publicationYear)) { if (e) *e = "Invalid or missing book fields"; return false; }
    if (index_.count(b.id)) { if (e) *e = "Duplicate book ID"; return false; }
    books_.push_back(b); index_[b.id] = books_.size() - 1; return true;
}
bool Library::updateBook(const std::string& id, const Book& b, std::string* e) {
    auto it = index_.find(id); if (it == index_.end()) { if (e) *e = "Book not found"; return false; }
    if (!InputValidator::isNonEmpty(b.title) || !InputValidator::isNonEmpty(b.author) || !InputValidator::isNonEmpty(b.category) || !InputValidator::isValidYear(b.publicationYear)) { if (e) *e = "Invalid updated fields"; return false; }
    Book& old = books_[it->second]; old.title=b.title; old.author=b.author; old.category=b.category; old.publicationYear=b.publicationYear; return true;
}
bool Library::deleteBook(const std::string& id, std::string* e) { auto it=index_.find(id); if(it==index_.end()){if(e)*e="Book not found";return false;} if(books_[it->second].borrowed){if(e)*e="Cannot delete a borrowed book";return false;} books_.erase(books_.begin()+static_cast<long>(it->second)); rebuildIndex(); return true; }
bool Library::borrowBook(const std::string& id,const std::string& borrower,const std::string& date,std::string* e){auto it=index_.find(id);if(it==index_.end()){if(e)*e="Book not found";return false;}if(books_[it->second].borrowed){if(e)*e="Book is already borrowed";return false;}if(!InputValidator::isNonEmpty(borrower)||!InputValidator::isValidDate(date)){if(e)*e="Invalid borrower or date";return false;}auto& b=books_[it->second];b.borrowed=true;b.borrower=borrower;b.borrowDate=date;b.returnDate.clear();return true;}
bool Library::returnBook(const std::string& id,const std::string& date,std::string* e){auto it=index_.find(id);if(it==index_.end()){if(e)*e="Book not found";return false;}auto& b=books_[it->second];if(!b.borrowed){if(e)*e="Book is not currently borrowed";return false;}if(!InputValidator::isValidDate(date)||date<b.borrowDate){if(e)*e="Invalid return date";return false;}b.borrowed=false;b.returnDate=date;b.borrower.clear();b.borrowDate.clear();return true;}
const Book* Library::findById(const std::string& id) const {auto it=index_.find(id);return it==index_.end()?nullptr:&books_[it->second];}
std::vector<Book> Library::search(const std::string& q) const {std::vector<Book> out;auto needle=InputValidator::toLower(q);for(const auto& b:books_)if(InputValidator::toLower(b.id).find(needle)!=std::string::npos||InputValidator::toLower(b.title).find(needle)!=std::string::npos||InputValidator::toLower(b.author).find(needle)!=std::string::npos||InputValidator::toLower(b.category).find(needle)!=std::string::npos)out.push_back(b);return out;}
std::size_t Library::availableCount() const {return books_.size()-borrowedCount();}
std::size_t Library::borrowedCount() const {return std::count_if(books_.begin(),books_.end(),[](const Book& b){return b.borrowed;});}
std::unordered_map<std::string,std::size_t> Library::categoryCounts() const {std::unordered_map<std::string,std::size_t> r;for(const auto& b:books_)++r[b.category];return r;}
void Library::replaceAll(std::vector<Book> books){books_=std::move(books);rebuildIndex();}
