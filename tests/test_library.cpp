#include "Library.h"
#include "InputValidator.h"
#include <cassert>
#include <iostream>
void runLibraryTests(){
 Library l;assert(l.all().empty());Book b{"B1","The C++ Journey","A. Writer","Technology",2022};std::string e;
 assert(l.addBook(b,&e));assert(!l.addBook(b,&e));assert(l.findById("B1")!=nullptr);assert(l.findById("missing")==nullptr);
 assert(l.search("c++").size()==1);Book u=b;u.title="Modern C++";assert(l.updateBook("B1",u,&e));assert(l.findById("B1")->title=="Modern C++");
 assert(l.borrowBook("B1","Test Reader","2024-02-29",&e));assert(!l.borrowBook("B1","Other","2024-03-01",&e));assert(!l.deleteBook("B1",&e));assert(l.returnBook("B1","2024-03-01",&e));assert(!l.returnBook("B1","2024-03-02",&e));assert(l.deleteBook("B1",&e));
 assert(InputValidator::isValidDate("2024-02-29"));assert(!InputValidator::isValidDate("2023-02-29"));assert(!InputValidator::isValidDate("2024-13-01"));assert(!InputValidator::isValidDate("2024-04-31"));
 std::cout<<"Library tests passed.\n";
}
