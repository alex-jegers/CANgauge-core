
#ifndef _CANGAUGE_H_
#define _CANGAUGE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********     INCLUDES        **********/
/**
 * The FreeRTOS stack.
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

/**
 * High level access to hardware specific features. Things like the on board LED,
 * the LCD backlight, the CAN transceivers enable pins, initializing the FPU and
 * cache are handled here. The cache can be enabled or disabled with a macro.
 */
#include "system/cg_system.h"

/**
 * This header includes tools to configure memory including assigning variables
 * explicitly to certain memory regions with section attributes defined as macros,
 * file system sizes and sectors, creating and mounting file systems, and writing
 * to flash sectors.
 */
#include "system/system_mem.h"

/**
 * This brings the touch screen and display related stuff.
 */
#include "lvgl_port/lvgl_port_def.h"

/*
 * Stuff to help handle errors and aid in debugging (message boxes, etc.).
 * Depends on LVGL.
 */
#include "system/error_handler.h"

/**
 * Generic UI functions to speed things up, depends on LVGL.
 */
#include "ui_helpers/ui_helpers.h"

/**
 * FatFS file system functions and types.
 */
#include "file_system/fatfs/ff.h"
#include "file_system/filesys_helpers.h"

/*
 * Control the USB connection. Depends on FatFS.
 */
#include "system/usb_task.h"

/**
 * For control of the CAN bus.
 */
#include "system/can/can_transmitter.h"
#include "system/can/can_uds.h"

/**
 * Hardware level drivers. Should depend on nothing except the STM32H7 and
 * CMSIS macros but the IIC driver uses FreeRTOS, this will be removed
 * and implemented somehow else eventually.
 */
#include "drivers/drivers.h"

/**********     TYPEDEFS         **********/

/**********     DEFINES      **********/

/**********     GLOBAL VARIABLE DECLRATIONS     **********/

/**********		GLOBAL FUNCTION DECLRATIONS		**********/




#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  //_CANGAUGE_H_
