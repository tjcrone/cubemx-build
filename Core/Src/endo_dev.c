/* Development code for Endurologger */

// includes
#include "endo_dev.h"
#include "string.h"
#include "gpio.h"
#include "fatfs.h"
#include "deadbeef.h"

// static variables in bss
static const char deadbeef[][16] = DEADBEEF;

// global variables
FRESULT fres;
FATFS fs = { 0 };
UINT bytesread;
BYTE work[4096];
FIL fil;

// delay function without using HAL
void loop_delay (uint32_t loops) {
    for (uint32_t i = 0; i < loops; i++) {
        for (uint32_t j = 0; j < SystemCoreClock/18020; j++) {}
    }
}

void flash(uint32_t n, uint32_t on_time, uint32_t off_time) {
  for (uint8_t i = 0; i<n; i++) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    loop_delay(on_time);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    loop_delay(off_time);
  }
}

// full test suite
void sd_test(void) {
    sd_format();
    HAL_Delay(1000);

    sd_mount();
    HAL_Delay(1000);

    sd_check_free();
    HAL_Delay(1000);

    /*
    sd_test_delete();
    HAL_Delay(1000);

    sd_test_write();
    HAL_Delay(1000);
    */
}

// format card
void sd_format(void) {
    flash(1, 200, 200); // test 1
    HAL_Delay(1000);
    fres = f_mkfs("ENDO", FM_EXFAT, 0, work, sizeof work);
    if (fres != FR_OK)
        Error_Handler();
    flash(8, 50, 50); // success
}

// mount sd card
void sd_mount(void) {
    flash(2, 200, 200); // test 2
    HAL_Delay(1000);
    fres = f_mount(&fs, "", 1);
    if (fres != FR_OK)
        Error_Handler();
    flash(8, 50, 50); // success
}

// check sd free space
void sd_check_free(void) {
    flash(3, 200, 200); // test 3
    HAL_Delay(1000);
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
    flash(8, 50, 50); // success
}

// write test file
void sd_test_write(void) {
    flash(4, 200, 200); // test 4
    HAL_Delay(1000);

    // open file to write
    fres = f_open(&fil, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (fres != FR_OK)
        Error_Handler();

    // write deadbeef data to file
    uint32_t datarows = ( (uint32_t) sizeof(deadbeef) ) / 16;
    UINT byteswritten;
    //for (uint32_t i = 0; i < datarows; i++) {
    fres = f_write(&fil, &deadbeef, 16 * datarows, &byteswritten);
    if (fres != FR_OK)
        Error_Handler();

    fres = f_sync(&fil);
    if (fres != FR_OK)
        Error_Handler();

    fres = f_close(&fil);
    if (fres != FR_OK)
        Error_Handler();

    flash(8, 50, 50); // success
}

// delete test file
void sd_test_delete(void) {
    flash(5, 200, 200); // test 5
    HAL_Delay(1000);
    fres = f_unlink("test.txt");
    flash(8, 50, 50); // success
}

/*
    // write text to file
    uint16_t total_size = 10; // size of file in MB

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
*/


// check test file



// delete test file



// format sd card




// sd card test scratch
    /*




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