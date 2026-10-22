/**
 * @file shift_light_example.h
 * @brief An example program that makes a F1/rally inspired shift light using the RPM PID from UDS.
 * @date 2026-10-20
 * 
 */
#ifndef _SHIFT_LIGHT_EXAMPLE_H_
#define _SHIFT_LIGHT_EXAMPLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********     INCLUDES        **********/

/**********     TYPEDEFS         **********/

/**********     DEFINES      **********/

/**********     GLOBAL VARIABLE DECLRATIONS     **********/

/**********		GLOBAL FUNCTION DECLRATIONS		**********/
/**
 * @brief Initializes the UI and the CAN for a shift light. Sets the CAN UDS query.
 * Call this in application_task initialization section.
 */
void shift_light_example_init();

/**
 * @brief The shift light application. Call this in the task loop with a 15-30ms delay.
 *
 */
void shift_light_example_run();


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  //_SHIFT_LIGHT_EXAMPLE_H_
