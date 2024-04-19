/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __GPT_H__
#define __GPT_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <types.h>

#include <sys-gpio.h>
#include <sys-sdcard.h>

#ifdef __cplusplus
extern "C" {
#endif// __cplusplus

#define MBR_SIZE (16 * 1024)
#define GPT_HEADER_SIGNATURE 0x5452415020494645ULL

#define GPT_ENTRY_OFFSET 1024
#define GPT_HEAD_OFFSET 512
#define GPT_DEFAULT_START_SEC 0

typedef struct {
    uint8_t b[16];
} efi_guid_t;

typedef uint16_t efi_char16_t;

typedef struct _gpt_header {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t header_crc32;
    uint32_t reserved1;
    uint64_t my_lba;
    uint64_t alternate_lba;
    uint64_t first_usable_lba;
    uint64_t last_usable_lba;
    efi_guid_t disk_guid;
    uint64_t partition_entry_lba;
    uint32_t num_partition_entries;
    uint32_t sizeof_partition_entry;
    uint32_t partition_entry_array_crc32;
} __attribute__((__packed__)) gpt_header;

typedef union _gpt_entry_attributes {
    struct {
        uint64_t required_to_function : 1;
        uint64_t no_block_io_protocol : 1;
        uint64_t legacy_bios_bootable : 1;
        uint64_t reserved : 27;
        uint64_t user_type : 16;
        uint64_t ro : 1;
        uint64_t keydata : 1;
        uint64_t type_guid_specific : 16;
    } fields;
    uint64_t raw;
} __attribute__((__packed__)) gpt_entry_attributes;

#define PARTNAME_SZ (72 / sizeof(efi_char16_t))

typedef struct _gpt_entry {
    efi_guid_t partition_type_guid;
    efi_guid_t unique_partition_guid;
    uint64_t starting_lba;
    uint64_t ending_lba;
    gpt_entry_attributes attributes;
    efi_char16_t partition_name[PARTNAME_SZ];
} __attribute__((__packed__)) gpt_entry;


int init_gpt_sdmmc(sdmmc_pdata_t *data);

#ifdef __cplusplus
}
#endif// __cplusplus

#endif//__GPT_H__