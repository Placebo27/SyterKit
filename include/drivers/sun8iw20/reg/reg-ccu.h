/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __SUN8IW20_REG_CCU_H__
#define __SUN8IW20_REG_CCU_H__

#include <reg-ncat.h>

#define CCU_BASE SUNXI_CCMU_BASE

#define CCU_PLL_CPU_CTRL_REG (0x000)
#define CCU_PLL_DDR_CTRL_REG (0x010)
#define CCU_PLL_PERI0_CTRL_REG (0x020)
#define CCU_PLL_PERI1_CTRL_REG (0x028)
#define CCU_PLL_GPU_CTRL_REG (0x030)
#define CCU_PLL_VIDEO0_CTRL_REG (0x040)
#define CCU_PLL_VIDEO1_CTRL_REG (0x048)
#define CCU_PLL_VE_CTRL (0x058)
#define CCU_PLL_DE_CTRL (0x060)
#define CCU_PLL_HSIC_CTRL (0x070)
#define CCU_PLL_AUDIO0_CTRL_REG (0x078)
#define CCU_PLL_AUDIO1_CTRL_REG (0x080)
#define CCU_PLL_DDR_PAT0_CTRL_REG (0x110)
#define CCU_PLL_DDR_PAT1_CTRL_REG (0x114)
#define CCU_PLL_PERI0_PAT0_CTRL_REG (0x120)
#define CCU_PLL_PERI0_PAT1_CTRL_REG (0x124)
#define CCU_PLL_PERI1_PAT0_CTRL_REG (0x128)
#define CCU_PLL_PERI1_PAT1_CTRL_REG (0x12c)
#define CCU_PLL_GPU_PAT0_CTRL_REG (0x130)
#define CCU_PLL_GPU_PAT1_CTRL_REG (0x134)
#define CCU_PLL_VIDEO0_PAT0_CTRL_REG (0x140)
#define CCU_PLL_VIDEO0_PAT1_CTRL_REG (0x144)
#define CCU_PLL_VIDEO1_PAT0_CTRL_REG (0x148)
#define CCU_PLL_VIDEO1_PAT1_CTRL_REG (0x14c)
#define CCU_PLL_VE_PAT0_CTRL_REG (0x158)
#define CCU_PLL_VE_PAT1_CTRL_REG (0x15c)
#define CCU_PLL_DE_PAT0_CTRL_REG (0x160)
#define CCU_PLL_DE_PAT1_CTRL_REG (0x164)
#define CCU_PLL_HSIC_PAT0_CTRL_REG (0x170)
#define CCU_PLL_HSIC_PAT1_CTRL_REG (0x174)
#define CCU_PLL_AUDIO0_PAT0_CTRL_REG (0x178)
#define CCU_PLL_AUDIO0_PAT1_CTRL_REG (0x17c)
#define CCU_PLL_AUDIO1_PAT0_CTRL_REG (0x180)
#define CCU_PLL_AUDIO1_PAT1_CTRL_REG (0x184)
#define CCU_PLL_CPU_BIAS_REG (0x300)
#define CCU_PLL_DDR_BIAS_REG (0x310)
#define CCU_PLL_PERI0_BIAS_REG (0x320)
#define CCU_PLL_PERI1_BIAS_REG (0x328)
#define CCU_PLL_GPU_BIAS_REG (0x330)
#define CCU_PLL_VIDEO0_BIAS_REG (0x340)
#define CCU_PLL_VIDEO1_BIAS_REG (0x348)
#define CCU_PLL_VE_BIAS_REG (0x358)
#define CCU_PLL_DE_BIAS_REG (0x360)
#define CCU_PLL_HSIC_BIAS_REG (0x370)
#define CCU_PLL_AUDIO0_BIAS_REG (0x378)
#define CCU_PLL_AUDIO1_BIAS_REG (0x380)
#define CCU_PLL_CPU_TUN_REG (0x400)
#define CCU_CPU_AXI_CFG_REG (0x500)
#define CCU_CPU_GATING_REG (0x504)
#define CCU_PSI_CLK_REG (0x510)
#define CCU_AHB3_CLK_REG (0x51c)
#define CCU_APB0_CLK_REG (0x520)
#define CCU_APB1_CLK_REG (0x524)
#define CCU_MBUS_CLK_REG (0x540)
#define CCU_DMA_BGR_REG (0x70c)
#define CCU_DRAM_CLK_REG (0x800)
#define CCU_MBUS_MAT_CLK_GATING_REG (0x804)
#define CCU_DRAM_BGR_REG (0x80c)
#define CCU_SMHC0_CLK_REG (0x830)
#define CCU_SMHC_BGR_REG (0x84c)
#define CCU_UART_BGR_REG (0x90c)
#define CCU_TWI_BGR_REG (0x91c)
#define CCU_SCR_BGR_REG (0x93C)
#define CCU_SPI0_CLK_REG (0x940)
#define CCU_SPI_BGR_REG (0x96c)
#define CCU_USB0_CLK_REG (0xA70)
#define CCU_USB_BGR_REG (0xA8C)

/* MMC clock bit field */
#define CCU_MMC_CTRL_M(x) ((x) -1)
#define CCU_MMC_CTRL_N(x) ((x) << 8)
#define CCU_MMC_CTRL_OSCM24 (0x0 << 24)
#define CCU_MMC_CTRL_PLL6X1 (0x1 << 24)
#define CCU_MMC_CTRL_PLL6X2 (0x2 << 24)
#define CCU_MMC_CTRL_PLL_PERIPH1X CCU_MMC_CTRL_PLL6X1
#define CCU_MMC_CTRL_PLL_PERIPH2X CCU_MMC_CTRL_PLL6X2
#define CCU_MMC_CTRL_ENABLE (0x1 << 31)
/* if doesn't have these delays */
#define CCU_MMC_CTRL_OCLK_DLY(a) ((void) (a), 0)
#define CCU_MMC_CTRL_SCLK_DLY(a) ((void) (a), 0)

#define CCU_MMC_BGR_SMHC0_GATE (1 << 0)
#define CCU_MMC_BGR_SMHC1_GATE (1 << 1)
#define CCU_MMC_BGR_SMHC2_GATE (1 << 2)

#define CCU_MMC_BGR_SMHC0_RST (1 << 16)
#define CCU_MMC_BGR_SMHC1_RST (1 << 17)
#define CCU_MMC_BGR_SMHC2_RST (1 << 18)

/* DSP base */
#define CCU_DSP_CLK_REG (0xc70)
#define CCU_BIT_DSP_SCLK_GATING (31)
#define CCU_DSP_CLK_M_MASK (0x1f << 0)
#define CCU_DSP_CLK_SRC_MASK (0x7 << 24)
#define CCU_DSP_CLK_SRC_HOSC (0)
#define CCU_DSP_CLK_SRC_32K (0x1 << 24)
#define CCU_DSP_CLK_SRC_16M (0x2 << 24)
#define CCU_DSP_CLK_SRC_PERI2X (0x3 << 24)
#define CCU_DSP_CLK_SRC_AUDIO1_DIV2 (0x4 << 24)
/* x must be 1 - 32 */
#define CCU_DSP_CLK_FACTOR_M(x) (((x)-1) << 0)

#define CCU_DSP_BGR_REG (0xc7c)
#define CCU_BIT_DSP0_CFG_GATING (1)
#define CCU_BIT_DSP0_RST (16)
#define CCU_BIT_DSP0_CFG_RST (17)
#define CCU_BIT_DSP0_DBG_RST (18)

/* RISC-V base */
#define CCU_RISCV_GATING_RST_REG (0x0d04)
#define CCU_RISCV_CLK_GATING (0x1 << 31)
#define CCU_RISCV_GATING_FIELD (0x16aa << 0)

#define CCU_RISCV_RST_REG (0x0f20)
#define CCU_RISCV_RST_KEY_FIELD (0x16aa << 16)
#define CCU_RISCV_RST_SOFT_RSTN (0x1 << 0)

#define CCU_RISCV_CLK_REG (0x0d00)
#define CCU_RISCV_CLK_MASK (0x7 << 24)
#define CCU_RISCV_CLK_HOSC (0x0 << 24)
#define CCU_RISCV_CLK_32K (0x1 << 24)
#define CCU_RISCV_CLK_16M (0x2 << 24)
#define CCU_RISCV_CLK_PERI_800M (0x3 << 24)
#define CCU_RISCV_CLK_PERI_PERIPLL1X (0x4 << 24)
#define CCU_RISCV_CLK_CPUPLL (0x5 << 24)
#define CCU_RISCV_CLK_AUDIO1PLL_DIV2 (0x6 << 24)

#define CCU_RISCV_CFG_BGR_REG (0x0d0c)
#define CCU_RISCV_CFG_RST (0x1 << 16)
#define CCU_RISCV_CFG_GATING (0x1 << 0)

#endif// __SUN8IW20_REG_CCU_H__
