/* Development code for Endurologger */

// includes
#include "stdint.h"
#include "gpio.h"
#include "fatfs.h"
#include "endo_dev.h"

// global variables
FRESULT fres;
FATFS fs = { 0 };
UINT byteswritten;
UINT bytesread;
BYTE work[4096];
FIL fil;
static const char deadbeef[][16] = DEADBEEF;

// delay function without using HAL
void loop_delay (uint32_t loops) {
    for (uint32_t i = 0; i < loops; i++) {
        for (uint32_t j = 0; j < SystemCoreClock/18020; j++) {}
    }
}

// basic LED flasher n-times
void flash(uint32_t n, uint32_t delay) {
  for (uint8_t i = 0; i<n; i++) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_Delay(delay);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_Delay(delay);
  }
}

// full test suite
void sd_test(void) {
    flash(1, 200);
    HAL_Delay(1000);
    sd_mount();
    flash(8, 50);
    HAL_Delay(1000);

    flash(2, 200);
    HAL_Delay(1000);
    sd_check_free();
    flash(8, 50);
    HAL_Delay(1000);
}

// mount sd card
void sd_mount(void) {
    fres = f_mount(&fs, "", 1);
    if (fres != FR_OK)
        Error_Handler();
}

// check sd free space
void sd_check_free(void) {
    DWORD fre_clust;
    FATFS *pfs;
    fres = f_getfree("", &fre_clust, &pfs);
    if (fres != FR_OK)
        Error_Handler();

    // calclulate sizes
    uint64_t volume_tot = ( (uint64_t) (pfs->n_fatent - 2) ) * pfs->csize * 512; // total volume size in bytes
    uint64_t volume_fre = ( (uint64_t) (fre_clust) ) * pfs->csize * 512; // total volume size in bytes

    // samsung pro 32
    if ( volume_tot != 32022691840 )
        Error_Handler();
    if ( volume_fre != 32022495232 )
        Error_Handler();
}



// sd card test scratch
    /*
    // format card
    start = uwTick;
    fres = f_mkfs("", FM_EXFAT, 0, work, sizeof work);
    stop = uwTick;
    uint32_t  format_time = stop - start;
    if (fres != FR_OK)
        Error_Handler();


    // write text to file
    uint16_t total_size = 10; // size of file in MB

    // open file to write
    fres = f_open(&fil, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (fres != FR_OK)
        Error_Handler();

    // write data to file
    for (uint32_t i = 0; i < total_size * 1024 / 16; i++) {
        fres = f_write(&fil, &data, 16 * 1024, &byteswritten);
        if (fres != FR_OK)
            Error_Handler();

        // fres = f_sync(&fil);
        // if (fres != FR_OK)
        // error_loop();
        //if (byteswritten != 16 * 1024)
        //    Error_Handler();
    }

    // close file
    fres = f_close(&fil);
    if (fres != FR_OK)
        Error_Handler();



    // check file data
    fres = f_open(&fil, "test.txt", FA_READ);
    if (fres != FR_OK)
        Error_Handler();

    char data_read[17] = { 0 };
    char data_check[17] = "0123456789abcde\n\0";

    for (uint32_t i = 0; i < total_size * 1024 * 1024 / 16; i++) {
        fres = f_read(&fil, data_read, 16, &bytesread);
        if (fres != FR_OK)
            Error_Handler();

        if (strcmp(data_read, data_check) != 0)
            Error_Handler();
    }

    // close file
    fres = f_close(&fil);
    if (fres != FR_OK)
        Error_Handler();

  */