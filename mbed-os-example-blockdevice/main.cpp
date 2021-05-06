#include <cstdio>
#include "BlockDevice.h"

int main(void)
{
    printf("hello world\n");
    BlockDevice *bd = BlockDevice::get_default_instance();
    printf("type : %s!!\n", bd->get_type());
}