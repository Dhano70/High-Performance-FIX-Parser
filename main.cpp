#include <iostream>
#include <fstream>
#include <string>

int main() {
    //open file in read mode
    std::ifstream file("test_data.txt");

    //check if file is successfully opend
    if(!file.is_open()){
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    //buffer
    std::string s;
    
    int buyCount = 0;
    // read file
    while(std::getline(file, s)){
        if(s.find("54=1") != std::string::npos) {
            buyCount ++;
        }
    }
    std::cout << "buyCount = " << buyCount << std::endl;
    file.close();
    return 0;
}

 