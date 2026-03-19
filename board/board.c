/**
 * @file board.c
 * @brief board config source file.
 *
 * @copyright Copyright (c) 2022  Semidrive Semiconductor.
 *            All rights reserved.
 */

#include <clock_ip.h>
#include <debug.h>
#include <irq_num.h>
#include <param.h>
#include <part.h>
#include <regs_base.h>
#include <reset_ip.h>
#include <types.h>
#include <cache.h>
#include <arm.h>
#include <register.h>
#include <reg.h>
#include <core_id.h>
#include <sdrv_scr.h>
#include <scr_hw.h>
#include "board.h"

#include "reset_cfg.h"
#if CONFIG_ARCH_WITH_MPU
#include <mpu.h>
#endif

#ifndef CONFIG_CONSOLE_UART
#define CONSOLE_UART UART15
#else
#define CONSOLE_UART CONFIG_CONSOLE_UART
#endif

extern uint64_t __vector;
extern uint64_t __hyp_vector;

sdrv_uart_t g_console_uart;

static sdrv_uart_config_t console_uart_config = {
    .base = DEVICE_BASE(CONSOLE_UART),
    .irq = DEVICE_INTR(CONSOLE_UART),
    .baud = 115200,
    .data_bits = SDRV_UART_CHAR_8_BITS,
    .stop_bits = SDRV_UART_STOP_1_BIT,
    .parity = SDRV_UART_NO_PARITY,
};

/* Set printf putchar interface */
void _putchar(char character)
{
    char r_value = '\r';

    if (character == '\n') {
        sdrv_uart_sync_transmit(&g_console_uart, (uint8_t *)&r_value, 1, NULL,
                                WaitForever);
    }

    sdrv_uart_sync_transmit(&g_console_uart, (uint8_t *)&character, 1, NULL,
                            WaitForever);
}

static scr_signal_t cr52_fault_chk_scr[] = {
#ifdef CORE_CR52P0
    SCR_SAFB_CR52P_SCR_FAULT_CHK_EN0,
#endif /* #ifdef CORE_R52P0 */
#ifdef CORE_CR52P1
    SCR_SAFB_CR52P_SCR_FAULT_CHK_EN1,
#endif /* #ifdef CORE_R52P1 */
#ifdef CORE_CR52P2
    SCR_SAFB_CR52P_SCR_FAULT_CHK_EN2,
#endif /* #ifdef CORE_R52P2 */
#ifdef CORE_CR52P3
    SCR_SAFB_CR52P_SCR_FAULT_CHK_EN3,
#endif /* #ifdef CORE_R52P3 */
};

static scr_signal_t core_debug_scr[] = {
#ifdef CORE_CR52P0
    SCR_LP_CR52P_DBGEN0, SCR_LP_CR52P_NIDEN0, SCR_LP_CR52P_HIDEN0, SCR_LP_CR52P_HNIDEN0,
#endif /* #ifdef CORE_R52P0 */
#ifdef CORE_CR52P1
    SCR_LP_CR52P_DBGEN1, SCR_LP_CR52P_NIDEN1, SCR_LP_CR52P_HIDEN1, SCR_LP_CR52P_HNIDEN1,
#endif /* #ifdef CORE_R52P1 */
#ifdef CORE_CR52P2
    SCR_LP_CR52P_DBGEN2, SCR_LP_CR52P_NIDEN2, SCR_LP_CR52P_HIDEN2, SCR_LP_CR52P_HNIDEN2,
#endif /* #ifdef CORE_R52P2 */
#ifdef CORE_CR52P3
    SCR_LP_CR52P_DBGEN3, SCR_LP_CR52P_NIDEN3, SCR_LP_CR52P_HIDEN3, SCR_LP_CR52P_HNIDEN3,
#endif /* #ifdef CORE_R52P3 */
#ifdef CORE_LP
    SCR_LP_CR5_LP_DBGEN0, SCR_LP_CR5_LP_NIDEN0,
#endif /* #ifdef CORE_LP */
#ifdef CORE_SE
    SCR_LP_CR5_SE_DBGEN0, SCR_LP_CR5_SE_NIDEN0,
#endif /* #ifdef CORE_SE */
};

void core_debug_enable(bool en)
{
    unsigned int i;

    /* scr sf controller */
    sdrv_scr_t scr_sf_ctrl = {
        .base = APB_SCR_SAETY_BOOT_BASE,
    };

    /* scr lp controller */
    sdrv_scr_t scr_lp_ctrl = {
        .base = APB_SCR_LP_BASE,
    };

    /* Disable CR52 lockstep fault check bit6 */
    for (i = 0; i < ARRAY_SIZE(cr52_fault_chk_scr); i++) {
        scr_set(&scr_sf_ctrl, &cr52_fault_chk_scr[i], 0x3F);
    }

    for (i = 0; i < ARRAY_SIZE(core_debug_scr); i++) {
        scr_set(&scr_lp_ctrl, &core_debug_scr[i], en);
    }

#ifdef CORE_SE
    RMWREG32(APB_ROMC_BASE + 0x5c, 0, 1, 1);
#endif

    /* Enable CR52 lockstep fault check bit6  */
    for (i = 0; i < ARRAY_SIZE(cr52_fault_chk_scr); i++) {
        scr_set(&scr_sf_ctrl, &cr52_fault_chk_scr[i], 0x7F);
    }
}

void board_reset_init(void)
{
#if NUM_OF_RSTGEN_SIGS
    uint32_t i = 0;

    for (i = 0; i < NUM_OF_RSTGEN_SIGS; i++) {
        sdrv_rstgen_reset(board_reset_array[i]);
    }
#endif /* #if NUM_OF_RSTGEN_SIGS */

#if CONFIG_RECOVERY_MODULE
    sdrv_recovery_module(&recovery_module_array);
#endif /* #if CONFIG_RECOVERY_MODULE */
}

#if CONFIG_INIT_MRAM
__WEAK Mram_HostConfigType g_default_mram_config_normal[] = {
    MRAM_HW_MRAM1_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
    MRAM_HW_MRAM2_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
    MRAM_HW_MRAM3_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
    MRAM_HW_MRAM4_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
    MRAM_HW_MRAM5_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
};
__WEAK Mram_HostConfigType g_default_mram_config_normal_bypass[] = {
    MRAM_HW_MRAM1_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
    MRAM_HW_MRAM2_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
    MRAM_HW_MRAM3_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
    MRAM_HW_MRAM4_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
    MRAM_HW_MRAM5_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
};
__WEAK Mram_HostConfigType g_default_mram_config_swap[] = {
    MRAM_HW_MRAM3_BASE_SWAP_MRAM1_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
    MRAM_HW_MRAM4_BASE_SWAP_MRAM2_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
    MRAM_HW_MRAM1_BASE_SWAP_MRAM3_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
    MRAM_HW_MRAM2_BASE_SWAP_MRAM4_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
    MRAM_HW_MRAM5_CONFIG_SETUP(true, MRAM_INIT_SW_AUTO_WAKEUP),
};
__WEAK Mram_HostConfigType g_default_mram_config_swap_bypass[] = {
    MRAM_HW_MRAM3_BASE_SWAP_MRAM1_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
    MRAM_HW_MRAM4_BASE_SWAP_MRAM2_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
    MRAM_HW_MRAM1_BASE_SWAP_MRAM3_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
    MRAM_HW_MRAM2_BASE_SWAP_MRAM4_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
    MRAM_HW_MRAM5_CONFIG_SETUP(true, MRAM_INIT_BYPASS),
};
__WEAK Mram_HostConfigType *g_default_mram_config = g_default_mram_config_normal;
__WEAK uint32_t num_of_default_mram_config = ARRAY_SIZE(g_default_mram_config_normal);

__WEAK Mram_RdBufferCfgType g_mram_rd_buffer_default_config[] = {
    {
        .enable = true,
        .channelId = 0,
        .masterId = MRAM_RDBUFFER_IGNOR_IDENT,
        .arid = MRAM_RDBUFFER_IGNOR_IDENT,
    },
    {
        .enable = true,
        .channelId = 1,
        .masterId = MRAM_RDBUFFER_IGNOR_IDENT,
        .arid = MRAM_RDBUFFER_IGNOR_IDENT,
    },
    {
        .enable = true,
        .channelId = 2,
        .masterId = MRAM_RDBUFFER_IGNOR_IDENT,
        .arid = MRAM_RDBUFFER_IGNOR_IDENT,
    },
    {
        .enable = true,
        .channelId = 3,
        .masterId = MRAM_RDBUFFER_IGNOR_IDENT,
        .arid = MRAM_RDBUFFER_IGNOR_IDENT,
    },
    {
        .enable = true,
        .channelId = 4,
        .masterId = MRAM_RDBUFFER_IGNOR_IDENT,
        .arid = MRAM_RDBUFFER_IGNOR_IDENT,
    }
};
__WEAK Mram_RdBufferCfgType *g_mram_rd_buffer_config = g_mram_rd_buffer_default_config;
__WEAK uint32_t num_of_default_mram_rd_buffer_config = ARRAY_SIZE(g_mram_rd_buffer_default_config);

__WEAK Mram_HostType g_mram_host[5];
__WEAK mram_context_t g_mram_context;

/**
 * @brief Initializes the MRAM.
 *
 * @return Initialization status:
 *         - SDRV_MRAM_RET_OK: Initialization success.
 *         - SDRV_MRAM_RET_FAIL: Initialization failed.
 */
int board_mram_init(void)
{
    int ret = SDRV_MRAM_RET_FAIL;
    if (num_of_default_mram_config == 0u) {
        /* Do nothing */
    }
    else if (SDRV_MRAM_RET_OK != sdrv_mram_init(&g_mram_context, g_mram_host,
                            g_default_mram_config, num_of_default_mram_config)) {
        /* Do nothing */
    }
    else {
        ret = SDRV_MRAM_RET_OK;
    }

    if ((num_of_default_mram_rd_buffer_config == 0u) || (ret != 0)) {
        /* Do nothing */
    }
    else if (SDRV_MRAM_RET_OK != sdrv_mram_rd_buffer_setup(&g_mram_context,
                                g_mram_rd_buffer_config, num_of_default_mram_rd_buffer_config)) {
        ret = SDRV_MRAM_RET_FAIL;
    }
    return ret;
}
#endif /* #if CONFIG_INIT_MRAM */

/* board debug console uart init */
void board_debug_console_init(void)
{
    scr_signal_t console_uart_dbg_en = SCR_CONSOLE_UART_DBG_EN(CONSOLE_UART);
    sdrv_scr_t scr_ctrl = {
        .base = APB_SCR_SAETY_BOOT_BASE
    };

    if (console_uart_config.irq <= UART2_INTR_NUM) {
        scr_ctrl.base = APB_SCR_LP_BASE;
        /* get console uart clk freq */
        console_uart_config.clk_freq = sdrv_ckgen_get_rate(CLK_NODE(g_ckgen_ip_uart1_2));
    } else {
        /* get console uart clk freq */
        console_uart_config.clk_freq = sdrv_ckgen_get_rate(CLK_NODE(g_ckgen_ip_uart3_20));
    }

    /* initializes uart controller */
    sdrv_uart_controller_init(&g_console_uart, &console_uart_config, NULL,
                              NULL);

    /* disable console uart debug mode */
    // scr_set(&scr_ctrl, &console_uart_dbg_en, 0);
}

/* MPU config regions */
#if CONFIG_ARCH_WITH_MPU
#if CONFIG_ARMV8R
/* For ARMV8R CR52P0, please modify the regions based on actual usage. */
__WEAK const mpu_config_t board_mpu_config[] = {

    /* cram region, normal */
    {CRAM1_BASE, 0x100000, MPU_REGION_NORMAL},

    /* the first 16KB of iram1 region (0x00A00000 ~ 0x00A03FFF), normal */
    {IRAM1_BASE, 0x4000, MPU_REGION_NORMAL},

    /* iram1-3 region (0x00A04000 ~ 0x00CFFFFF), normal.
     * If the program runs in IRAM, the range of the exception vector table
     * (0x00A04000 to 0x00A04080) should also be configured as read-only.
     * For example:
     * {IRAM1_BASE + 0x4000, 0x80, MPU_REGION_NORMAL_RO},
     * {IRAM1_BASE + 0x4000 + 0x80, 0x2FBF80, MPU_REGION_NORMAL},
     */
    {IRAM1_BASE + 0x4000, 0x2FC000, MPU_REGION_NORMAL},

    /* iram lp region, normal */
    {IRAM_LP_BASE, 0x20000, MPU_REGION_NORMAL},

    /* MRAM1-4 region, for MRAM flash port, read only */
    {MRAM1_F_BASE, 0x1000000, MPU_REGION_NORMAL_RO},

    /* MRAM5 region, for MRAM flash port, read only */
    {MRAM5_F_BASE, 0x80000, MPU_REGION_NORMAL_RO},

    /* MRAM5 region, for MRAM master port, normal non-cacheable */
    {MRAM5_M_BASE, 0x80000, MPU_REGION_NORMAL_NONCACHEABLE},

    /* IP module, device */
    {0xF0000000, 0x10000000, MPU_REGION_DEVICE},

#if defined(CONFIG_ARM_WITH_TCM) && defined(CONFIG_ARM_TCM_BASE)
    /* intr vector, read only */
    {CONFIG_ARM_TCM_BASE, 0x80, MPU_REGION_NORMAL_RO},
    /* tcm region, normal */
    {CONFIG_ARM_TCM_BASE + 0x80, 0x1FF80, MPU_REGION_NORMAL},
#endif /* #if CONFIG_ARM_WITH_TCM */

};

#else /* #if CONFIG_ARMV8R */

/* For ARMV7R CR5, please modify the regions based on actual usage. */
__WEAK const mpu_config_t board_mpu_config[] = {
    /* background region, 4GB size, not accessable. */
    {0, 0x100000000, MPU_REGION_NO_ACCESS},

    /* cram se region, normal */
    {CRAM1_BASE, 0x100000, MPU_REGION_NORMAL},

    /* iram1-2 region, normal */
    {IRAM1_BASE, 0x200000, MPU_REGION_NORMAL},

    /* iram3 region, normal */
    {IRAM3_BASE, 0x100000, MPU_REGION_NORMAL},

    /* MRAM1-4 region, for MRAM flash port, read only */
    {MRAM1_F_BASE, 0x1000000, MPU_REGION_NORMAL_RO},

    /* MRAM5 region, for MRAM flash port, normal non-cacheable */
    {MRAM5_F_BASE, 0x80000, MPU_REGION_NORMAL_NONCACHEABLE},

    /* iram lp region, normal */
    {IRAM_LP_BASE, 0x20000, MPU_REGION_NORMAL},

    /* IP module, device */
    {0xF0000000, 0x10000000, MPU_REGION_DEVICE},

#if CONFIG_ARMV7R_USE_TCMA
    /* tcm region, normal */
    {CONFIG_ARMV7R_TCMA_BASE, 0x10000, MPU_REGION_NORMAL},
#endif

    /* int vector, read only */
    {(addr_t)&__vector, 0x40, MPU_REGION_NORMAL_RO},

    /* int vector, read only */
    {0, 0x40, MPU_REGION_NORMAL_RO},
};
#endif /* #if CONFIG_ARMV8R */

__WEAK uint32_t num_of_mpu_cfg = ARRAY_SIZE(board_mpu_config);

__WEAK void board_mpu_init(void)
{
    uint32_t index = 0;
    uint32_t val = arm_read_sctlr();

    if (val & SCTLR_C) {
        arch_disable_cache(DCACHE);
    }

    if (val & SCTLR_I) {
        arch_disable_cache(ICACHE);
    }

    mpu_enable(false);

#if CONFIG_ARMV7R
    mpu_clear_region();
#endif

#if CONFIG_ARMV8R
    mpu_init();
#endif

    for (uint32_t i = 0; i < num_of_mpu_cfg; i++) {
        const mpu_config_t *config = &board_mpu_config[i];
        if (config->size > 0) {
            mpu_add_region(index++, config->addr, config->size, config->type);
        }
    }

    if (index > 0) {
        mpu_enable(true);
    }

    if (val & SCTLR_I) {
        arch_enable_cache(ICACHE);
    }

    if (val & SCTLR_C) {
        arch_enable_cache(DCACHE);
    }
}
#endif

/*
 * Call constructor functions.
 * This is required when the project has C++ modules.
 */
void call_constructors(void)
{
#if __GNUC__
    extern void (*__ctor_list[])(void);
    extern void (*__ctor_end[])(void);
    void (**ctor)(void);

    for (ctor = __ctor_list; ctor != __ctor_end; ctor++)
        (*ctor)();
#elif __ICCARM__
    extern void __iar_dynamic_initialization(void);
    __iar_dynamic_initialization();
#endif
}

void copy_intvec(void)
{
#if ((CONFIG_ARMV7R_USE_TCMA) && (CONFIG_ARMV7R_TCMA_BASE == 0x0))

    if ((uint32_t)(&__vector) != (uint32_t)0x0) {
        /* copy intvec to tcm */
        volatile uint64_t *p_dst = (uint64_t *)0x0;
        volatile uint64_t *p_src = (uint64_t *)(&__vector);

        for (int i = 0; i < 8; i++) {
            *p_dst++ = *p_src++;
        }
    }

#endif

#if (CONFIG_ARMV8R_COPY_INTVEC && CONFIG_ARM_WITH_TCM)

    /* TCM address should be 32-byte aligned. */
    if ((uint32_t)(&__hyp_vector) != (uint32_t)CONFIG_ARM_TCM_BASE) {
        /* copy intvec to tcm */
        volatile uint64_t *p_dst = (uint64_t *)CONFIG_ARM_TCM_BASE;
        volatile uint64_t *p_src = (uint64_t *)(&__hyp_vector);

        for (int i = 0; i < 16; i++) {
            *p_dst++ = *p_src++;
        }
    }

    /* System vector address equals hypervisor vector address plus 0x20 offset */
    arm_write_vbar((uint32_t)CONFIG_ARM_TCM_BASE + 0x20U);
#endif /* #if (CONFIG_ARMV8R_COPY_INTVEC && CONFIG_ARM_WITH_TCM) */
}
