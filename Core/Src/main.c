/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "string.h"
#include "deadbeef.h"
#include "SEGGER_RTT.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

FRESULT fres;
FATFS fs = { 0 };
UINT byteswritten;
UINT bytesread;
BYTE work[4096];
FIL fil;

static const char deadbeef[][16] = DEADBEEF;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  HAL_Delay(1000);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  // toggle mcu led
  for (uint8_t i = 0; i<10; i++) {
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5);
  HAL_Delay(50);
  }

  uint32_t  start;
  uint32_t  stop;

    /*
    // power-on memory module
    mem_pwr_off();
    HAL_Delay(100);
    mem_pwr_on();
    */

    // format card
    SEGGER_RTT_printf(0, "formating card\n");
    start = uwTick;
    fres = f_mkfs("", FM_EXFAT, 0, work, sizeof work);
    stop = uwTick;
    uint32_t  format_time = stop - start;
    if (fres != FR_OK)
        Error_Handler();
    SEGGER_RTT_printf(0, "format time: %u\n", format_time);

    //mount card
    start = uwTick;
    fres = f_mount(&fs, "", 1);
    stop = uwTick;
    uint32_t  mount_time = stop - start;
    if (fres != FR_OK)
        Error_Handler();
    SEGGER_RTT_printf(0, "mount time: %u\n", mount_time);

    // check free space
    DWORD fre_clust;
    FATFS *pfs;
    start = uwTick;
    fres = f_getfree("", &fre_clust, &pfs);
    stop = uwTick;
    uint32_t  check_time = stop - start;
    if (fres != FR_OK)
        Error_Handler();
    SEGGER_RTT_printf(0, "check free time: %u\n", check_time);

    uint32_t total = (uint32_t) ( ((pfs->n_fatent)-2) * ((pfs->csize)/2) ) ;// * 1024; // total volume size in bytes
    uint32_t totalfree = (uint32_t) ( (fre_clust) * ((pfs->csize)/2) ) ;// * 1024; // total volume size in bytes
    //total = (uint32_t) (((pfs->n_fatent)-2) * (pfs->csize)/2) * 1024; // total volume size in bytes
    //totalfree = (uint32_t) ((fre_clust) * (pfs->csize)/2) * 1024; // free space on volume in bytes
    //total = (uint32_t) ((pfs->n_fatent - 2) * pfs->csize * 0.5); // total volume size (kB)
    //totalfree = (uint32_t) (fre_clust * pfs->csize * 0.5); // free space on volume (kB)



    HAL_Delay(1000);
    // test card total/free
    //uint32_t total_test = 
    //uint32_t samsung_pro_32_total = 31256800;
    //uint32_t samsung_pro_32_free = 31256608;
    //uint32_t lexar_512_total = 483313152;
    //uint32_t lexar_512_free = 483312384;

    //if (total != total_test)
    //    Error_Handler();
    //if (totalfree != lexar_512_free)
    //    Error_Handler();

    /*
    // format card
    fres = f_mkfs("", FM_EXFAT, 0, work, sizeof work);
    if (fres != FR_OK)
        Error_Handler();

    // fill 16-kB page with 16-byte string
    for (uint32_t i = 0; i < 1024; i++)
        strcpy(data[i], "0123456789abcde\n");

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

    //success flash
    mem_led_flash(5, 40, 40);
    HAL_Delay(1000);

    // power-on memory module
    mem_pwr_off();
    HAL_SD_MspDeInit(&hsd1);

*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    // toggle mcu led
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5);
    HAL_Delay(200);

    /* toggle mem pwr
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_Delay(3000);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_Delay(3000);
    */

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV8;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
