
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
 * app_can_controller_run:
 * desc:
 * 
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
