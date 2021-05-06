#include "mbed.h"
#include <cstring>

FlashIAP flash;

void write_to_flash()
{
    flash.init();

    const uint32_t flash_start = flash.get_flash_start();
    const uint32_t flash_size = flash.get_flash_size();
    const uint32_t flash_end = flash_start + flash_size;
    const uint32_t page_size = flash.get_page_size();            // in bytes
    uint32_t sector_size = flash.get_sector_size(flash_end - 1); // in bytes
    uint32_t addr = flash_end - sector_size;                     // address of first byte in the last sector

    printf("flash_start = 0x%.8x\r\n", flash_start);
    printf("flash_size = 0x%.8x\r\n", flash_size);
    printf("flash_end = 0x%.8x\r\n", flash_end);
    printf("page_size = 0x%.8x\r\n", page_size);
    printf("sector_size = 0x%.8x\r\n", sector_size);
    printf("addr = 0x%.8x\r\n", addr);
    printf("----------------------\r\n");

    // Create an array in SRAM (later on to be stored in the first page of last sector in FLASH)
    uint32_t data_len = page_size / sizeof(uint32_t); // calculate available space
    uint32_t *data = new uint32_t[data_len]{1, 2, 3}; // create an array in SRAM and initialize the first three elements

    // Print the first 15 elements
    int k = 35;
    for (int i = 0; i < 15; i++) {
        data[i] = k++;
        printf("data[%d] = %d\r\n", i, data[i]);
    }

    // Store the array 'data' in FLASH
    flash.erase(addr, sector_size);       // erase needs sector_size (not page size)
    flash.program(data, addr, sizeof(int) * 15); // write needs page_size

    delete[] data;
    flash.deinit();

    printf("----------------------\r\n");
    printf("\twrite Done.\r\n");
}

void read_to_flash()
{
    flash.init();

    const uint32_t flash_start = flash.get_flash_start();
    const uint32_t flash_size = flash.get_flash_size();
    const uint32_t flash_end = flash_start + flash_size;
    const uint32_t page_size = flash.get_page_size();            // in bytes
    uint32_t sector_size = flash.get_sector_size(flash_end - 1); // in bytes
    uint32_t addr = flash_end - sector_size;                     // address of first byte in the last sector

    printf("flash_start = 0x%.8x\r\n", flash_start);
    printf("flash_size = 0x%.8x\r\n", flash_size);
    printf("flash_end = 0x%.8x\r\n", flash_end);
    printf("page_size = 0x%.8x\r\n", page_size);
    printf("sector_size = 0x%.8x\r\n", sector_size);
    printf("addr = 0x%.8x\r\n", addr);
    printf("----------------------\r\n");

    int* data = new int[15];
    memset(data, 0, sizeof(data));

    flash.read(data, addr, sizeof(int) * 15); // read needs page_size

    // Print the first 15 elements
    for (int i = 0; i < 15; i++) {
        printf("data[%d] = %d\r\n", i, data[i]);
    }

    delete [] data;
    flash.deinit();

    printf("----------------------\r\n");
    printf("\tread Done.\r\n");
}

int main()
{
    //printf("\n\twrite Starting...\r\n");

    //write_to_flash();

    printf("\n\tread Starting...\r\n");

    read_to_flash();
}
