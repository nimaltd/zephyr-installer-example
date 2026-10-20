/**
 * @file        example_uptime.h
 * @brief       How long the board has been running. The uptime option of the example library.
 * @version     1.1.0
 *
 * @author      Nima Askari (NimaLTD)
 * @email       nima.askari@gmail.com
 * @github      https://www.github.com/nimaltd
 * @linkedin    https://www.linkedin.com/in/nimaltd
 * @youtube     https://www.youtube.com/@nimaltd
 * @instagram   https://instagram.com/github.nimaltd
 *
 * @copyright   (c) 2026 Nima Askari (NimaLTD)
 *              SPDX-License-Identifier: Apache-2.0
 *              See LICENSE.md in the project root for the full license text.
 *
 * @note        Installed with --with uptime, and taken out with --without uptime.
 */

#ifndef EXAMPLE_UPTIME_H
#define EXAMPLE_UPTIME_H

/*
 * ****************************************************************************************************
 * Includes
 * ****************************************************************************************************
*/

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * ****************************************************************************************************
 * Types
 * ****************************************************************************************************
*/

/*****************************************************************************************************/
/**
 * @brief Time since start up, in hours, minutes and seconds.
 */
typedef struct
{
    uint32_t hours;   /**< Whole hours, which keep counting past 24. */
    uint8_t  minutes; /**< 0 to 59.                                  */
    uint8_t  seconds; /**< 0 to 59.                                  */

} example_uptime_t;

/*
 * ****************************************************************************************************
 * Public function prototypes
 * ****************************************************************************************************
*/

/*****************************************************************************************************/
/**
 * @brief How long the board has been running.
 */
example_uptime_t example_uptime(void);

#ifdef __cplusplus
}
#endif

#endif /* EXAMPLE_UPTIME_H */
