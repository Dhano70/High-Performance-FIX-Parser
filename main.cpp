#include <iostream>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd = open("test_data.txt", O_RDONLY);
    if(fd == -1){
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
        std::cout << "Error getting file stats" << std::endl;
        close(fd);
        return 1;
    }

    size_t fileSize = fileStat.st_size;
    if (fileSize < 4) {
        std::cout << "File too small\n";
        close(fd);
        return 0;
    }

    char* data = (char*)mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        std::cerr << "Mapping failed!" << std::endl;
        close(fd);
        return 1;
    }

    std::cout << "Successfully mapped " << fileSize << " bytes." << std::endl;

    int buyCount = 0;
    for (size_t i = 0; i < fileSize - 4; ++i) {
        if (data[i] == '5' && data[i+1] == '4' && 
            data[i+2] == '=' && data[i+3] == '1') {
            buyCount++;
            i += 3; // Skip the characters that matched
        }
    }

    std::cout << "buyCount = " << buyCount << std::endl;

    munmap(data, fileSize);
    close(fd);

    return 0;
}