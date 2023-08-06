#ifndef _INITRAMFS_HPP
#define _INITRAMFS_HPP

#include <stdint.h>
#include <stddef.h>

namespace TarFS {
    struct USTARItemHeader {
        char filepath[100];
        char mode[8];
        char uid[8];
        char gid[8];
        char size[12];
        char mtime[12];
        char Checksum[8];
        char TypeFlag;
        char filename[100];
        char ID[6]; // should be "ustar", with null termination
        char version[2]; // should be "00", with no null termination
        char OwnerUserName[32];
        char OwnerGroupName[32];
        char DeviceMajorNumber[8];
        char DeviceMinorNumber[8];
        char FilenamePrefix[155];
    } __attribute__((packed));

    enum class USTARItemType {
        FILE = 0,
        HARD_LINK = 1,
        SYM_LINK = 2,
        CHAR_DEVICE = 3,
        BLOCK_DEVICE = 4,
        DIRECTORY = 5,
        NAMED_PIPE = 6
    };

    size_t ASCII_OCT_To_UInt(char* str, size_t len);

    int USTAR_Lookup(uint8_t* archive, char* filename, uint8_t** out);
    void EnumerateUSTAR(uint8_t* archive);
}

#endif /* _INITRAMFS_HPP */