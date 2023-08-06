#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "initramfs.hpp"

int64_t fileSizeInBytes(const char* path) {
    struct stat stat_buf;
    int rc = stat(path, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <archive-file>\n", argv[0]);
        return 1;
    }
    int64_t size = fileSizeInBytes(argv[1]);
    if (size < 0) {
        perror("stat");
        return 1;
    }
    //printf("\"%s\" size = %ld", argv[1], size);

    uint8_t* archive = (uint8_t*)malloc(size);

    FILE* stream = fopen(argv[1], "rb");
    if (stream == nullptr) {
        perror("fopen");
        return 1;
    }
    int rc = fseek(stream, 0, SEEK_SET);
    if (rc < 0) {
        perror("fseek");
        return 1;
    }
    size_t rc2 = fread(archive, 1, size, stream);
    if (rc2 != (uint64_t)size) {
        perror("fread");
        return 1;
    }
    rc = fclose(stream);
    if (rc != 0) {
        perror("fclose");
        return 1;
    }

    TarFS::EnumerateUSTAR(archive);
    putchar('e');

    return 0;
}
