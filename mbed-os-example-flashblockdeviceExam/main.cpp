/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include <errno.h>
#include <functional>
#include <stdio.h>

#include "BlockDevice.h"

// Maximum number of elements in buffer
#define BUFFER_MAX_LEN 10
#define FORCE_REFORMAT true
// This will take the system's default block device
BlockDevice *bd = BlockDevice::get_default_instance();

// Instead of the default block device, you can define your own block device.
// For example: HeapBlockDevice with size of 2048 bytes, read size 1, write size 1 and erase size 512.
// #include "HeapBlockDevice.h"
// BlockDevice *bd = new HeapBlockDevice(2048, 1, 1, 512);

// This example uses LittleFileSystem as the default file system
#include "LittleFileSystem.h"
LittleFileSystem fs("fs");
#include "FlashIAPBlockDevice.h"

// Uncomment the following two lines and comment the previous two to use FAT file system.
// #include "FATFileSystem.h"
// FATFileSystem fs("fs");

// Set up the button to trigger an erase
InterruptIn irq(BUTTON1);
void erase()
{
    printf("Initializing the block device... ");
    fflush(stdout);
    int err = bd->init();
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        error("error: %s (%d)\n", strerror(-err), err);
    }

    printf("Erasing the block device... ");
    fflush(stdout);
    err = bd->erase(0, bd->size());
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        error("error: %s (%d)\n", strerror(-err), err);
    }

    printf("Deinitializing the block device... ");
    fflush(stdout);
    err = bd->deinit();
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        error("error: %s (%d)\n", strerror(-err), err);
    }
}

static auto erase_event = mbed_event_queue() -> make_user_allocated_event(erase);

// Entry point for the example
void make_file()
{
    // Try to mount the filesystem
    printf("Mounting the filesystem... ");
    fflush(stdout);
    int err = fs.mount(bd);
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err || FORCE_REFORMAT) {
        // Reformat if we can't mount the filesystem
        printf("formatting... ");
        fflush(stdout);
        err = fs.reformat(bd);
        printf("%s\n", (err ? "Fail :(" : "OK"));
        if (err) {
            error("error: %s (%d)\n", strerror(-err), err);
        }
    }

    // Open the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);
    FILE *f = fopen("/fs/numbers.txt", "w+");
    if (!f) {
        printf("\tfail to make a file...\n");
        return;
    }

    printf("\tmake a files!!\n");
}

void remove_file()
{
    printf("Mounting the filesystem... ");
    fflush(stdout);
    int err = fs.mount(bd);
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err || FORCE_REFORMAT) {
        // Reformat if we can't mount the filesystem
        printf("formatting... ");
        fflush(stdout);
        err = fs.reformat(bd);
        printf("%s\n", (err ? "Fail :(" : "OK"));
        if (err) {
            error("error: %s (%d)\n", strerror(-err), err);
        }
    }

    // Open the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);
    FILE *f = fopen("/fs/numbers.txt", "r+");
    if (!f) {
        printf("\tno files...\n");
        return ;
    }

    int res = remove("/fs/numbers.txt");
    if (!res) {
        printf("\tfail to remove a files\n");
        return;
    }

    printf("\tyes files!!\n");
}

void TestFunc()
{
    printf("TestFunc start...!\n");
    FlashIAP flash;
    const uint32_t flash_start = flash.get_flash_start();
    const uint32_t flash_size = flash.get_flash_size();
    const uint32_t flash_end = flash_start + flash_size;
    const uint32_t page_size = flash.get_page_size();            // in bytes
    uint32_t sector_size = flash.get_sector_size(flash_end - 1); // in bytes
    uint32_t addr = flash_end - sector_size;
    //FlashIAPBlockDevice fib(flash_start, flash_size);
    FlashIAPBlockDevice fib;
    fib.init();
    int *ptr = new int[15];
    for (int i = 0; i < 15; i++) {
        ptr[i] = 10;
    }
    int res = fib.read(ptr, sector_size - 1, sizeof(int) * 15);
    if (!res) {
        printf("\tin TestFunc, read flash fail...\n");
    }

    for (int i = 0; i < 15; i++) {
        printf("%d : %d\t", i, ptr[i]);
    }

    printf("\n");

    fib.deinit();
    delete [] ptr;
    printf("TestFunc end....\n");
}

int main()
{
    printf("\n\n\t--- Mbed OS filesystem example ---\n");

    // Setup the erase event on button press, use the event queue
    // to avoid running in interrupt context
    irq.fall(std::ref(erase_event));
    //make_file();
    //remove_file();
    TestFunc();

    printf("Mbed OS filesystem example done!\n");
}