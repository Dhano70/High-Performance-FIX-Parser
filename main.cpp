#include <iostream>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>

/* Helper functions */

inline int64_t parse_price(const char*& ptr) {
    int64_t dollars = 0;
    while (*ptr >= '0' && *ptr <= '9') {
        dollars = dollars * 10 + (*ptr - '0');
        ptr++;
    }
    
    int64_t cents = 0;
    if (*ptr == '.') {
        ptr++; // Skip the decimal point '.'
        
        // Parse exactly two decimal places for cents
        if (*ptr >= '0' && *ptr <= '9') {
            cents = (*ptr - '0') * 10;
            ptr++;
        }
        if (*ptr >= '0' && *ptr <= '9') {
            cents += (*ptr - '0');
            ptr++;
        }
    }
    return (dollars * 100) + cents;
}

inline uint32_t parse_int(const char*& ptr) {
    uint32_t val = 0;
    while (*ptr >= '0' && *ptr <= '9') {
        val = val * 10 + (*ptr - '0');
        ptr++;
    } 
    return val;
}

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

    uint64_t totalBuyVolume = 0;
    uint64_t totalSellVolume = 0;
    int64_t totalNotionalValue = 0;

    auto start = std::chrono::high_resolution_clock::now();

   const char* current = data;
    const char* endPtr = data + fileSize;

    while (current < endPtr) {
        // Look for Tag 54 (Side) -> "54="
        if (*current == '5' && *(current + 1) == '4' && *(current + 2) == '=') {
            current += 3; 
            char side = *current; 
            
            // Fast-forward our pointer until we hit Tag 38 (Quantity) -> "38="
            while (current < endPtr && !(*current == '3' && *(current + 1) == '8' && *(current + 2) == '=')) {
                current++;
            }
            current += 3; 
            uint32_t qty = parse_int(current); 

            // Fast-forward our pointer until we hit Tag 44 (Price) -> "44="
            while (current < endPtr && !(*current == '4' && *(current + 1) == '4' && *(current + 2) == '=')) {
                current++;
            }
            current += 3; 
            int64_t price = parse_price(current); 

            if (side == '1') {
                totalBuyVolume += qty;
            } else if (side == '2') {
                totalSellVolume += qty;
            }
            totalNotionalValue += (price * qty);
        }
        current++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "HFT Engine Summary" << std::endl;
    std::cout << "Total Buy Volume: " << totalBuyVolume << " shares" << std::endl;
    std::cout << "Total Sell Volume: " << totalSellVolume << " shares" << std::endl;
    std::cout << "Total Notional Value: €" << std::fixed << (totalNotionalValue / 100.0) << std::endl;
    std::cout << "Time taken: " << duration << " microseconds." << std::endl;

    munmap(data, fileSize);
    close(fd);

    return 0;
}