#include "mbed.h"
#include <stdio.h>
#include <errno.h>
#include <functional>

#include "BlockDevice.h"

// Maximum number of elements in buffer
#define FORCE_REFORMAT true

#include "FATFileSystem.h"
FATFileSystem fs("fs");

void isThere()
{
    FILE *fp = fopen("/fs/test.txt", "r");
    if (!fp) {
        printf("no file..\n");
        return;
    }

    printf("yes file!!\n");
    fclose(fp);
}

void writeFile()
{

}

int main()
{
    BlockDevice *bd = BlockDevice::get_default_instance();

    printf("\n--- Mbed OS filesystem example ---\n");
    printf("type : %s!!\n", bd->get_type());
    
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
    isThere();

    fs.unmount();
    return 0;
    
}
