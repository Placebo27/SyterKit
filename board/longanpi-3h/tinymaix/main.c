/* SPDX-License-Identifier: Apache-2.0 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <types.h>

#include <log.h>

#include <common.h>
#include <mmu.h>
#include <smalloc.h>
#include <sstdlib.h>
#include <string.h>

#include <pmu/axp.h>
#include <sys-dram.h>
#include <sys-i2c.h>

#include "tinymaix.h"
#include "tm_port.h"

extern sunxi_serial_t uart_dbg;

extern sunxi_i2c_t i2c_pmu;

extern void set_cpu_poweroff(void);

#define CONFIG_HEAP_BASE (0x40800000)
#define CONFIG_HEAP_SIZE (16 * 1024 * 1024)

/* clang-format off */
#define MDL_BUF_LEN (1464)
#define LBUF_LEN (1424)
const uint8_t mdl_data[2408]={\
	0x4d, 0x41, 0x49, 0x58, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x06, 0x00, 0xb8, 0x05, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03, 0x00, 0x00, 
	0x03, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0d, 0x00, 0x0d, 0x00, 0x04, 0x00, 
	0x81, 0x80, 0x80, 0x3b, 0x80, 0xff, 0xff, 0xff, 0x31, 0xe9, 0x84, 0x3c, 0x80, 0xff, 0xff, 0xff, 
	0x03, 0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 
	0x7d, 0x77, 0x4a, 0x3c, 0x85, 0xad, 0x87, 0x3c, 0x26, 0x92, 0xc5, 0x3b, 0xb9, 0xc9, 0x48, 0x3c, 
	0x2f, 0x5e, 0x5b, 0x46, 0x14, 0xc0, 0xb0, 0x81, 0xc8, 0x32, 0x0a, 0xd8, 0x6f, 0x09, 0x81, 0x27, 
	0xf6, 0xd6, 0x7f, 0x79, 0x50, 0xf9, 0x16, 0x2b, 0x2b, 0x5e, 0x60, 0xf3, 0x85, 0x99, 0x7a, 0x0a, 
	0x81, 0x67, 0x65, 0x19, 0x00, 0x00, 0x00, 0x00, 0x94, 0xfb, 0xff, 0xff, 0x94, 0xfc, 0xff, 0xff, 
	0x94, 0x41, 0x01, 0x00, 0xab, 0xfa, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x01, 0x00, 0x00, 
	0x10, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0d, 0x00, 0x0d, 0x00, 0x04, 0x00, 
	0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x08, 0x00, 0x31, 0xe9, 0x84, 0x3c, 0x80, 0xff, 0xff, 0xff, 
	0xa0, 0x2a, 0x84, 0x3c, 0x80, 0xff, 0xff, 0xff, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 
	0x70, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0xb6, 0xe0, 0x8a, 0x3b, 0xe0, 0xa8, 0x36, 0x3b, 
	0x50, 0x3d, 0xc9, 0x3b, 0x3e, 0x4f, 0x91, 0x3b, 0x88, 0x9e, 0x57, 0x3b, 0x22, 0xa8, 0x6c, 0x3b, 
	0x1f, 0x25, 0x9f, 0x3b, 0xf2, 0x40, 0x71, 0x3b, 0xf1, 0x0d, 0x28, 0x3e, 0x71, 0x3e, 0xa8, 0xda, 
	0xef, 0xad, 0x3d, 0x2c, 0x81, 0xb5, 0xcc, 0x0f, 0xf2, 0xdf, 0xdc, 0xdc, 0xbf, 0x9f, 0xd1, 0xe9, 
	0xea, 0xe9, 0xd0, 0xc3, 0xc1, 0x99, 0xf2, 0x53, 0x48, 0xe8, 0x1b, 0x34, 0xdd, 0xc3, 0xba, 0xaf, 
	0x81, 0xdb, 0xac, 0xb1, 0xeb, 0x2b, 0x13, 0x09, 0x18, 0x1c, 0x20, 0x2f, 0x34, 0x20, 0x56, 0x46, 
	0x3d, 0x12, 0xd6, 0xdb, 0xd4, 0xfb, 0x00, 0x3e, 0x5b, 0x3f, 0x48, 0x5d, 0x33, 0x55, 0x3f, 0x10, 
	0x81, 0xc0, 0xd3, 0xd6, 0x93, 0xbf, 0xd5, 0xd7, 0xca, 0xc9, 0xf2, 0x1b, 0x02, 0x24, 0xf8, 0x1b, 
	0xf9, 0xf7, 0xdf, 0xf8, 0x07, 0xff, 0x50, 0x1c, 0xe6, 0xb8, 0xc7, 0x1a, 0xd2, 0xce, 0x0c, 0x09, 
	0x11, 0xe1, 0xf6, 0x0e, 0x1f, 0x2f, 0x20, 0xfa, 0x2e, 0x2b, 0xc7, 0xf8, 0x0e, 0x49, 0x47, 0x19, 
	0x2b, 0x11, 0xf7, 0x37, 0x06, 0xe9, 0xb9, 0xb9, 0xe7, 0x7f, 0x42, 0x14, 0xa1, 0xf4, 0x29, 0xd1, 
	0xd5, 0x07, 0xf9, 0xfc, 0x0c, 0x1b, 0x2f, 0x2f, 0x25, 0x1e, 0x07, 0x02, 0xeb, 0xdd, 0x08, 0xa8, 
	0xb4, 0x2b, 0x1d, 0xe4, 0x16, 0x31, 0x16, 0x1f, 0xf7, 0x5c, 0x17, 0xe0, 0x0a, 0x15, 0x08, 0x25, 
	0xa4, 0xd9, 0x1a, 0x19, 0xf9, 0xf7, 0xf9, 0x8d, 0xa9, 0xcb, 0x81, 0xc0, 0xec, 0x18, 0x51, 0xc4, 
	0x0c, 0x40, 0x0c, 0x65, 0x24, 0xcd, 0xcd, 0xcd, 0x87, 0x81, 0xcd, 0xc2, 0x9f, 0xe4, 0x75, 0x52, 
	0xf8, 0x08, 0x08, 0x0c, 0x8e, 0x8a, 0x90, 0x3a, 0x1c, 0x03, 0x77, 0x3b, 0xf1, 0x45, 0x4b, 0x06, 
	0x48, 0x41, 0x10, 0x1a, 0x05, 0xfe, 0x00, 0xff, 0xea, 0xac, 0x81, 0xad, 0xda, 0xb6, 0xcb, 0x13, 
	0xef, 0xea, 0xd5, 0xc8, 0xb3, 0x0f, 0xfd, 0xf4, 0xfa, 0xfe, 0x0d, 0x04, 0x4b, 0x4c, 0x1c, 0x4b, 
	0x44, 0x09, 0x38, 0x33, 0xb0, 0xd2, 0x17, 0x8d, 0xaf, 0xe6, 0xda, 0xcb, 0xdd, 0x2e, 0x0f, 0x04, 
	0x3c, 0x7f, 0x3d, 0x24, 0x53, 0x32, 0xb0, 0xc7, 0xe1, 0xf1, 0x2d, 0x13, 0xe7, 0x4d, 0x24, 0xa1, 
	0xd8, 0xf1, 0xb1, 0x83, 0xd0, 0xc5, 0xc0, 0xe2, 0x24, 0x1f, 0xff, 0xff, 0xde, 0xd1, 0x00, 0x00, 
	0x57, 0xc8, 0xfe, 0xff, 0x53, 0xcc, 0x00, 0x00, 0xf3, 0x76, 0xff, 0xff, 0x99, 0xfc, 0xff, 0xff, 
	0xa0, 0x12, 0x00, 0x00, 0x5e, 0x1b, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x50, 0x05, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x78, 0x05, 0x00, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x08, 0x00, 
	0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x10, 0x00, 0xa0, 0x2a, 0x84, 0x3c, 0x80, 0xff, 0xff, 0xff, 
	0x05, 0x09, 0x68, 0x3d, 0x80, 0xff, 0xff, 0xff, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 
	0x90, 0x00, 0x00, 0x00, 0x10, 0x05, 0x00, 0x00, 0xe3, 0x93, 0xad, 0x3c, 0x61, 0x96, 0x68, 0x3c, 
	0x71, 0xd9, 0x96, 0x3c, 0xca, 0xa1, 0x4f, 0x3c, 0x7b, 0x9b, 0x6d, 0x3c, 0x35, 0x94, 0xa4, 0x3c, 
	0xc1, 0xf4, 0xa1, 0x3c, 0x36, 0xe6, 0x83, 0x3c, 0x93, 0xd4, 0x45, 0x3c, 0x16, 0x82, 0x5f, 0x3c, 
	0x39, 0xf8, 0x56, 0x3c, 0x8b, 0xf0, 0xaa, 0x3c, 0x68, 0x39, 0x66, 0x3c, 0x58, 0x1e, 0xa3, 0x3c, 
	0xd8, 0x1b, 0x86, 0x3c, 0x38, 0xad, 0x9a, 0x3c, 0xfb, 0xf8, 0x49, 0x99, 0xfb, 0xd6, 0x82, 0xff, 
	0x07, 0xdf, 0xfd, 0x16, 0x49, 0x28, 0xfd, 0x03, 0x10, 0x10, 0x0e, 0x13, 0xfe, 0x19, 0x31, 0x23, 
	0x0d, 0x81, 0xa5, 0x01, 0xde, 0xfb, 0x18, 0xf8, 0xca, 0xf6, 0xe1, 0xf2, 0x14, 0xf9, 0xff, 0x3d, 
	0x49, 0x1f, 0x2d, 0xf8, 0x04, 0x42, 0x05, 0x13, 0xf1, 0x1b, 0x03, 0x2a, 0xfb, 0x17, 0xde, 0xed, 
	0xfb, 0xe6, 0xf6, 0xc8, 0x16, 0x05, 0xd5, 0x32, 0x43, 0x10, 0x13, 0x38, 0x40, 0xe9, 0x05, 0x29, 
	0xa4, 0xae, 0x2d, 0xab, 0xa1, 0x05, 0xca, 0x81, 0x4a, 0xd0, 0xba, 0xe5, 0x3d, 0xc8, 0xbc, 0x06, 
	0xeb, 0x2b, 0x32, 0xf9, 0x12, 0x40, 0x2e, 0xcc, 0x10, 0xe4, 0x04, 0x5e, 0x58, 0x03, 0x2f, 0x1a, 
	0x33, 0xd2, 0xf5, 0xf4, 0x11, 0x2c, 0x08, 0xfd, 0xdd, 0x17, 0x11, 0xec, 0x48, 0x82, 0xf2, 0x3e, 
	0xe4, 0x22, 0x11, 0x2a, 0xc8, 0x0e, 0x2a, 0x0c, 0x53, 0xb0, 0x4b, 0x0a, 0xcb, 0x09, 0xf9, 0x42, 
	0x47, 0x09, 0x6a, 0x30, 0xe2, 0xfc, 0x02, 0xbb, 0x48, 0x0a, 0xf4, 0xdc, 0x7f, 0xd6, 0x9d, 0xfd, 
	0xb0, 0x07, 0x12, 0x61, 0x34, 0x11, 0xce, 0x44, 0xc0, 0xfd, 0x0d, 0x19, 0x29, 0xcf, 0xfb, 0xcd, 
	0xe0, 0xed, 0xae, 0xde, 0xe9, 0xf7, 0x22, 0x19, 0x11, 0xe9, 0x19, 0x18, 0xd0, 0x52, 0x17, 0xda, 
	0x1f, 0xb4, 0x10, 0x23, 0x36, 0xef, 0x08, 0xdd, 0xc2, 0xe8, 0x30, 0xd3, 0x08, 0x23, 0x02, 0x24, 
	0x99, 0x1d, 0xed, 0x12, 0x03, 0x11, 0xee, 0xe5, 0xdc, 0xf2, 0x13, 0x0a, 0x26, 0x1a, 0xf2, 0x02, 
	0xf8, 0xfc, 0xcb, 0x84, 0x5c, 0xed, 0x1d, 0x99, 0xf2, 0x57, 0x3e, 0xf3, 0x8a, 0xd6, 0xd6, 0xc1, 
	0x16, 0x05, 0x81, 0x00, 0xfd, 0x92, 0xd9, 0x36, 0xe7, 0x25, 0x17, 0xe8, 0x05, 0x19, 0x33, 0x0d, 
	0x48, 0xac, 0xdf, 0x15, 0xe9, 0xf5, 0xbd, 0xf7, 0xf0, 0x31, 0x72, 0x23, 0x16, 0x65, 0xe2, 0xb5, 
	0xf4, 0x0d, 0xea, 0xbc, 0xf3, 0x3b, 0x5a, 0xdf, 0x91, 0x10, 0xe9, 0xf7, 0x20, 0xb2, 0xfb, 0xe9, 
	0xfc, 0x0f, 0xb6, 0x3b, 0x04, 0x04, 0x1b, 0x25, 0x95, 0xc6, 0xd5, 0xfa, 0xa1, 0x90, 0xde, 0xb9, 
	0x8b, 0x39, 0xe8, 0xc1, 0x09, 0xf9, 0x05, 0x1d, 0x1b, 0x0f, 0x39, 0xff, 0xd5, 0x46, 0xf7, 0x29, 
	0x7a, 0x6a, 0xfa, 0xf7, 0x08, 0x3d, 0xaf, 0x07, 0x56, 0x41, 0x02, 0xd7, 0xec, 0xe7, 0x01, 0xe0, 
	0xd1, 0xb9, 0xc1, 0x94, 0xd4, 0xeb, 0xca, 0xdd, 0xd6, 0xb2, 0xe1, 0xe1, 0x0c, 0xfc, 0x38, 0x11, 
	0x01, 0xea, 0xdd, 0x23, 0x81, 0xe4, 0x10, 0xff, 0x00, 0x20, 0x1a, 0xc7, 0xf4, 0x3b, 0xf9, 0xe5, 
	0x81, 0xe7, 0x12, 0xe0, 0xeb, 0x07, 0x0e, 0x37, 0xf6, 0x36, 0xf6, 0xed, 0x27, 0x09, 0x38, 0x0c, 
	0x0d, 0xe9, 0x14, 0xee, 0xde, 0x16, 0x1e, 0x2c, 0x22, 0x22, 0xe3, 0xfb, 0x14, 0x45, 0x08, 0xf2, 
	0xc7, 0xf8, 0xf9, 0xf4, 0x13, 0xdb, 0xde, 0x10, 0x21, 0xdd, 0x04, 0x06, 0x22, 0x0f, 0x1a, 0x23, 
	0x18, 0xef, 0xec, 0x00, 0xde, 0xd9, 0xc4, 0xcf, 0x0b, 0xf0, 0xfe, 0x37, 0xf2, 0xee, 0xf4, 0x4c, 
	0x48, 0xec, 0xbd, 0x27, 0xfb, 0xc4, 0xea, 0x09, 0x29, 0x00, 0x0c, 0x2a, 0x2e, 0xdc, 0xf8, 0x03, 
	0xd1, 0xb3, 0x16, 0x04, 0x81, 0xd3, 0x09, 0xf7, 0xd3, 0xfa, 0xf5, 0xd3, 0xe0, 0xe0, 0xc6, 0xff, 
	0x19, 0xf6, 0x2f, 0x01, 0xf7, 0x0a, 0x26, 0x02, 0xf5, 0x2c, 0x11, 0x0b, 0x22, 0xff, 0xfc, 0x06, 
	0xdd, 0xfd, 0x16, 0xd9, 0xeb, 0x4f, 0x1c, 0xe0, 0xe1, 0xe3, 0xd5, 0x15, 0xe8, 0xf6, 0x64, 0x29, 
	0x3d, 0xc3, 0x23, 0x49, 0xe1, 0xce, 0xdb, 0xbc, 0xde, 0xd9, 0xe3, 0xd3, 0xb0, 0x0c, 0x22, 0x2a, 
	0x2c, 0x08, 0x0d, 0x0c, 0x7f, 0xf6, 0x1b, 0xd3, 0x17, 0xe0, 0x11, 0xf6, 0xdc, 0xfd, 0x08, 0xca, 
	0xf6, 0x28, 0xf3, 0xe3, 0xb7, 0x31, 0x09, 0x20, 0x14, 0xa8, 0x18, 0xf2, 0xe3, 0x25, 0x3b, 0xd7, 
	0x1a, 0xc5, 0xb8, 0xe2, 0xf2, 0x0b, 0xf8, 0x1e, 0xb9, 0xfa, 0x3f, 0xd1, 0xf4, 0xcd, 0x24, 0xde, 
	0x03, 0x41, 0x11, 0x08, 0xfd, 0xe6, 0xdd, 0xfc, 0x22, 0x14, 0x0f, 0x07, 0x0e, 0x25, 0x01, 0xd8, 
	0x02, 0x2b, 0xf9, 0x94, 0xdc, 0x1f, 0xfa, 0xfa, 0x7c, 0x71, 0xef, 0x09, 0x4d, 0x9f, 0xb9, 0x81, 
	0xe8, 0x20, 0xe6, 0x49, 0x43, 0x16, 0xd0, 0x49, 0x37, 0xdf, 0x00, 0xf2, 0x9a, 0x15, 0x9a, 0x8f, 
	0xd8, 0xd3, 0x46, 0xd0, 0xdf, 0xc8, 0xd7, 0xf0, 0x24, 0x12, 0xff, 0x12, 0xf9, 0x42, 0x15, 0xf0, 
	0x10, 0x13, 0xae, 0xe4, 0x33, 0xa6, 0x20, 0x1e, 0xb2, 0xdf, 0x24, 0xda, 0x06, 0x71, 0x62, 0x2a, 
	0x0a, 0x2c, 0x34, 0xcc, 0x27, 0xeb, 0x3a, 0x62, 0xf0, 0x14, 0x1f, 0x56, 0x2c, 0xcc, 0x4e, 0x36, 
	0x48, 0xfc, 0x90, 0x1c, 0x0d, 0xca, 0x1d, 0x58, 0x16, 0x44, 0x1a, 0xeb, 0x88, 0x0f, 0x21, 0xf9, 
	0xfb, 0x1e, 0x5a, 0x2e, 0xa7, 0x0d, 0xfb, 0x10, 0xbf, 0xd1, 0x3b, 0xe7, 0x11, 0x2b, 0x3c, 0xdd, 
	0xfb, 0x08, 0xe2, 0xa2, 0x81, 0xd2, 0x01, 0xc3, 0xe6, 0x29, 0xae, 0xde, 0x33, 0xf1, 0x05, 0x72, 
	0x29, 0xe0, 0x07, 0x05, 0xc1, 0xcf, 0xb7, 0xf2, 0xec, 0x18, 0x51, 0xff, 0xe2, 0x3d, 0x35, 0x0c, 
	0xb3, 0x06, 0x32, 0xa5, 0xad, 0x22, 0x3d, 0xeb, 0x7f, 0x45, 0x92, 0x89, 0x0c, 0xc7, 0x20, 0x12, 
	0x3b, 0x00, 0x52, 0x1d, 0x08, 0xb0, 0xa3, 0xee, 0xdc, 0x0f, 0x1c, 0xfa, 0x2b, 0x06, 0xfa, 0x03, 
	0x04, 0xe1, 0xd2, 0x16, 0xf8, 0xde, 0xe9, 0x39, 0x22, 0x30, 0x3f, 0x36, 0x14, 0x1f, 0x1e, 0xcb, 
	0x3e, 0x2b, 0xd4, 0xa7, 0x8c, 0xf1, 0xbd, 0xd0, 0xe0, 0x32, 0x21, 0xff, 0x46, 0x6b, 0xf5, 0xfd, 
	0x51, 0x35, 0xc8, 0xce, 0xa7, 0x06, 0xef, 0xba, 0xf8, 0x54, 0x1e, 0x1d, 0x37, 0x27, 0xd6, 0xd2, 
	0x81, 0xf6, 0xf9, 0xe0, 0xea, 0xc4, 0xfd, 0xfa, 0xe4, 0x4c, 0x29, 0xfb, 0x2a, 0xd6, 0xc5, 0xc6, 
	0x0c, 0x31, 0x24, 0x3c, 0xfc, 0x0e, 0x02, 0x94, 0xf0, 0xea, 0x40, 0x37, 0x22, 0x1f, 0xce, 0xc3, 
	0xe2, 0xf0, 0x2a, 0x24, 0xf6, 0xed, 0xdd, 0xd5, 0xe8, 0x2a, 0x4d, 0x59, 0x2e, 0x16, 0x11, 0x1c, 
	0x15, 0x2d, 0x18, 0xc5, 0xfa, 0xfd, 0x1f, 0xf8, 0xd3, 0x2b, 0xeb, 0xe7, 0xf6, 0xc9, 0xdc, 0x34, 
	0xf8, 0xd7, 0xee, 0xd7, 0x1c, 0x0f, 0x1d, 0x1f, 0x9b, 0x9f, 0xd8, 0x03, 0x30, 0x3a, 0xb4, 0x02, 
	0xce, 0x1a, 0x08, 0x43, 0xe5, 0xf8, 0xdf, 0x28, 0xbf, 0xd8, 0xa9, 0x92, 0xe7, 0xbe, 0x18, 0xe9, 
	0x6e, 0x60, 0x97, 0x02, 0x2a, 0xfb, 0xcf, 0x07, 0x0b, 0xe8, 0xdc, 0x05, 0x2d, 0x43, 0x3c, 0x34, 
	0x47, 0x26, 0x09, 0x97, 0x99, 0xf2, 0xb7, 0x15, 0x9e, 0xb4, 0xd6, 0x8b, 0x0b, 0xba, 0xba, 0xea, 
	0xd1, 0x02, 0x7f, 0x38, 0xf1, 0x30, 0x29, 0x33, 0x4c, 0x30, 0x52, 0x1d, 0x01, 0x53, 0x32, 0xbf, 
	0xc9, 0xe8, 0x08, 0xd7, 0xc9, 0xe0, 0xf9, 0xc8, 0x2a, 0xcf, 0xe2, 0x3e, 0x08, 0x17, 0xfd, 0x05, 
	0xc9, 0xee, 0xf3, 0xd3, 0x05, 0x1a, 0xff, 0x16, 0x03, 0x10, 0xe2, 0x3d, 0x20, 0xee, 0xff, 0xf9, 
	0x0a, 0xef, 0x09, 0x0a, 0x02, 0xf9, 0xfd, 0xda, 0xde, 0x2a, 0x09, 0x08, 0x07, 0xd0, 0x02, 0xd3, 
	0x7f, 0xdf, 0xee, 0x2c, 0x27, 0xff, 0xe0, 0xf6, 0xf7, 0x0f, 0xe9, 0xf2, 0x22, 0xfb, 0xf5, 0xff, 
	0x3d, 0x53, 0x14, 0xeb, 0x15, 0xfe, 0xe4, 0x06, 0xdc, 0x27, 0x11, 0xe3, 0x1b, 0x9d, 0xd8, 0x07, 
	0xec, 0xcd, 0xb0, 0xee, 0xf8, 0x05, 0x0d, 0x2c, 0x24, 0xf8, 0xdf, 0xfe, 0xec, 0x1b, 0x69, 0xe6, 
	0x04, 0xdb, 0x04, 0x3b, 0x53, 0x1b, 0x3d, 0x20, 0x07, 0x00, 0xc8, 0xdb, 0x47, 0xd7, 0xe0, 0xd8, 
	0xc7, 0xde, 0xdc, 0xe6, 0x19, 0xdc, 0xf7, 0xef, 0x14, 0x33, 0xf3, 0x2d, 0xee, 0xac, 0xe1, 0x9c, 
	0xfb, 0xd5, 0xfe, 0xff, 0x14, 0x2d, 0x81, 0xfc, 0xf0, 0x03, 0x06, 0xcc, 0xe3, 0x1e, 0xdf, 0xa2, 
	0xef, 0xed, 0xeb, 0x21, 0x06, 0x3c, 0x3c, 0x7f, 0x43, 0xf7, 0x16, 0x01, 0x37, 0x2c, 0x4d, 0xc4, 
	0xf9, 0xf5, 0x10, 0x2b, 0xfb, 0x02, 0xd1, 0x0c, 0x08, 0x99, 0x2b, 0xfb, 0xb4, 0xfc, 0xe3, 0xd5, 
	0xd4, 0x37, 0xcc, 0x04, 0x29, 0xfa, 0x51, 0x16, 0x18, 0x19, 0x03, 0x01, 0x0a, 0xe7, 0x44, 0xbf, 
	0x01, 0xf4, 0xf7, 0xf7, 0xe5, 0x2a, 0x6f, 0x36, 0x1f, 0x4c, 0x03, 0x93, 0xdb, 0xd8, 0xe0, 0x3a, 
	0x18, 0xf8, 0x08, 0x0e, 0x46, 0x07, 0xe3, 0xce, 0xe8, 0x58, 0x00, 0x00, 0x38, 0x27, 0x00, 0x00, 
	0x0c, 0xe2, 0xff, 0xff, 0xdc, 0x6a, 0xff, 0xff, 0x7e, 0x85, 0xfe, 0xff, 0x76, 0x17, 0x00, 0x00, 
	0xeb, 0x5a, 0xff, 0xff, 0x46, 0xcb, 0xff, 0xff, 0xb6, 0x80, 0x00, 0x00, 0x31, 0xaf, 0xff, 0xff, 
	0xdb, 0x36, 0x00, 0x00, 0xf6, 0xef, 0xff, 0xff, 0x0c, 0x82, 0xff, 0xff, 0x82, 0xe2, 0xff, 0xff, 
	0x55, 0x12, 0xff, 0xff, 0xf1, 0xbf, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 
	0x78, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x10, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 0x05, 0x09, 0x68, 0x3d, 0x80, 0xff, 0xff, 0xff, 
	0x21, 0x1a, 0xb7, 0x3c, 0x80, 0xff, 0xff, 0xff, 0x02, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xa8, 0x05, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x0a, 0x00, 0x21, 0x1a, 0xb7, 0x3c, 0x80, 0xff, 0xff, 0xff, 
	0xfd, 0x06, 0x1b, 0x3e, 0x2a, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 
	0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe9, 0xe2, 0xaf, 0x3c, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x2a, 0xe2, 0x29, 0xbf, 0x0b, 0xdd, 0x09, 0xfa, 0x42, 0xb1, 0xf9, 0xc2, 0x46, 0xd2, 0x0f, 0xb5, 
	0x34, 0xea, 0xb3, 0x17, 0xdb, 0xfb, 0xbc, 0x0f, 0x24, 0xdf, 0xce, 0xf9, 0x1c, 0xe0, 0xbb, 0x38, 
	0xb7, 0xe2, 0x28, 0x0c, 0x04, 0x93, 0x39, 0xe3, 0xf5, 0xee, 0x3c, 0xc6, 0x37, 0xf6, 0xb6, 0x41, 
	0xca, 0xba, 0xd7, 0xed, 0xc3, 0xfd, 0x3a, 0x0d, 0xd2, 0x1d, 0x07, 0xa9, 0xe9, 0x06, 0x53, 0x25, 
	0x25, 0xd7, 0x1a, 0x31, 0xf0, 0x0e, 0x81, 0x19, 0xef, 0xc0, 0x0a, 0x2d, 0xb5, 0x33, 0xfb, 0xe6, 
	0xdc, 0x1d, 0x03, 0x83, 0xe2, 0x48, 0xd9, 0x28, 0x23, 0x31, 0x34, 0xe6, 0xeb, 0xdb, 0xdd, 0xdc, 
	0xe7, 0x4e, 0xc0, 0xcf, 0x25, 0x3c, 0x98, 0xb5, 0xec, 0xcc, 0x36, 0xc3, 0x3c, 0x08, 0xf2, 0xe3, 
	0x0d, 0xf7, 0x41, 0x44, 0x90, 0x1f, 0x22, 0x07, 0xcb, 0x0b, 0xb8, 0x09, 0xfe, 0xc8, 0xbf, 0x0b, 
	0xbd, 0xf5, 0x06, 0xd2, 0x4d, 0xcc, 0x07, 0xdf, 0x1f, 0xfb, 0xf9, 0x2f, 0xe6, 0xa2, 0x26, 0x00, 
	0xf0, 0x36, 0xfc, 0xe9, 0xf3, 0xeb, 0x0e, 0xb0, 0xbd, 0x3f, 0xc6, 0x1c, 0xca, 0x4d, 0x02, 0xf4, 
	0xbc, 0xb2, 0xff, 0xff, 0x1a, 0xa5, 0xff, 0xff, 0xc5, 0xc3, 0xff, 0xff, 0x1c, 0xaa, 0xff, 0xff, 
	0x44, 0xc4, 0xff, 0xff, 0x64, 0xcc, 0xff, 0xff, 0xfb, 0x9c, 0xff, 0xff, 0x62, 0xc3, 0xff, 0xff, 
	0x97, 0xbd, 0xff, 0xff, 0xe6, 0xc9, 0xff, 0xff, 0x03, 0x00, 0x01, 0x00, 0x30, 0x00, 0x00, 0x00, 
	0xa8, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x0a, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x0a, 0x00, 0xfd, 0x06, 0x1b, 0x3e, 0x2a, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x80, 0x3b, 0x80, 0xff, 0xff, 0xff, 
};

const static uint8_t mnist_pic[28*28]={
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,116,125,171,255,255,150, 93,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,253,253,253,218, 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,213,142,176,253,253,122,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 52,250,253,210, 32, 12,  0,  6,206,253,140,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 77,251,210, 25,  0,  0,  0,122,248,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 31, 18,  0,  0,  0,  0,209,253,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,117,247,253,198, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 76,247,253,231, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,253,253,144,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,176,246,253,159, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25,234,253,233, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,198,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0, 78,248,253,189, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 19,200,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,134,253,253,173, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 43, 20, 20, 20, 20,  5,  0,  5, 20, 20, 37,150,150,150,147, 10,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253,253,253,253,253,253,168,143,166,253,253,253,253,253,253,253,123,  0,
  0,  0,  0,  0,  0,  0,  0,  0,174,253,253,253,253,253,253,253,253,253,253,253,249,247,247,169,117,117, 57,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,118,123,123,123,166,253,253,253,155,123,123, 41,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
/* clang-format on */

static tm_err_t layer_cb(tm_mdl_t *mdl, tml_head_t *lh) {//dump middle result
    int h = lh->out_dims[1];
    int w = lh->out_dims[2];
    int ch = lh->out_dims[3];
    mtype_t *output = TML_GET_OUTPUT(mdl, lh);
    return TM_OK;
    TM_PRINTF("Layer %d callback ========\n", mdl->layer_i);
#if 1
    for (int y = 0; y < h; y++) {
        TM_PRINTF("[");
        for (int x = 0; x < w; x++) {
            TM_PRINTF("[");
            for (int c = 0; c < ch; c++) {
#if TM_MDL_TYPE == TM_MDL_FP32
                TM_PRINTF("%.3f,", output[(y * w + x) * ch + c]);
#else
                TM_PRINTF("%.3f,", TML_DEQUANT(lh, output[(y * w + x) * ch + c]));
#endif
            }
            TM_PRINTF("],");
        }
        TM_PRINTF("],\n");
    }
    TM_PRINTF("\n");
#endif
    return TM_OK;
}

static void parse_output(tm_mat_t *outs) {
    tm_mat_t out = outs[0];
    float *data = out.dataf;
    float maxp = 0;
    int maxi = -1;
    for (int i = 0; i < 10; i++) {
        TM_PRINTF("%d: %.3f\n", i, data[i]);
        if (data[i] > maxp) {
            maxi = i;
            maxp = data[i];
        }
    }
    TM_PRINTF("### Predict output is: Number %d, prob %.3f\n", maxi, maxp);
    return;
}

int main(void) {
    sunxi_serial_init(&uart_dbg);

    show_banner();

    sunxi_clk_init();

    sunxi_clk_dump();

    set_cpu_poweroff();

    neon_enable();

    sunxi_i2c_init(&i2c_pmu);

    pmu_axp1530_init(&i2c_pmu);

    pmu_axp1530_dump(&i2c_pmu);

    int set_vol = 1100; /* LPDDR4 1100mv */

    int temp_vol, src_vol = pmu_axp1530_get_vol(&i2c_pmu, "dcdc3");
    if (src_vol > set_vol) {
        for (temp_vol = src_vol; temp_vol >= set_vol; temp_vol -= 50) {
            pmu_axp1530_set_vol(&i2c_pmu, "dcdc3", temp_vol, 1);
        }
    } else if (src_vol < set_vol) {
        for (temp_vol = src_vol; temp_vol <= set_vol; temp_vol += 50) {
            pmu_axp1530_set_vol(&i2c_pmu, "dcdc3", temp_vol, 1);
        }
    }

    mdelay(30); /* Delay 300ms for pmu bootup */

    pmu_axp1530_dump(&i2c_pmu);

    /* Initialize the DRAM and enable memory management unit (MMU). */
    uint64_t dram_size = sunxi_dram_init(NULL);

    arm32_mmu_enable(SDRAM_BASE, dram_size);

    /* Initialize the small memory allocator. */
    smalloc_init(CONFIG_HEAP_BASE, CONFIG_HEAP_SIZE);

    sunxi_clk_dump();

    TM_DBGT_INIT();
    TM_PRINTF("Running MNIST Test\n");
    tm_mdl_t mdl;

    TM_PRINTF("Picture of Data: \n");
    for (int i = 0; i < 28 * 28; i++) {
        TM_PRINTF("%3d,", mnist_pic[i]);
        if (i % 28 == 27) TM_PRINTF("\n");
    }

    tm_mat_t in_uint8 = {3, 28, 28, 1, {(mtype_t *) mnist_pic}};
    tm_mat_t in = {3, 28, 28, 1, {NULL}};
    tm_mat_t outs[1];
    tm_err_t res;

    res = tm_load(&mdl, mdl_data, NULL, layer_cb, &in);
    if (res != TM_OK) {
        TM_PRINTF("tm model load err %d\n", res);
        return -1;
    }

#if (TM_MDL_TYPE == TM_MDL_INT8) || (TM_MDL_TYPE == TM_MDL_INT16)
    res = tm_preprocess(&mdl, TMPP_UINT2INT, &in_uint8, &in);
#else
    res = tm_preprocess(&mdl, TMPP_UINT2FP01, &in_uint8, &in);
#endif
    TM_DBGT_START();
    res = tm_run(&mdl, &in, outs);
    TM_DBGT(" MNIST Run");
    if (res == TM_OK) parse_output(outs);
    else
        TM_PRINTF("tm run error: %d\n", res);
    tm_unload(&mdl);

    abort();

    return 0;
}