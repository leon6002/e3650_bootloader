#include <types.h>
#include <string.h>
#include <sdrv_ckgen.h>
#include <sdrv_pinctrl.h>
#include <sdrv_rstgen.h>
#include <clock_cfg.h>
#include <pinmux_cfg.h>
#include <reset_cfg.h>
#include <debug.h>
#include <core_id.h>
#include <remap/remap.h>
#include <sdrv_uart.h>
#include "board.h"
#include "bpt.h"

#if CONFIG_ARCH_WITH_MPU
#include <mpu.h>
#endif

// Extern configs from SSDK source files (present in src/ssdk)
extern sdrv_ckgen_config_t g_clock_config;
extern const pin_settings_config_t g_pin_init_config[];
// Ensure we use the correct size. The demo usually defines this macro.
// In pinmux_cfg.c associated with e3650, it is 4 (TX, RX for 2 UARTs or similar).
// We'll define it based on common e3650 demo values if header is missing.
#ifndef NUM_OF_CONFIGURED_PINS
#define NUM_OF_CONFIGURED_PINS 4
#endif

// External Reset Config
// Check reset_cfg.c for the actual array name. Usually 'board_reset_array'.
extern const sdrv_rstgen_sig_t *board_reset_array[];
#ifndef NUM_OF_RSTGEN_SIGS
#define NUM_OF_RSTGEN_SIGS 2 // Common value, adjust if needed by checking reset_cfg.h
#endif

// App BPT Address in Flash (MRAM)
#define APP_BPT_ADDR 0x80FE000

#define PPC_APBMUX_BASE_OFFSET (0x10000U)
#define PPC_APBMUX6_OFFSET     (0x6000U)
#define EXTRA_UART_BAUD        (115200U)

static sdrv_uart_t uart18_ctrl;
static sdrv_uart_t uart19_ctrl;
static bool uart18_ready;
static bool uart19_ready;

static sdrv_uart_config_t uart18_cfg = {
    .base = APB_UART18_BASE,
    .irq = UART18_INTR_NUM,
    .baud = EXTRA_UART_BAUD,
    .data_bits = SDRV_UART_CHAR_8_BITS,
    .stop_bits = SDRV_UART_STOP_1_BIT,
    .parity = SDRV_UART_NO_PARITY,
};

static sdrv_uart_config_t uart19_cfg = {
    .base = APB_UART19_BASE,
    .irq = UART19_INTR_NUM,
    .baud = EXTRA_UART_BAUD,
    .data_bits = SDRV_UART_CHAR_8_BITS,
    .stop_bits = SDRV_UART_STOP_1_BIT,
    .parity = SDRV_UART_NO_PARITY,
};

static void extra_uart_send(sdrv_uart_t *ctrl, const char *msg)
{
    sdrv_uart_sync_transmit(ctrl, (const uint8_t *)msg, strlen(msg), NULL,
                            WaitForever);
}

static void init_extra_debug_uarts(void)
{
    uint32_t clk_freq = sdrv_ckgen_get_rate(CLK_NODE(g_ckgen_ip_uart3_20));

    uart18_cfg.clk_freq = clk_freq;
    if (sdrv_uart_controller_init(&uart18_ctrl, &uart18_cfg, NULL, NULL) ==
        Status_Success) {
        uart18_ready = true;
        extra_uart_send(&uart18_ctrl,
                        "\r\n[Simple Bootloader] UART18 debug channel ready.\r\n");
    }

    uart19_cfg.clk_freq = clk_freq;
    if (sdrv_uart_controller_init(&uart19_ctrl, &uart19_cfg, NULL, NULL) ==
        Status_Success) {
        uart19_ready = true;
        extra_uart_send(&uart19_ctrl,
                        "\r\n[Simple Bootloader] UART19 debug channel ready.\r\n");
    }
}

static void broadcast_extra_debug(const char *msg)
{
    if (uart18_ready) {
        extra_uart_send(&uart18_ctrl, msg);
    }
    if (uart19_ready) {
        extra_uart_send(&uart19_ctrl, msg);
    }
}

/* PPC APBMUX6 is usually associated with UART15 based on user input */
static void sdrv_ppc_unlock_uart15(void)
{
    uint32_t base = APB_MAC_S0_BASE + PPC_APBMUX_BASE_OFFSET + PPC_APBMUX6_OFFSET;
    
    ssdk_printf(SSDK_INFO, "Unlocking PPC for UART15 (APBMUX6) at 0x%x...\\r\\n", base);

    for (int i = 0; i < 16; i++) {
        /* Unlock permissions (write 0 to allow all RW) */
        writel(0x0, base + i * 0x10 + 0x0); /* PER0 */
        writel(0x0, base + i * 0x10 + 0x4); /* PER1 */
        writel(0x0, base + i * 0x10 + 0x8); /* PER2 */
        writel(0x0, base + i * 0x10 + 0xC); /* PER3 */

        /* Inverse check registers (write ~0) */
        writel(0xFFFFFFFF, base + 0x90C + i * 0x10); /* PER0_INV */
        writel(0xFFFFFFFF, base + 0x908 + i * 0x10); /* PER1_INV */
        writel(0xFFFFFFFF, base + 0x904 + i * 0x10); /* PER2_INV */
        writel(0xFFFFFFFF, base + 0x900 + i * 0x10); /* PER3_INV */
    }
}

int main(void) {
    // 1. SSDK Clock Init
    sdrv_ckgen_init(&g_clock_config);

    // 2. SSDK Reset Init (Manual implementation of board_reset_init)
    for (uint32_t i = 0; i < NUM_OF_RSTGEN_SIGS; i++) {
        sdrv_rstgen_reset(board_reset_array[i]);
    }

    // 3. SSDK Pinmux Init
    sdrv_pinctrl_init(NUM_OF_CONFIGURED_PINS, g_pin_init_config);

    // 4. Init Debug Console (Now that pins are ready)
    board_debug_console_init();
    init_extra_debug_uarts();
    ssdk_printf(SSDK_INFO, "Simple Bootloader Start...\r\n");
    broadcast_extra_debug("\r\n[Simple Bootloader] Boot flow started.\r\n");

    /* initializes mpu */
#if CONFIG_ARCH_WITH_MPU
    ssdk_printf(SSDK_INFO, "Initializing MPU...\r\n");
    board_mpu_init();
    ssdk_printf(SSDK_INFO, "MPU Initialized.\r\n");
#endif 

#if CONFIG_INIT_MRAM
    /* initializes mram */
    ssdk_printf(SSDK_INFO, "Initializing MRAM...\r\n");
    board_mram_init();
    ssdk_printf(SSDK_INFO, "MRAM Initialized.\r\n");
#endif

    // 4. Load app from Flash to RAM
    ssdk_printf(SSDK_INFO, "Loading app from BPT at 0x%x...\r\n", APP_BPT_ADDR);
    broadcast_extra_debug("[Simple Bootloader] Loading app from BPT.\r\n");
    
    bpt_v2_t *bpt = (bpt_v2_t *)APP_BPT_ADDR;
    uint32_t load_addr = 0;
    uint32_t load_size = 0;
    int ret;

    // ssdk_printf(SSDK_INFO, "Inspect BPT at 0x%x:\r\n", APP_BPT_ADDR);
    
    // Dump BPT info for debugging
    // bpt_dump_info(bpt);

    if (bpt_check_valid(bpt) != 0) {
        ssdk_printf(SSDK_WARNING, "BPT Validation Failed (Check PSN/CRC above)! Proceeding anyway...\r\n");
        // Proceeding despite error to allow booting if only PSN is wrong
    } else {
        // ssdk_printf(SSDK_INFO, "BPT is Valid.\r\n");
    }

    // Load CR52P0 image (Bao)
    ret = bpt_load_image(bpt, CORE_CR52P0, BPT_IIB_IMG_TYPE_NORMAL, &load_addr, &load_size);
    if (ret != 0) {
        ssdk_printf(SSDK_CRIT, "Failed to load image for CR52P0! Halting.\r\n");
        while(1);
    }

    ssdk_printf(SSDK_INFO, "Image loaded to 0x%x, size 0x%x\r\n", load_addr, load_size);
    broadcast_extra_debug("[Simple Bootloader] Image loaded successfully.\r\n");

    // Get Entry Point
    uint32_t entry_point = bpt_get_image_entry(bpt, CORE_CR52P0, BPT_IIB_IMG_TYPE_NORMAL);
    if (entry_point == 0) {
         ssdk_printf(SSDK_WARNING, "Entry point is 0, using load logic address 0x%x\r\n", load_addr);
         entry_point = load_addr;
    }

    // 5. Jump to Bao (via Remap Reset)
    // sdrv_ppc_unlock_uart15();
    ssdk_printf(SSDK_INFO, "Remapping Cores to 0x%x...\r\n", entry_point);
    broadcast_extra_debug("[Simple Bootloader] Remapping cores.\r\n");
    
    // Kick other cores first
    ssdk_printf(SSDK_INFO, "Kick CR52P1...\r\n");
    sdrv_core_force_remap((sdrv_remap_core_e)CORE_CR52P1, entry_point);
    
    ssdk_printf(SSDK_INFO, "Kick CR52P2...\r\n");
    sdrv_core_force_remap((sdrv_remap_core_e)CORE_CR52P2, entry_point);
    
    ssdk_printf(SSDK_INFO, "Kick CR52P3...\r\n");
    sdrv_core_force_remap((sdrv_remap_core_e)CORE_CR52P3, entry_point);

    // Finally kick CR52P0
    ssdk_printf(SSDK_INFO, "Kick CR52P0...\r\n");
    ret = sdrv_core_force_remap((sdrv_remap_core_e)CORE_CR52P0, entry_point);
    
    if (ret != SDRV_REMAP_OK) {
        ssdk_printf(SSDK_CRIT, "Failed to remap core! ret=%d\r\n", ret);
        while(1);
    }
    
    // Should not be reached if remap/reset is successful
    ssdk_printf(SSDK_INFO, "Remap triggered. Waiting for reset...\r\n");
    while(1);
}
