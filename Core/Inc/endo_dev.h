/* Development code for Endurologger */

#ifndef __ENDO_DEV_H__
#define __ENDO_DEV_H__

//includes
#include "stdint.h"

// function prototypes

/**
  * @brief  Flash the LED n-times with adjustable non-symmetrical delay.
  * @note   This function flashes the LED on and then off with a symmetrical
  *         on/off delay.
  * @param  n is the number of times to flash the LED.
  * @param  on_time is the time on for each flash.
  * @param  off_time is the time off for each flash.
  * @retval None
  */
void flash(uint32_t, uint32_t, uint32_t);

void loop_delay (uint32_t);
void sd_test(void);
void sd_mount(void);
void sd_check_free(void);
void sd_test_write(void);

#endif /* __ENDO_DEV_H__ */