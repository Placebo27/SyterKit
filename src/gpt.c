/* SPDX-License-Identifier: GPL-2.0 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <types.h>

#include <smalloc.h>

#include <sys-gpio.h>
#include <sys-sdcard.h>
#include <gpt.h>

static uint8_t *gpt_data_buffer;

static int gpt_show_partition_info(char *buf) {
    char char8_name[PARTNAME_SZ] = {0};
    gpt_header *gpt_head = (gpt_header *) (buf + GPT_HEAD_OFFSET);
    gpt_entry *entry = (gpt_entry *) (buf + GPT_ENTRY_OFFSET);

    for (int i = 0; i < gpt_head->num_partition_entries; i++) {
        for (int j = 0; j < PARTNAME_SZ; j++) {
            char8_name[j] = (char) (entry[i].partition_name[j]);
        }
        printk_debug("GPT:%s: 0x%lx  0x%lx\n", char8_name, (long) entry[i].starting_lba, (long) entry[i].ending_lba);
    }

    return 0;
}

static int is_gpt_valid(void *buffer) {
    gpt_header *gpt_head = (gpt_header *) (buffer + GPT_HEAD_OFFSET);

    if (gpt_head->signature != GPT_HEADER_SIGNATURE) {
        printk_error("GPT: magic error, 0x%llx != 0x%llx\n", gpt_head->signature, GPT_HEADER_SIGNATURE);
        return 0;
    }
    gpt_show_partition_info(buffer);
    return 1;
}

int init_gpt_sdmmc(sdmmc_pdata_t *data) {
    uint8_t *buffer = smalloc(MBR_SIZE);
    memset(buffer, 0, MBR_SIZE);

    sdmmc_blk_read(data, buffer, GPT_DEFAULT_START_SEC, MBR_SIZE / 512);

    if (is_gpt_valid(buffer)) {
        gpt_data_buffer = buffer;
        return 0;
    } else {
        sfree(buffer);
        return -1;
    }
}