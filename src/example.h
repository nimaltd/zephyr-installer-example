/**
 * @file        example.h
 * @brief       A small library made to try lib-installer on: a period that never blocks.
 * @version     1.4.0
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
 * @note        Its settings are Kconfig symbols, set in prj.conf:
 *              CONFIG_EXAMPLE_PERIOD_MS and CONFIG_EXAMPLE_GREETING.
 */

#ifndef EXAMPLE_H
#define EXAMPLE_H

/*
 * ****************************************************************************************************
 * Includes
 * ****************************************************************************************************
*/

#include <stdbool.h>
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
 * @brief One period. Declare one for each thing that repeats, and hand it to example_init().
 */
typedef struct
{
    uint32_t start; /**< Uptime in ms when the period under way began. */
    uint32_t count; /**< Times example_due() has said yes.            */

} example_t;

/*
 * ****************************************************************************************************
 * Public function prototypes
 * ****************************************************************************************************
*/

/*****************************************************************************************************/
/**
 * @brief Start a period of CONFIG_EXAMPLE_PERIOD_MS from now.
 */
void example_init(example_t *handle);

/*****************************************************************************************************/
/**
 * @brief Whether the period has passed. Returns at once, and starts the next one when it has.
 */
bool example_due(example_t *handle);

/*****************************************************************************************************/
/**
 * @brief How many times example_due() has said yes.
 */
uint32_t example_count(const example_t *handle);

/*****************************************************************************************************/
/**
 * @brief This device's own ID, from the example_id.h the make-id step made, or 0.
 */
uint32_t example_id(void);

/*****************************************************************************************************/
/**
 * @brief Print CONFIG_EXAMPLE_GREETING with printk, how many periods have passed, and the device ID.
 */
void example_hello(const example_t *handle);

#ifdef __cplusplus
}
#endif

#endif /* EXAMPLE_H */
