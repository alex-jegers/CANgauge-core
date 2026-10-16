/*
 * stm32_canbus.h
 *
 *  Created on: Mar 9, 2024
 *      Author: awjpp
 */

#ifndef SRC_DRIVERS_STM32_CANBUS_H_
#define SRC_DRIVERS_STM32_CANBUS_H_

/***********	INCLUDES	************/
#include <include/stm32h745xx.h>
#include "stm32_canbus_def.h"
#include "stdbool.h"

/***********	TYPEDEFS	************/

/***********	DEFINES		************/

/***********	VARIABLE DEFINITIONS	************/

/***********	GLOBAL FUNCTION DECLARATIONS	************/
/**
 * @brief Sets up the IO pins. Sets up the filter to store non-matching standard and extended
 * frames in FIFO0. Configures the address pointers to the standard and extended filter
 * lists based on CAN1_xxx_ID_FILTER_ADDR macro. Configures the buffer address pointers
 * to the RX and TX FIFOs. Clears interrupts.
 */
void can_init();

/**
 * @brief Disables interrupts. Turns off clocks, resets both peripheral blocks
 * (FDCAN1 and 2).
 */
void can_deinit();

/**
 * @brief Sets the INIT and CCE bits so the CAN registers can be modified.
 */
void can_stop(FDCAN_GlobalTypeDef* canbus);

/**
 * @brief Clears the INIT and CCE bits locking CAN registers.
 * @attention For some reason that I haven't been able to find in ST documentation
 * FDCAN1 needs to be initialized for FDCAN2 to work after it gets initialized.
 * After initializing FDCAN2, FDCAN1 can be uninitialized.
 */
void can_run();

/**
 * @brief Sets the values of the NBTP registers, assumes a 48MHz kernel clock.
 */
void can_set_baud_rate(FDCAN_GlobalTypeDef* canbus, can_baud_rate_t baud_rate);

/**
 * @brief Samples the bus activity in listen only mode and will
 * return the baud rate assuming it's an option in can_baud_rate_t.
 */
can_baud_rate_t can_get_baud_rate(FDCAN_GlobalTypeDef* canbus);

/**
 * @brief Returns whatever the last error code stored in the PSR register was.
 * Saves it's state even after LEC has switched to NO_CHANGE.
 */
can_error_code_t can_get_last_error_code(FDCAN_GlobalTypeDef* canbus);

uint32_t can_get_rx_error_counter(FDCAN_GlobalTypeDef* canbus);
uint32_t can_get_tx_error_counter(FDCAN_GlobalTypeDef* canbus);

/**
 * @brief Adds a TX buffer entry to the CAN message RAM TX buffers.
 * @param canbus Either FDCAN1 or FDCAN2.
 * @param new_message A pointer to the tx_buffer_entry_t that will be copied to message RAM.
 * @param index What index in message RAM to place the buffer (will overwrite previous data written to said index).
**/
int8_t can_add_tx_buffer(FDCAN_GlobalTypeDef* canbus, can_tx_buffer_entry_t* new_message, uint8_t index);

/**
 * @brief Returns a pointer to a TX buffer specified by index.
 * @return A pointer to a TX buffer entry in CAN msg RAM memory.
 */
can_tx_buffer_entry_t* can_get_tx_buffer(FDCAN_GlobalTypeDef* canbus, uint8_t index);

/**
 * @brief Sets the TXBAR bit requesting a message be sent.
 */
void can_tx(FDCAN_GlobalTypeDef* canbus, uint8_t index);

/**
 * @brief Checks if there are any unread messages in FIFO0.
 * @return True if there is a message(s) in FIFO0.
 */
bool can_check_for_rx_fifo0(FDCAN_GlobalTypeDef* canbus);

/**
 * @brief Copys the next message in the FIFO into the message param.
 * @param canbus FDCAN1 or 2.
 * @param message A pointer to a CAN RX buffer entry that the incoming message will be copied into.
 * @returns The number of items remaining in the FIFO.
 */
uint8_t can_read_from_fifo0(FDCAN_GlobalTypeDef* canbus, can_rx_buffer_entry_t* message);

/**
 * @brief Checks if there are any unread messages in FIFO1.
 * @return True if there is a message(s) in FIFO1.
 */
bool can_check_for_rx_fifo1(FDCAN_GlobalTypeDef* canbus);

/**
 * @brief Copys the next message in the FIFO into the message param.
 * @param canbus FDCAN1 or 2.
 * @param message A pointer to a CAN RX buffer entry that the incoming message will be copied into.
 * @returns The number of items remaining in the FIFO.
 */
uint8_t can_read_from_fifo1(FDCAN_GlobalTypeDef* canbus, can_rx_buffer_entry_t* message);

/**
 * @brief You can't read the ID directly from the can_rx_buffer_entry_t bc it's shifted 17 bits if the
 * ID is standard and not extended. This function takes care of that and will check if it's a std or xtd
 * ID and do the shift if necessary and then return the ID.
 * @param message The can_rx_buffer_entry_t to extract the CAN ID from.
 * @return The CAN ID of the message.
 */
uint32_t can_get_can_id(can_rx_buffer_entry_t* message);

/**
 * @brief Sets the data and data length code of a TX buffer.
 * @param message Pointer to the TX buffer who's data and DLC we're setting.
 * @param dlc The data length code (size of data_arr in bytes).
 */
void can_set_data(can_tx_buffer_entry_t* message, const uint8_t* data_arr, uint8_t dlc);

/**
 * @brief Sets the CAN ID of a TX message. You can't directly write the bits without shifting them
 * if it's a standard ID so use this instead and you dont have to worry about shifting the bits.
 * This function will set the ID and the ID type on it's own.
 * @param message Pointer to the TX buffer entry who's ID to set.
 * @param can_id The CAN ID, not shifted.
 * @param id_type CAN_ID_STD or CAN_ID_XTD
 */
void can_set_can_id(can_tx_buffer_entry_t* message, uint32_t can_id, can_id_t id_type);

/**
 * @brief Sets a standard filter by index. Will do nothing if the index is greater than the amount of filter elements.
 */
void can_set_std_id_filter(FDCAN_GlobalTypeDef* canbus, uint8_t index, can_std_id_filter_t* filter);

/**
 * @brief Sets an extended filter by index. Will do nothing if the index is greater than the amount of filter elements.
 */
void can_set_ext_id_filter(FDCAN_GlobalTypeDef* canbus, uint8_t index, can_ext_id_filter_t* filter);


void can_assign_rx_rf0n_cb(FDCAN_GlobalTypeDef* canbus, void (*func)());		//Assign a callback for a new message in FIFO 0 interrupt.
void can_assign_rx_rf1n_cb(FDCAN_GlobalTypeDef* canbus, void (*func)());		//Assign a callback for a new message in FIFO 1 interrupt.
void can_assign_rx_rf0f_cb(FDCAN_GlobalTypeDef* canbus, void (*func)());		//RX FIFO0 full interrupt.

void can_enable_rx_rf0n_interrupt(FDCAN_GlobalTypeDef* canbus);     //Enable new message in FIFO 0 interrupt.
void can_enable_rx_rf1n_interrupt(FDCAN_GlobalTypeDef* canbus);     //Enable new message in FIFO 1 interrupt.





#endif /* SRC_DRIVERS_STM32_CANBUS_H_ */
