#include "CSVManager.h"
#include <cassert>
#include <fstream>
#include <cstdio>
#include <iostream>
void runCSVTests(){
 const std::string path="test-books.csv";Book b{"T1","A, quoted title","Writer \"One\"","Fiction",2020};std::string error;
 assert(CSVManager::save(path,{b},&error));auto loaded=CSVManager::load(path);assert(loaded.size()==1);assert(loaded[0].title==b.title);assert(loaded[0].author==b.author);
 {std::ofstream out(path);out<<"id,title,author,category,publicationYear,borrowed,borrower,borrowDate,returnDate\n";out<<"D1,Good,Author,Fiction,2000,0,,,\n";out<<"D1,Duplicate,Author,Fiction,2000,0,,,\n";out<<"bad,row\n";}
 std::vector<std::string> errors;loaded=CSVManager::load(path,&errors);assert(loaded.size()==1);assert(!errors.empty());
 std::remove(path.c_str());loaded=CSVManager::load("this-file-should-not-exist.csv",&errors);assert(loaded.empty());assert(!errors.empty());
 std::cout<<"CSV tests passed.\n";
}
