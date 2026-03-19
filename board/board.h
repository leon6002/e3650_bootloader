/**
 * @file board.h
 * @brief board config header.
 *
 * @copyright Copyright (c) 2022  Semidrive Semiconductor.
 *            All rights reserved.
 */
#ifndef BOARD_H_
#define BOARD_H_

#include <sdrv_uart.h>
#include <scr_hw.h>
#if CONFIG_INIT_MRAM
#include <mram_hw.h>
#include <sdrv_mram.h>
#endif

/* PRQA S 0602, 0603 EOF */

#define DEVICE_BASE(dev) _DEVICE_BASE(dev)
#define _DEVICE_BASE(dev) APB_##dev##_BASE

#define DEVICE_INTR(dev) _DEVICE_INTR(dev)
#define _DEVICE_INTR(dev) dev##_INTR_NUM

#define SCR_UART1_DBG_EN  SCR_LP_SOC_DBG_GASKET_LP_IP_DBG_EN_UART1
#define SCR_UART2_DBG_EN  SCR_LP_SOC_DBG_GASKET_LP_IP_DBG_EN_UART2
#define SCR_UART3_DBG_EN  SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART3
#define SCR_UART4_DBG_EN  SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART4
#define SCR_UART5_DBG_EN  SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART5
#define SCR_UART6_DBG_EN  SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART6
#define SCR_UART7_DBG_EN  SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART7
#define SCR_UART8_DBG_EN  SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART8
#define SCR_UART9_DBG_EN  SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART9
#define SCR_UART10_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART10
#define SCR_UART11_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART11
#define SCR_UART12_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART12
#define SCR_UART13_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART13
#define SCR_UART14_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART14
#define SCR_UART15_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART15
#define SCR_UART16_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART16
#define SCR_UART17_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART17
#define SCR_UART18_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART18
#define SCR_UART19_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART19
#define SCR_UART20_DBG_EN SCR_SAFB_SOC_DBG_GASKET_SF_IP_DBG_EN_UART20

#define SCR_CONSOLE_UART_DBG_EN(uart) _SCR_CONSOLE_UART_DBG_EN(uart)
#define _SCR_CONSOLE_UART_DBG_EN(uart) SCR_##uart##_DBG_EN

extern sdrv_uart_t g_console_uart;

#if CONFIG_INIT_MRAM
extern Mram_HostConfigType g_default_mram_config_normal[5];
extern Mram_HostConfigType g_default_mram_config_normal_bypass[5];
extern Mram_HostConfigType g_default_mram_config_swap[5];
extern Mram_HostConfigType g_default_mram_config_swap_bypass[5];
extern Mram_HostConfigType *g_default_mram_config;
extern uint32_t num_of_default_mram_config;
extern Mram_RdBufferCfgType *g_mram_rd_buffer_config;
extern uint32_t num_of_default_mram_rd_buffer_config;
extern Mram_HostType g_mram_host[5];
extern mram_context_t g_mram_context;
#endif

/**
 * @brief enable core bug
 */
void core_debug_enable(bool en);

/**
 * @brief board reset init
 */
void board_reset_init(void);

/**
 * @brief board debug console uart init
 */
void board_debug_console_init(void);

#if CONFIG_ARCH_WITH_MPU
/**
 * @brief board memory protect unit (mpu) init
 */
__WEAK void board_mpu_init(void);
#endif

/*
 * Call constructor functions.
 * This is required when the project has C++ modules.
 */
void call_constructors(void);

#if CONFIG_INIT_MRAM
/**
 * @brief Initializes the MRAM.
 *
 * @return Initialization status:
 *         - SDRV_MRAM_RET_OK: Initialization success.
 *         - SDRV_MRAM_RET_FAIL: Initialization failed.
 */
int board_mram_init(void);
#endif /* #if CONFIG_INIT_MRAM */
#endif /* BOARD_H_ */
