#include "mbed.h"
#include <errno.h>
#include <functional>
#include <stdio.h>

#include "BlockDevice.h"

// Maximum number of elements in buffer
#define FORCE_REFORMAT true
#define FILE_NAME "/fs/test.txt"

#include "LittleFileSystem.h"

void isThere()
{
    FILE *fp = fopen(FILE_NAME, "w+");
    printf("file is %p\n", fp);

    if (!fp) {
        printf("no file..\n");
        fp = fopen(FILE_NAME, "w");
        printf("write file..\n");
    }

    fflush(fp);
    char buf[20] = "hello world";
    int res = fprintf(fp, "buf : %s\n", buf);
    if (res < 0) {
        printf("write buf fail\n");
        return;
    }

    printf("write res : %d\n", res);
    //fwrite(buf, sizeof(char), sizeof(buf), fp);

    char dst[20] = {0};
    while (!feof(fp)) {
        int c = fgetc(fp);
        printf("%c", c);
    }
#if 0
    res = 10;
    res = fread(dst, sizeof(char), sizeof(buf), fp);
     //while (0 != (res = fread(dst, sizeof(char), sizeof(buf), fp)))
        printf("dst : %s\n", dst);

    printf("res : %d\n", res);
    #endif
    printf("yes file!!\n");
    fclose(fp);
}

void writeDir()
{
    printf("Dir start!!\n");

    int res = mkdir("/fs/testDir", 0777);
    if (0 != res) {
        printf("create dir fail..\n");
        return;
    }

    printf("create dir success!!\n");
}

int main()
{
    fflush(stdout);

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
#if 1
    LittleFileSystem fs("fs");
    int res = fs.mount(bd);
    if (res < 0) {
        printf("mount failed...\n");
        printf("re formfat...\n");
        fflush(stdout);
        res = fs.reformat(bd);
        if (!res) {
            printf("reformat success!!\n");
        } else {
            printf("reformat fail...\n");
            return 0;
        }
    }

    printf("mount success!!\n");

    isThere();

    writeDir();

    fs.unmount();

    printf("program exit...\n");

    fflush(stdout);

    return 0;
#endif
}
