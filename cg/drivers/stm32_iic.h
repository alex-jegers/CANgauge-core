/*
 * stm32_iic.h
 *
 *  Created on: Apr 14, 2024
 *      Author: awjpp
 */

#ifndef DRIVERS_STM32_IIC_H_
#define DRIVERS_STM32_IIC_H_


#include "stm32h745xx.h"
#include "stdbool.h"
#include "stddef.h"


typedef enum 
{
	I2C_CLK_100K,
	I2C_CLK_400K,
	I2C_CLK_1M,
}i2c_clk_speed_t;

typedef enum
{
	I2C_INTERNAL_ADDR_8_BIT,
	I2C_INTERNAL_ADDR_16_BIT,
}i2c_internal_addr_t;

typedef enum
{
	I2C_EXIT_CODE_NACK 				= 1,
	I2C_EXIT_CODE_STOP				= 2,
	I2C_EXIT_CODE_TC				= 3,
	I2C_EXIT_CODE_TIMEOUT			= 4,
	I2C_EXIT_CODE_ERR				= 5,
	I2C_EXIT_CODE_ARB_LOST			= 6,
}i2c_exit_code_t;

void i2c_init(I2C_TypeDef* i2c);
void i2c_set_clk_speed(I2C_TypeDef* i2c, i2c_clk_speed_t clk_frq);
void i2c_enable(I2C_TypeDef* i2c);
void i2c_disable(I2C_TypeDef* i2c);
void i2c_disable_analog_filt(I2C_TypeDef* i2c);
void i2c_disable_clk_stretch(I2C_TypeDef* i2c);
void i2c_enable_timeout_detection(I2C_TypeDef* i2c);

/**
 * @brief Read data from an I2C device.
 * @param i2c Which I2C port (use I2C4 for CANgauge).
 * @param slave_addr The device address of the targeted slave.
 * @param internal_addr The address internal to the slave to read from.
 * @param internal_addr_type Either 8 bit or 16 bit. Use I2C_INTERNAL_ADDR_8_BIT or I2C_INTERNAL_ADDR_16_BIT
 * @param data A pointer to a uint8_t array to read the data into.
 * @param num_bytes The number of bytes to read into "data".
 * @param auto_stop Whether or not to send a stop bit after sending the internal addr. True to send a stop, false to stretch the clock.
 * @return Returns type i2c_exit_code_t. I2C_EXIT_CODE_TC for transfer complete.
 */
i2c_exit_code_t i2c_read(I2C_TypeDef* i2c, uint8_t slave_addr, uint16_t internal_addr, i2c_internal_addr_t internal_addr_type,
					uint8_t* data, uint8_t num_bytes, bool auto_stop);

/**
 * @brief Write data to an I2C device.
 * @param i2c Which I2C port (use I2C4 for CANgauge).
 * @param slave_addr The device address of the targeted slave.
 * @param internal_addr The address internal to the slave to read from.
 * @param internal_addr_type Either 8 bit or 16 bit. Use I2C_INTERNAL_ADDR_8_BIT or I2C_INTERNAL_ADDR_16_BIT
 * @param data A pointer to a uint8_t array containing the data to write.
 * @param num_bytes The number of bytes to write from "data".
 * @param auto_stop Whether or not to stretch the clock after num_bytes are transfered. True = send a stop bit. False = stretch the clock.
 * @return Returns type i2c_exit_code_t. I2C_EXIT_CODE_TC for transfer complete.
 */
i2c_exit_code_t i2c_write(I2C_TypeDef* i2c, uint8_t slave_addr, uint16_t internal_addr, i2c_internal_addr_t internal_addr_type,
					uint8_t* data, uint8_t num_bytes, bool auto_stop);

/**
 * i2c_bus_reset:
 * 		desc: toggles the clock pin 9 times in an attempt to reset a stuck low data line.
 */
void i2c_bus_reset(I2C_TypeDef* i2c);

int8_t i2c_probe(I2C_TypeDef* i2c);		//Returns the address of a devices on the bus that acks.

uint32_t i2c_status(I2C_TypeDef* i2c);
void i2c_clear_status(I2C_TypeDef* i2c);

#endif /* DRIVERS_STM32_IIC_H_ */
