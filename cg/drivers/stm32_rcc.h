/**
 * @file stm32_rcc.h
 * @author Alex Jegers
 * @brief Low level control of all RCC related functions for the STM32H745.
 * @version 0.1
 * @date 2026-09-09
 * 
 */

#ifndef INC_STM32_RCC_H_
#define INC_STM32_RCC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h745xx.h"
#include "system/system_mem.h"

#define F_CPU_M7			480000000
#define F_CPU_M4			240000000
#define RCC_F_TIMERS_CLK	240000000

/* Kernel clock select bit masks. */
#define RCC_D1CCIPR_FMCSEL_PLL1Q		0x1
#define RCC_D1CCIPR_FMCSEL_PLL2R		0x2
#define RCC_D2CCIP1R_SPI45SEL_PLL3Q		0x2

/**
 * @brief Enables the PLL1 and waits for it to be ready.
 * 
 */
#define rcc_enable_pll_1()       RCC->CR |= RCC_CR_PLL1ON; while ((RCC->CR & RCC_CR_PLL1RDY) == 0){} 
/**
 * @brief Enables the PLL2 and waits for it to be ready.
 * 
 */
#define rcc_enable_pll_2()       RCC->CR |= RCC_CR_PLL2ON; while ((RCC->CR & RCC_CR_PLL2RDY) == 0){} 
/**
 * @brief Enables the PLL3 and waits for it to be ready.
 * 
 */
#define rcc_enable_pll_3()       RCC->CR |= RCC_CR_PLL3ON; while ((RCC->CR & RCC_CR_PLL3RDY) == 0){} 

/**
 * @brief Disables the PLL1.
 * 
 */
#define rcc_disable_pll_1()       RCC->CR &= ~(RCC_CR_PLL1ON)

/**
 * @brief Disables the PLL2.
 * 
 */
#define rcc_disable_pll_2()       RCC->CR &= ~(RCC_CR_PLL2ON)
/**
 * @brief Disables the PLL3.
 * 
 */
#define rcc_disable_pll_3()       RCC->CR &= ~(RCC_CR_PLL3ON)

/**
 * @brief Enables the PLL1 P output.
 * 
 */
#define rcc_enable_pll_1p()	    RCC->PLLCFGR |= RCC_PLLCFGR_DIVP1EN
/**
 * @brief Enables the PLL1 Q output.
 * 
 */
#define rcc_enable_pll_1q()     RCC->PLLCFGR |= RCC_PLLCFGR_DIVQ1EN
/**
 * @brief Enables the PLL1 R output.
 * 
 */
#define rcc_enable_pll_1r()     RCC->PLLCFGR |= RCC_PLLCFGR_DIVR1EN
/**
 * @brief Enables the PLL2 P output.
 * 
 */
#define rcc_enable_pll_2p()     RCC->PLLCFGR |= RCC_PLLCFGR_DIVP2EN
/**
 * @brief Enables the PLL2 Q output.
 * 
 */ 
#define rcc_enable_pll_2q()     RCC->PLLCFGR |= RCC_PLLCFGR_DIVQ2EN
/**
 * @brief Enables the PLL2 R output.
 * 
 */
#define rcc_enable_pll_2r()     RCC->PLLCFGR |= RCC_PLLCFGR_DIVR2EN
/**
 * @brief Enables the PLL3 P output.
 * 
 */
#define rcc_enable_pll_3p()     RCC->PLLCFGR |= RCC_PLLCFGR_DIVP3EN
/**
 * @brief Enables the PLL3 Q output.
 * 
 */
#define rcc_enable_pll_3q()     RCC->PLLCFGR |= RCC_PLLCFGR_DIVQ3EN
/**
 * @brief Enables the PLL3 R output.
 * 
 */
#define rcc_enable_pll_3r()     RCC->PLLCFGR |= RCC_PLLCFGR_DIVR3EN

#define rcc_disable_pll_1p()	RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVP1EN | RCC_PLL1DIVR_R1)
#define rcc_disable_pll_1q()    RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVQ1EN | RCC_PLL1DIVR_R1)
#define rcc_disable_pll_1r()    RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVR1EN | RCC_PLL1DIVR_R1)
#define rcc_disable_pll_2p()    RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVP2EN | RCC_PLL1DIVR_R1)
#define rcc_disable_pll_2q()    RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVQ2EN | RCC_PLL1DIVR_R1)
#define rcc_disable_pll_2r()    RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVR2EN | RCC_PLL1DIVR_R1)
#define rcc_disable_pll_3p()    RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVP3EN | RCC_PLL1DIVR_R1)
#define rcc_disable_pll_3q()    RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVQ3EN | RCC_PLL1DIVR_R1)
#define rcc_disable_pll_3r()	RCC->PLLCFGR &= ~(RCC_PLLCFGR_DIVR3EN | RCC_PLL1DIVR_R1)

#define rcc_enable_lsi()			RCC->CSR |= RCC_CSR_LSION; while ((RCC->CSR & RCC_CSR_LSIRDY) == 0) {}

#define rcc_reset_usb2otg()			RCC->AHB1RSTR |= RCC_AHB1RSTR_USB2OTGFSRST
#define rcc_clr_reset_usb2otg()		RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_USB2OTGFSRST)

typedef enum 
{
    PER_CK_SRC_HSI,
    PER_CK_SRC_CSI,
    PER_CK_SRC_HSE,
}per_ck_src_t;

typedef enum
{
    RCC_SYS_CK_HSI,
    RCC_SYS_CK_CSI,
    RCC_SYS_CK_HSE,
    RCC_SYS_CK_PLL1_P
}rcc_sys_ck_t;

void rcc_main_clock_config();
void rcc_init_systick();
/**
 * rcc_enable_csi:
 * desc: turns on the csi oscillator and waits for it to be ready.
 *  Waits for the clock to become stable before returning.
 */
void rcc_enable_csi();
/**
 * rcc_select_per_ck:
 * desc: selects the source for per_ck.
 */
void rcc_select_per_ck(per_ck_src_t ck);
void rcc_c2_clock_config();

/**
 * @brief Sets the initial, overall multiplier and the dividers for the PLL1 
 *      outputs (p, q, r). 
 *      Ex: PLL1_R frequency(Hz) = input frequency * mult / div_r.
 *      The output frequency of the VCO (input freq. * mult) needs to be in
 *      compliance with PLLxVCOSEL setting (assumes PLLxVCOSEL is 0). 
 *      PLL needs to be disabled.
 *      Assumes an 8MHz input.
 *      The function will return 0 if anything not allowed is attemped with 
 *      any of the passed params.
 *      NOTE THAT - PLL1R, div 1 is not allowed, min value of 2.
 *          PLL1P, odd division values are not allowed except 1.
 *          PLL multipliers, minimum value of 4.
 * @param mult: how much to multiply the input frequency by.
 * @param div_x: how much to divide the VCO output by.
 * @return 0: if config settings are NOT valid and were not changed.
 *      non-zero: config settings are valid and were changed. 
 */
int8_t rcc_config_pll_1(uint16_t mult, uint8_t div_p, uint8_t div_q, uint8_t div_r);
/**
 * @brief sets the initial, overall multiplier and the dividers for the PLL2 
 *          outputs (p, q, r). See rcc_config_pll_1 comment for more details.
 * @param mult: how much to multiply the input frequency by.
 * @param div_x: how much to divide the VCO output by.
 * @return 0: if config settings are NOT valid and were not changed.
 *      non-zero: config settings are valid and were changed.
 */
int8_t rcc_config_pll_2(uint16_t mult, uint8_t div_p, uint8_t div_q, uint8_t div_r);

/**
 * @brief sets the initial, overall multiplier and the dividers for the PLL3
 *          outputs (p, q, r). See rcc_config_pll_1 comment for more details.
 * @param mult: how much to multiply the input frequency by.
 * @param div_x: how much to divide the VCO output by.
 * @return 0: if config settings are NOT valid and were not changed.
 *      Non-zero: config settings are valid and were changed. 
 */
int8_t rcc_config_pll_3 (uint16_t mult, uint8_t div_p, uint8_t div_q, uint8_t div_r);

/**
 * @brief Fully shuts down all PLLs and their outputs.
 */
void rcc_disable_all_pll();

/**
 * @brief Sets sys_ck according to ck_src. Does not return until the clock is stable
 *      and running.
 */
void rcc_set_sys_ck(rcc_sys_ck_t ck_src);

SYS_MEM_REGION_RAM_EXE void rcc_sw_reset();
void rcc_set_systick_reload(uint32_t reload);



#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* INC_STM32_RCC_H_ */
