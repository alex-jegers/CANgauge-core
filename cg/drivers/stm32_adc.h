/**
 * @file cg_adc.h
 * @author Alex Jegers
 * @brief 
 * @date 2026-09-13
 * 
 */
#ifndef _CG_ADC_H_
#define _CG_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********     INCLUDES        **********/
#include "stm32h745xx.h"
#include <stddef.h>

/**********     DEFINES      **********/

typedef enum
{
	ADC_RESOLUTION_16_BIT 	= 0x00,
	ADC_RESOLUTION_14_BIT 	= 0x05,
	ADC_RESOLUTION_12_BIT 	= 0x06,
	ADC_RESOLUTION_10_BIT 	= 0x03,
	ADC_RESOLUTION_8_BIT 	= 0x07,
}adc_resolution_t;

typedef enum
{
	ADC_SAMPLE_TIME_1_CYCLES	= 0x00,
	ADC_SAMPLE_TIME_2_CYCLES,
	ADC_SAMPLE_TIME_8_CYCLES,
	ADC_SAMPLE_TIME_16_CYCLES,
	ADC_SAMPLE_TIME_32_CYCLES,
	ADC_SAMPLE_TIME_64_CYCLES,
	ADC_SAMPLE_TIME_387_CYCLES,
	ADC_SAMPLE_TIME_810_CYCLES,
}adc_sample_time_t;

typedef enum
{
	ADC_PRESCALER_DIV_1		= 0x00,
	ADC_PRESCALER_DIV_2,
	ADC_PRESCALER_DIV_4,
	ADC_PRESCALER_DIV_6,
	ADC_PRESCALER_DIV_8,
	ADC_PRESCALER_DIV_10,
	ADC_PRESCALER_DIV_12,
	ADC_PRESCALER_DIV_16,
	ADC_PRESCALER_DIV_32,
	ADC_PRESCALER_DIV_64,
	ADC_PRESCALER_DIV_128,
	ADC_PRESCALER_DIV_256,
}adc_prescaler_t;

typedef enum
{
	ADC_INT_FLAG_ADC_RDY				= 0x01,
	ADC_INT_FLAG_END_OF_SAMPLING		= 0x02,
	ADC_INT_FLAG_END_OF_CONVERSION		= 0x04,
	ADC_INT_FLAG_END_OF_REG_SEQ			= 0x08, //End of regular sequence.
	ADC_INT_FLAG_OVERRUN				= 0x10,
	ADC_INT_FLAG_INJ_END_OF_CONVERSION	= 0x20,
	ADC_INT_FLAG_INJ_END_OF_SEQ			= 0x40,
	ADC_INT_FLAG_WATCHDOG_1				= 0x80,
	ADC_INT_FLAG_WATCHDOG_2				= 0x100,
	ADC_INT_FLAG_WATCHDOG_3				= 0x200,
	ADC_INT_FLAG_INJ_QUEUE_OVERFLOW		= 0x400,
	ADC_INT_FLAG_LDORDY					= 0x1000,
	ADC_INT_FLAG_ALL					= 0x17FF,
}adc_int_flag_t;

/// Refer to cg_rcc API for clock frequency control.
typedef enum
{
	ADC_CK_SRC_PLL2_P,
	ADC_CK_SRC_PLL3_R,
	ADC_CK_SRC_PER_CK,
}adc_ck_src_t;

/**********     GLOBAL VARIABLE DECLRATIONS     **********/

/**********		GLOBAL FUNCTION DECLRATIONS		**********/
/**
 * @brief Select the kernel clock source and enable the AHB clock for ADC 1 & 2.
 * @todo Only ADC 1 & 2 clock initialization is implemented, needs to
 * support ADC 3 which is initialized separately.
 * 
 * @param adc Which instance of the ADC to apply the changes to (currently
 * unimplemented, clock is only activated for ADC 1&2).
 * 
 * @param ck_src Which clock to use as the kernel clock.
 */
void adc_init_clk(ADC_TypeDef* adc, adc_ck_src_t ck_src);

/**
 * @brief Goes through the ADC initialization process. Waits in a while
 * loop for the LDO regulator ready bit to be set.
 * 
 * @param adc The instance of ADC to enable.
 */
void adc_enable(ADC_TypeDef* adc);

/**
 * @brief Sets the ADC disable bit. 
 * @todo Check if this should to set the voltage reg disable and deep power enable bits or not.
 * @param adc The instance of ADC to disable.
 */
void adc_disable(ADC_TypeDef* adc);

/**
 * @brief starts an ADC conversion - the ADC must be enabled.
 * @param adc Which ADC instance to start (1,2, or 3).
 */
void adc_start_conversion(ADC_TypeDef* adc);

/**
 * @brief Clears the ADSTART bit.
 * @todo Check if this should be rather setting the ADSTP bit, a 
 * quick review makes me think that this currently isn'tdoing anything.
 * 
 * @param adc The ADC instance to stop. 
 */
void adc_stop_conversion(ADC_TypeDef* adc);

/**
 * @brief Sets the continuous mode bit so conversions will be automatically started
 * after the previous one completes.
 * 
 * @param adc The instance of ADC to put into continuous mode.
 */
void adc_set_continuous_mode(ADC_TypeDef* adc);

/**
 * @brief Sets the resolution of the ADC.
 * @param adc The ADC instance of which resolution is being set.
 * @param res The resolution. Options are 8,10,12,14, and 16.
 */
void adc_set_resolution(ADC_TypeDef* adc, adc_resolution_t res);

/**
 * @brief Enables a watchdog window.
 * 
 * @param adc The instance of ADC to enable the watchdog for.
 */
void adc_enable_watchdog_1(ADC_TypeDef* adc);

/**
 * @brief Specifies which ADC channel to apply the watchdog to. 
 * @param adc Pointer to an ADC instance.
 * @param channel Number 0 thru 19 corresponding with adc channel number.
 */
void adc_select_watchdog_1_channel(ADC_TypeDef* adc, uint8_t channel);

/**
 * @brief Sets the lower threshold of the watchdog window adjusted for the
 * resolution setting.
 * 
 * @todo Make this trim the threshold value based on the resolution setting.
 * @param adc The ADC instance to set the threshold for.
 * @param threshold The threshold value in counts.
 */
void adc_set_watchdog_1_low_threshold(ADC_TypeDef* adc, uint32_t threshold);

/**
 * @brief Sets the upper threshold of the watchdog window adjusted for the
 * resolution setting.
 * 
 * @todo Make this trim the threshold value based on the resolution setting.
 * @param adc The ADC instance to set the threshold for.
 * @param threshold The threshold value in counts.
 */
void adc_set_watchdog_1_high_threshold(ADC_TypeDef* adc, uint32_t threshold);

/**
 * @brief Sets the sample time in ADC clock cycles. Discrete values selected with the adc_sample_time_t enum.
 * @param adc The ADC instance to set the sample time for.
 * @param sample_time The sample time to set.
 * @param channel The channel number as an integer between 0 and 19.
 */
void adc_set_sample_time(ADC_TypeDef* adc, adc_sample_time_t sample_time, uint8_t channel);

/**
 * @brief Sets the channel for the next conversion. Sets the preselection
 * bits then sets the channel first in the regular sequence register. 
 * Does not currently support more than 1 channel to be queued in the regular sequence 
 * register.
 * 
 * @param adc The ADC instance.
 * @param channel The channel number (integer value 0-19).
 */
void adc_set_channel(ADC_TypeDef* adc, uint8_t channel);

/**
 * @brief Returns the conversion result from the last converted regular channel.
 * 
 * @return Returns conversion result from the last converted regular channel.
 */
uint32_t adc_get_conversion(ADC_TypeDef* adc);

/** 
 * @brief Used to check the status of an interrupt or status bit.
 * @param adc The ADC instance.
 * @param interrupt The interrupt to check the status of (see adc_int_flag_t).
 * @return Returns zero if the bit is not set. returns non-zero if the bit is set.
 */
uint32_t adc_get_interrupt(ADC_TypeDef* adc, adc_int_flag_t interrupt);

/**
 * @brief Clears an interrupt.
 * @param adc The ADC instance.
 * @param interrupt The interrupt to clear (see adc_int_flag_t).
 */
void adc_clear_interrupt(ADC_TypeDef* adc, adc_int_flag_t interrupt);

/**
 * @brief Enables an ADC interrupt.
 * 
 * @param adc The ADC instance.
 * @param interrupt The interrupt to enable.
 */
void adc_enable_interrupt(ADC_TypeDef* adc, adc_int_flag_t interrupt);

/**
 * @brief Disables an ADC interrupt.
 * 
 * @param adc The ADC instance.
 * @param interrupt The interrupt to disable.
 */
void adc_disable_interrupt(ADC_TypeDef* adc, adc_int_flag_t interrupt);

/**
 * @brief Sets the prescaler of the ADC1 and 2 clock. This divides the kernel clock.
 * 
 * @param prescaler How much to divide the clock by.
 */
void adc12_set_clock_prescaler(adc_prescaler_t prescaler);

/**
 * @brief Enables interrupts in the NVIC for ADC 1 & 2.
 * 
 */
void adc12_enable_nvic_interrupts();
void adc12_disable_nvic_interrupts();
void adc12_set_int_handler(void (*func)());

void adc3_set_int_handler(void (*func)());
void adc3_enable_nvic_interrupts();
void adc3_disable_nvic_interrupts();



#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  //_STM32_ADC_H_
