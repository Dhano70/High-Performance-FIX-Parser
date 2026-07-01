#include <iostream>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>

int main() {
    int fd = open("test_data.txt", O_RDONLY);
    if(fd == -1){
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
        std::cerr << "Error: getting file stats" << std::endl;
        close(fd);
        return 1;
    }

    size_t fileSize = fileStat.st_size;
    if (fileSize < 4) {
        std::cerr << "Error: file too small\n";
        close(fd);
        return 0;
    }

    char* data = (char*)mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        std::cerr << "Error: mapping failed!" << std::endl;
        close(fd);
        return 1;
    }

    std::cout << "Successfully mapped " << fileSize << " bytes." << std::endl;

    int buyCount = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < fileSize - 4; ) {
        if (data[i] == '5' && data[i+1] == '4' && 
            data[i+2] == '=' && data[i+3] == '1') {
            buyCount++;
            i += 4; // Jump completely past "54=1"
        } else {
            i++; // Regular 1-byte slide
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "buyCount = " << buyCount << std::endl;
    std::cout << "Time taken by Zero-Copy: " << duration << " microseconds." << std::endl;

    munmap(data, fileSize);
    close(fd);

    return 0;
}