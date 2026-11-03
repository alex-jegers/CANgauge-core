/**
 * @file template.h
 * @brief 
 * @date yyyy-mm-dd
 * 
 */
#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********     INCLUDES        **********/
#include "FreeRTOS.h"
#include "task.h"

/**********     TYPEDEFS         **********/

/**********     DEFINES      **********/

/**********     GLOBAL VARIABLE DECLRATIONS     **********/

/**********		GLOBAL FUNCTION DECLRATIONS		**********/
/**
 * @brief
 */
void example_basic_can_init(TaskHandle_t task_to_notify);
void example_basic_can_run();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  //_TEMPLATE_H_
