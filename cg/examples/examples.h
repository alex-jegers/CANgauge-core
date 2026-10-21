/**
 * @file examples.h
 * @brief Parent header to include all the example headers.
 * @date 2026-10-20
 * 
 */
#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#ifdef __cplusplus
extern "C" {
#endif


#define EXAMPLES_BUILD_EXAMPLES		1
/**********     INCLUDES        **********/
#if EXAMPLES_BUILD_EXAMPLES

#include "shift_light_example.h"

#endif //EXAMPLES_BUILD_EXAMPLES

/**
 * @brief
 */



#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  //_EXAMPLE_H_
