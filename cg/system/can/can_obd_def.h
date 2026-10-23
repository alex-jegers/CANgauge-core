
#ifndef _CAN_UDS_DEF_H_
#define _CAN_UDS_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif
/**********     INCLUDES        **********/
#include "drivers/can/stm32_canbus.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/**********     DEFINES      **********/

/**********     TYPEDEFS        **********/
typedef struct can_obd_current_data_t can_obd_current_data_t;
struct can_obd_current_data_t
{
    const char* name;                     //For debugging and easier readability.
    const uint8_t pid_code;
    float scale;
    float offset;
    uint8_t data_bytes;     //How many bytes of data are valid (usually 1 or 2).
    uint8_t first_byte;
    int32_t min;
    int32_t max;
    const char* units;
    bool available;
    can_obd_current_data_t** nested;      //Pointer to the nested data array. NULL if not a nested PID.
};

/**********     GLOBAL VARIABLE DECLRATIONS     **********/


/**********		GLOBAL FUNCTION DECLRATIONS		**********/
/**
 * Sets the CAN frame that should be transmitted to get the ECU to return
 * data as per SAE J1979 Service 01. Unused PIDs should be set to 0xCC.
 * Returns true if anything is being transmitted onto the CANbus, false
 * if nothing is being sent.
 */
bool can_obd_set_current_data_query(uint8_t pid1, uint8_t pid2, uint8_t pid3, uint8_t pid4);

/** 
 * @brief Returns the raw value of a PID given the CAN message.
 * If it's not a valid SAE J1979 Service 01 response CAN frame returns 0.
*/
int32_t can_obd_process_raw_data(can_rx_buffer_entry_t* input);

/**
 * @brief Returns the processed value for a PID given the can_uds_current_data_t struct.
 * Uses the last raw value received.
 */
float can_obd_get_processed_current_data(can_obd_current_data_t* data);

/**
 * @brief Returns a current data LUT element. The LUT includes info like the scale and offset needed
 * to be applied to the raw data, which position the first byte is in, how many data bytes to
 * include, etc. This will never directly return a nested element.
 * @returns A pointer to a saej1979_current_data_t element.
 */
can_obd_current_data_t* can_obd_get_current_data_by_pid(uint8_t pid);

/**
 * @brief Returns a current data LUT element. The LUT includes info like the scale and offset needed
 * to be applied to the raw data, which position the first byte is in, how many data bytes to
 * include, etc. Searching by name will check the nested elements as well.
 * @param name The name of the data element being searched for.
 * @returns A pointer to a saej1979_current_data_t element. Returns NULL if the data element with
 * name 'name' couldnt be found.
 */
can_obd_current_data_t* can_obd_get_current_data_by_name(const char* name);

/**
 * @brief Check if a saej1979_current_data_t has been marked as available or not. Available
 * means the car supports this PID.
 * @returns True if the car supports this PID, false if it does not. Return value only valid
 * after CAN UDS has been successfully initialized with can_uds_run and verified with can_uds_is_init.
 */
bool can_obd_get_current_data_availability(can_obd_current_data_t* data);

/**
 * @returns The PID of a current data struct.
 */
uint8_t can_obd_get_current_data_pid(can_obd_current_data_t* data);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  //_CAN_UDS_DEF_H_
