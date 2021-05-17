#include "mbed.h"
#include <errno.h>
#include <functional>
#include <stdio.h>

#include "BlockDevice.h"

// Maximum number of elements in buffer
#define FILE_NAME "/fs/test.txt"

#include "FATFileSystem.h"
#include "LittleFileSystem.h"

void write_file(FILE* fp)
{
    printf("file is %p\n", fp);

    char buf[20] = "hello world\n";
    int res = fprintf(fp, "buf : %s\n", buf);
    fflush(fp);
    
    if (res < 0) {
        printf("write buf fail\n");
    }
    
    printf("somewhere1 : %ld\n", ftell(fp));
    printf("somewhere2 : %ld\n", ftell(fp));
    
    printf("write file!!!\n");
    //fclose(fp);
}

void read_file(FILE* fp)
{
    printf("file is %p\n", fp);
    printf("read file!!\n");

    rewind(fp);

    char dst[20] = {0};
    while (0 < fread(dst, sizeof(char), sizeof(dst), fp)) {
        printf("dst : %s\n", dst);
    }
}

void writeDir(FILE* fp)
{
    printf("Dir start!!\n");
 
    int res = mkdir("/fs/testDir", 0777);
    if (0 != res) {
        printf("create dir fail..\n");
        fflush(nullptr);
        return;
    }

    printf("create dir success!!\n");
}

void delete_file(FILE* fp)
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
    printf("\n--- Mbed OS filesystem example ---\n");
    BlockDevice *bd = BlockDevice::get_default_instance();
    printf("type : %s!!\n", bd->get_type());
    LittleFileSystem fs("fs");
#if 1
    
    int res = fs.mount(bd);
    if (res < 0) {
        printf("mount failed...\n");
        printf("re formfat...\n");
        fflush(nullptr);
        res = fs.reformat(bd);
        if (!res) {
            printf("reformat success!!\n"); 
        }
    }

    printf("mount success!!\n");
    
    FILE *fp = fopen(FILE_NAME, "a+b");
    if (!fp) {
        perror("in main");
        
        fflush(stdout);
        return -1;
    }
    
    for (int i = 0; i < 5; i++) {
        //write_file(fp);
    }
    read_file(fp);
    //writeDir();

    //delete_file();
    fs.unmount();

    printf("program exit...\n");

    //fflush(fp);

    fclose(fp);
    return 0;
#endif
}
