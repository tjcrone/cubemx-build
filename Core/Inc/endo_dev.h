/* Development code for Endurologger */

#ifndef __ENDO_DEV_H__
#define __ENDO_DEV_H__

//includes
#include "string.h"
#include "deadbeef.h"

// function prototypes
void flash(uint32_t, uint32_t);
void loop_delay (uint32_t);
void sd_test(void);
void sd_mount(void);
void sd_check_free(void);

#endif /* __ENDO_DEV_H__ */