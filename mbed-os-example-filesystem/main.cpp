#include "mbed.h"
#include <errno.h>
#include <functional>
#include <stdio.h>

#include "BlockDevice.h"

// Maximum number of elements in buffer
#define FORCE_REFORMAT true
#define FILE_NAME "/fs/test.txt"

#include "FATFileSystem.h"
#include "LittleFileSystem.h"

void isThere()
{
    FILE *fp = fopen(FILE_NAME, "r");
    printf("file is %p\n", fp);
    
    if (!fp) {
        printf("no file..\n");
        return;
    }
    
    char buf[20] = "hello world\n";
    printf("somewhere1 : %d\n", ftell(fp));
    int res = 10;
    printf("somewhere2 : %d\n", ftell(fp));

    rewind(fp);

    char dst[20] = {0};
    while (0 < (res = fread(dst, sizeof(char), sizeof(dst), fp))) 
        printf("dst : %s\n", dst);
    
    printf("res : %d\n", res);
    printf("yes file!!\n");
    fclose(fp);
}

void writeDir()
{
    printf("Dir start!!\n");
 
    int res = mkdir("/fs/testDir", 0777);
    if (0 != res) {
        printf("create dir fail..\n");
        fflush(stdout);
        return;
    }

    printf("create dir success!!\n");
}

void delete_file()
{
    int res = remove(FILE_NAME);
    if (!res) {
        printf("remove success\n");
    } else {
        printf("remove fail...\n");
    }
}

int main()
{
    fflush(nullptr);
    /*
    BufferedSerial pc(USBTX, USBRX);

    pc.set_baud(115200);
    char c;
    char buffer[128] = "hello world\n";
    pc.write(buffer, strlen(buffer));
    */
    printf("\n--- Mbed OS filesystem example ---\n");
    BlockDevice *bd = BlockDevice::get_default_instance();
    printf("type : %s!!\n", bd->get_type());
    LittleFileSystem fs("fs");
#if 1
    int res = fs.mount(bd);
    if (res < 0) {
        printf("mount failed...\n");
        printf("re formfat...\n");
        fflush(stdout);
        res = fs.reformat(bd);
        if (!res) {
            printf("reformat success!!\n"); 
        }
    }

    printf("mount success!!\n");

    isThere();

    //writeDir();

    //delete_file();
    fs.unmount();

    printf("program exit...\n");

    fflush(stdout);

    return 0;
#endif
}
