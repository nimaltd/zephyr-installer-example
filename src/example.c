/**
 * @file        example.c
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
 */

/*
 * ****************************************************************************************************
 * Includes
 * ****************************************************************************************************
*/

#include "example.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/printk.h>

/* The device's own ID, in the example_id.h the make-id step made in the
   project, when it was run. */
#if defined(__has_include)
#if __has_include("example_id.h")
#include "example_id.h"
#endif
#endif

/*
 * ****************************************************************************************************
 * Macros
 * ****************************************************************************************************
*/

/* 0 when the make-id step was not run, or the compiler cannot look for its file. */
#ifndef EXAMPLE_DEVICE_ID
#define EXAMPLE_DEVICE_ID           0UL
#endif

/*
 * ****************************************************************************************************
 * Public function implementations
 * ****************************************************************************************************
*/

/*****************************************************************************************************/
/**
 * @brief Start a period of CONFIG_EXAMPLE_PERIOD_MS from now.
 *
 * @param[out] handle  The period to start.
 */
void example_init(example_t *handle)
{
    __ASSERT_NO_MSG(handle != NULL);

    handle->start = k_uptime_get_32();
    handle->count = 0U;
}

/*****************************************************************************************************/
/**
 * @brief Whether the period has passed. Returns at once, and starts the next one when it has.
 *
 * @param[in,out] handle  Period from example_init().
 * @return true once every CONFIG_EXAMPLE_PERIOD_MS, false in between.
 */
bool example_due(example_t *handle)
{
    bool due = false;

    __ASSERT_NO_MSG(handle != NULL);

    /* Subtracted, never added, so the answer stays right when the 32 bit
       uptime wraps round after 49 days. */
    if ((k_uptime_get_32() - handle->start) >= (uint32_t)CONFIG_EXAMPLE_PERIOD_MS)
    {
        /* The next period starts where this one should have ended, not now,
           so a loop that comes back late does not make the rhythm drift. */
        handle->start += (uint32_t)CONFIG_EXAMPLE_PERIOD_MS;
        handle->count++;
        due = true;
    }

    return due;
}

/*****************************************************************************************************/
/**
 * @brief How many times example_due() has said yes.
 *
 * @param[in] handle  Period from example_init().
 * @return The count since example_init().
 */
uint32_t example_count(const example_t *handle)
{
    __ASSERT_NO_MSG(handle != NULL);

    return handle->count;
}

/*****************************************************************************************************/
/**
 * @brief This device's own ID, from the example_id.h the make-id step made.
 *
 * @return The ID, or 0 when the step was not run.
 */
uint32_t example_id(void)
{
    return (uint32_t)EXAMPLE_DEVICE_ID;
}

/*****************************************************************************************************/
/**
 * @brief Print CONFIG_EXAMPLE_GREETING with printk, how many periods have passed, and the device ID.
 *
 * @param[in] handle  Period from example_init().
 */
void example_hello(const example_t *handle)
{
    __ASSERT_NO_MSG(handle != NULL);

    printk("%s %u, device %08X\n", CONFIG_EXAMPLE_GREETING, (unsigned int)handle->count,
           (unsigned int)example_id());
}
