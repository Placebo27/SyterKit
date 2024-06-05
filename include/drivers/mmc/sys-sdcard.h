/* SPDX-License-Identifier:	GPL-2.0+ */

#ifndef _SYS_SDCARD_H_
#define _SYS_SDCARD_H_

#include <io.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <types.h>

#include "sys-mmc.h"
#include "sys-sdhci.h"

#ifdef __cplusplus
extern "C" {
#endif// __cplusplus

typedef struct {
    mmc_t card;
    sunxi_sdhci_t *hci;
    uint8_t buf[512];
    bool online;
} sdmmc_pdata_t;

/**
 * External declaration of the 'card0' SDMMC platform data structure.
 */
extern sdmmc_pdata_t card0;

#ifdef __cplusplus
}
#endif// __cplusplus

#endif// _SYS_SDCARD_H_