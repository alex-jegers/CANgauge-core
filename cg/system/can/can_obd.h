
#ifndef _CAN_UDS_H_
#define _CAN_UDS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********     INCLUDES        **********/
#include <system/can/can_obd_def.h>
#include "stdbool.h"
#include "stdint.h"
/**********     DEFINES      **********/

/**********     GLOBAL VARIABLE DECLRATIONS     **********/


/**********		GLOBAL FUNCTION DECLRATIONS		**********/
/**
 * @brief: can_obd_run starts a FreeRTOS task internally, the task function named prv_task_can_obd.
 * This task first intializes the low level CAN driver for FDCAN1, sets the baud rate to 500kbps,
 * initializes the CAN transmitter task. It sets up the RX filter to accept SAEJ1979 messages into
 * FIFO1. Next it attempts to communicate with the car over the ISO 15765 network and determine
 * which CAN ID the car uses. If it doesn't respond to either ID it will set the ID to 0 and set the
 * task to be deleted. Before deleting itself it sets the event bit for initialization complete.
 * If it does find a valid ISO 15675 CAN ID it will wait for a CAN frame to be received into FIFO1.
 * When a CAN frame is received into FIFO1 it will process it and store the raw data. The raw data
 * can be retrieved with can_obd_get_raw_ current_data/infotype_data and processed with
 * can_obd_get_processed_current_data.
 * @returns True if the FreeRTOS task was successfully created or already running. False if the FreeRTOS
 * task could not be created (i.e. out of memory).
 */
bool can_obd_run();
bool can_obd_stop(uint32_t block_time_ms);
uint32_t can_obd_get_raw_current_data(uint8_t pid, uint8_t first_byte, uint8_t num_params);
uint32_t can_obd_get_raw_infotype_data(uint8_t pid, uint8_t first_byte, uint8_t num_params);
bool app_can_sniffer_running();			//Delete??
bool can_obd_is_init(uint32_t block_time_ms);

/**
 * @brief Will return zero if there is no UDS on CAN found (i.e. no response
 * was returned in request for PID 0x00). Otherwise returns the ID that is
 * used to query the OBD.
 * @returns See brief.
 */
uint32_t can_obd_get_query_can_id();

/**
 * @brief //Will return zero if there is no UDS on CAN found (i.e. no response
 * was returned in request for PID 0x00). Otherwise returns the ID that was
 * received from the ECU.
 * @returns See brief.
 */
uint32_t can_obd_get_response_can_id();
void can_obd_stop_query();					//Stops all the queries for current data being sent on the bus.
void can_obd_change_pressure_units(const char* units);
void can_obd_change_temperature_units(const char* units);
void can_obd_change_speed_units(const char* units);
void can_obd_change_torque_units(const char* units);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  //_CAN_UDS_H_
