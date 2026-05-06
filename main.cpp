#include <iostream>
#include <fstream>
#include <string>

int main() {
    //open file in read mode
    std::ifstream file("test_data.txt");

    //check if file is successfully opend
    if (!file.is_open()){
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    //buffer
    std::string s;
    
    // read file
    while(std::getline(file, s));

    file.close();
    return 0;
}

