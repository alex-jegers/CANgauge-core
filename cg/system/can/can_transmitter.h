/**
 * @file can_transmitter.h
 * @brief This is a scheduler for transmitting periodic messages on FDCAN1 (FDCAN2)
 * is not yet implemented. CAN messages can be assigned to a regular or high priority
 * message slot. High-priority handles are checked first each cycle and are always one
 * shot. Regular handles are scanned round-robin from a saved restart position.
 * One-shot messages are sent and freed while regular periodic messages are sent when
 * their period has elapsed. The time between transmissions will never exceed
 * min_time_between_msg_ms that is passed to can_transmit_run.
 */


#ifndef _APP_CAN_TRANSMIT_H_
#define _APP_CAN_TRANSMIT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********     INCLUDES        **********/
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/drivers.h"

/**********     DEFINES      **********/
#define CAN_TRANSMIT_PERIOD_ONE_SHOT        0xFFFF

/**********     TYPEDEFS        **********/
typedef struct 
{
    bool active;
    uint32_t period_ms;     //use CAN_TRANSMIT_PERIOD_ONE_SHOT for a single, non-repeating msg.
    uint32_t last_time_sent;
    can_tx_buffer_entry_t* buf;
}can_transmit_handle_t;


/**********     GLOBAL VARIABLE DECLRATIONS     **********/

/**********		GLOBAL FUNCTION DECLRATIONS		**********/
/**
 * @brief Starts the CAN transmitter task. Does not initialize the low level CAN
 * driver, that must be done before.
 */
bool can_transmit_run(FDCAN_GlobalTypeDef* can, uint32_t min_time_between_msg_ms);
bool can_transmit_stop(uint32_t block_time_ms);

can_transmit_handle_t* can_transmit_create_msg();

/**
 * can_transmit_create_high_priority_msg
 * 		desc: creates an inactive handle with a default period of ONE_SHOT.
 * 				Will get transmitted first on the next cycle after being activated.
 */
can_transmit_handle_t* can_transmit_create_high_priority_msg();
bool can_transmit_set_period(can_transmit_handle_t* hndl, uint32_t period_ms);

/**
 * @brief buf gets copied into CAN TX message RAM and does not need a permanent lifespan. buf can be
 * 	accessed and edited through hndl after a message is created.
 */
bool can_transmit_set_msg_data(can_transmit_handle_t* hndl, can_tx_buffer_entry_t* buf);
bool can_transmit_set_active(can_transmit_handle_t* hndl);
bool can_transmit_set_inactive(can_transmit_handle_t* hndl);
void can_transmit_delete_handle(can_transmit_handle_t* hndl);

TaskHandle_t* can_transmit_get_task_handle();


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  //_APP_CAN_TRANSMIT_H_
