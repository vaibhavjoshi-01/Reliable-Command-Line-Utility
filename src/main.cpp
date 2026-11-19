#include "Library.h"
#include "CSVManager.h"
#include "InputValidator.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

namespace {
const std::string DATA_FILE = "data/books.csv";
std::string ask(const std::string& prompt) { std::cout << prompt; std::string s; std::getline(std::cin,s); return InputValidator::trim(s); }
int askInt(const std::string& prompt) { while(true){std::cout<<prompt;std::string s;std::getline(std::cin,s);try{std::size_t n=0;int v=std::stoi(s,&n);if(n==s.size())return v;}catch(...){}std::cout<<"Please enter a valid whole number.\n";} }
void printBooks(const std::vector<Book>& books) {
    if(books.empty()){std::cout<<"No books found.\n";return;}
    std::cout<<"\n"<<std::left<<std::setw(12)<<"ID"<<std::setw(27)<<"TITLE"<<std::setw(22)<<"AUTHOR"<<std::setw(16)<<"CATEGORY"<<std::setw(8)<<"YEAR"<<"STATUS\n";
    std::cout<<std::string(95,'-')<<"\n";
    for(const auto& b:books)std::cout<<std::left<<std::setw(12)<<b.id<<std::setw(27)<<b.title.substr(0,25)<<std::setw(22)<<b.author.substr(0,20)<<std::setw(16)<<b.category.substr(0,14)<<std::setw(8)<<b.publicationYear<<(b.borrowed?"Borrowed":"Available")<<"\n";
}
void save(const Library& lib){std::string error;if(!CSVManager::save(DATA_FILE,lib.all(),&error))std::cout<<"WARNING: "<<error<<"\n";}
void showError(const std::string& e){std::cout<<"Operation failed: "<<e<<"\n";}
}
int main(){
    Library library; std::vector<std::string> loadErrors; auto initial=CSVManager::load(DATA_FILE,&loadErrors); library.replaceAll(std::move(initial));
    if(!loadErrors.empty()){std::cout<<"Data load notes:\n";for(const auto& e:loadErrors)std::cout<<" - "<<e<<"\n";}
    while(true){
        std::cout<<"\n===============================================\n OFFLINE LIBRARY MANAGER\n===============================================\n"
                 <<"1. Add book\n2. List all books\n3. Search books\n4. Update book\n5. Delete book\n6. Borrow book\n7. Return book\n8. Import CSV\n9. Export CSV\n10. Library statistics\n0. Save and exit\n";
        int choice=askInt("Select an option: "); std::string id,error;
        if(choice==0){save(library);std::cout<<"Goodbye. Your library data has been saved.\n";break;}
        switch(choice){
        case 1:{Book b;b.id=ask("Book ID: ");b.title=ask("Title: ");b.author=ask("Author: ");b.category=ask("Category: ");b.publicationYear=askInt("Publication year (0-9999): ");if(library.addBook(b,&error)){save(library);std::cout<<"Book added.\n";}else showError(error);break;}
        case 2:printBooks(library.all());break;
        case 3:{auto q=ask("Search ID, title, author or category: ");printBooks(library.search(q));break;}
        case 4:{id=ask("Book ID to update: ");Book b;b.title=ask("New title: ");b.author=ask("New author: ");b.category=ask("New category: ");b.publicationYear=askInt("New publication year: ");if(library.updateBook(id,b,&error)){save(library);std::cout<<"Book updated.\n";}else showError(error);break;}
        case 5:{id=ask("Book ID to delete: ");if(ask("Type YES to confirm deletion: ")!="YES"){std::cout<<"Deletion cancelled.\n";break;}if(library.deleteBook(id,&error)){save(library);std::cout<<"Book deleted.\n";}else showError(error);break;}
        case 6:{id=ask("Book ID: ");auto borrower=ask("Borrower name: ");auto date=ask("Borrow date (YYYY-MM-DD): ");if(library.borrowBook(id,borrower,date,&error)){save(library);std::cout<<"Book borrowed.\n";}else showError(error);break;}
        case 7:{id=ask("Book ID: ");auto date=ask("Return date (YYYY-MM-DD): ");if(library.returnBook(id,date,&error)){save(library);std::cout<<"Book returned.\n";}else showError(error);break;}
        case 8:{auto path=ask("CSV path to import: ");std::vector<std::string> errors;auto imported=CSVManager::load(path,&errors);std::size_t added=0;for(const auto& b:imported)if(library.addBook(b))++added;save(library);std::cout<<"Imported "<<added<<" new record(s); "<<imported.size()-added<<" duplicate(s) skipped.\n";for(const auto& e:errors)std::cout<<" - "<<e<<"\n";break;}
        case 9:{auto path=ask("Export path [data/books-export.csv]: ");if(path.empty())path="data/books-export.csv";if(CSVManager::save(path,library.all(),&error))std::cout<<"Export completed: "<<path<<"\n";else showError(error);break;}
        case 10:{auto counts=library.categoryCounts();std::cout<<"Total books: "<<library.all().size()<<"\nAvailable: "<<library.availableCount()<<"\nBorrowed: "<<library.borrowedCount()<<"\nBy category:\n";for(const auto& p:counts)std::cout<<" - "<<p.first<<": "<<p.second<<"\n";break;}
        default:std::cout<<"Invalid menu option. Choose 0-10.\n";
        }
    }
    return 0;
}
