#include "initramfs.hpp"

#include <string.h>
#include <stdio.h>
#include <util.h>

namespace TarFS {
    size_t ASCII_OCT_To_UInt(char* str, size_t len) {
        if (str == nullptr)
            return (size_t)-1;
        size_t num = 0;
        for (size_t i = 0; i < len; i++) {
            if (str[i] == 0)
                break;
            num *= 8;
            num += str[i] - '0';
        }
        return num;
    }

    int USTAR_Lookup(uint8_t* archive, char* filename, uint8_t** out) {
        uint8_t* ptr = archive;
 
        while (memcmp(ptr + 257, "ustar", 5) == 0) {
            int filesize = ASCII_OCT_To_UInt((char*)((uint64_t)ptr + 0x7c), 12);
            if (memcmp(ptr, filename, strlen(filename) + 1) == 0) {
                *out = (uint8_t*)((uint64_t)ptr + 512);
                return filesize;
            }
            ptr = (uint8_t*)((uint64_t)ptr + (((filesize + 511) / 512) + 1) * 512);
        }
        return 0;
    }

    void EnumerateUSTAR(uint8_t* archive) {
        USTARItemHeader* header = (USTARItemHeader*)archive;
 
        while (memcmp(&(header->ID), "ustar", 5) == 0) {
            
            uint64_t size = ASCII_OCT_To_UInt(header->size, 12);
            printf("USTAR item: path=\"%s\", size=%lu, type=%c\n", header->filepath, size, header->TypeFlag);

            if ((header->TypeFlag - '0') == 0) {
                puts("File contents:\n"); // printing 2 new lines is intentional
                fwrite((const void*)((uint64_t)header + 512), 1, size, stdout);
                puts("\n"); // printing 2 new lines is intentional
            }

            header = (USTARItemHeader*)((uint64_t)header + 512 + ALIGN_UP(size, 512));
        }
    }
}