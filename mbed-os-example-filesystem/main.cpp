#include "mbed.h"
#include <stdio.h>
#include <errno.h>
#include <functional>

#include "BlockDevice.h"

// Maximum number of elements in buffer
#define FORCE_REFORMAT true
#define FILE_NAME "/fs/test.txt"

#include "FATFileSystem.h"
FATFileSystem fs("fs");

void isThere()
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) {
        printf("no file..\n");
        return;
    }

    printf("yes file!!\n");
    fclose(fp);
}

void writeDir()
{
    printf("Dir start!!\n");

    int res = mkdir("/fs/testDir", O_RDONLY);
    if (res) {
        printf("create dir fail..\n");
        return;
    }

    printf("create dir success!!\n");
}

int main()
{
    BlockDevice *bd = BlockDevice::get_default_instance();

    printf("\n--- Mbed OS filesystem example ---\n");
    printf("type : %s!!\n", bd->get_type());
    /*
    int res = fs.mount(bd);
    if (res < 0) {
        printf("mount failed...\n");
        printf("re format...\n");
        fflush(stdout);
        res = fs.reformat(bd);
        if (!res) {
            printf("reformat success!!\n");
        }
    }

    printf("mount success!!\n");

    writeDir();
    fs.unmount();
    return 0;
    */
}
