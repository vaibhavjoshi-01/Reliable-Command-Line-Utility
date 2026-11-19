#include <iostream>
#include <functional>
#include <vector>
#include <stdexcept>

void runLibraryTests();
void runCSVTests();
int main(){int failed=0;try{runLibraryTests();runCSVTests();}catch(const std::exception& e){std::cerr<<"Test failure: "<<e.what()<<"\n";++failed;}if(failed==0)std::cout<<"All test suites completed successfully.\n";return failed?1:0;}
