#include <iostream>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    //open file
    int fd = open("test_data.txt", O_RDONLY);
    if(fd == -1){
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    //retrieve file size
    struct stat sBuff;
    if (fstat(fd, &sBuff) == -1) {
        std::cout << "Error getting file stats" << std::endl;
        close(fd);
        return 1;
    }
    size_t fileSize = sBuff.st_size;

    //map file into process memory
    char* data = (char*)mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        std::cerr << "Mapping failed!" << std::endl;
        return 1;
    }

   
    std::cout << "Successfully mapped " << fileSize << " bytes." << std::endl;

    //close fd and unmap memory when done (next time)

    return 0;
}

/*NEXT TIME WORK ON!
Specifically, we will:

Pointer Arithmetic: We’ll learn how to "jump" straight to the data we want 
instead of scanning every character

The Memory Walk: Loop through your char* data pointer byte-by-byte.

Pointer Match: Write a super-fast if check for the sequence '5','4','=','1'.

The Benchmark: Time it against your first version to see the massive speed increase.
*/