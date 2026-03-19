#include <irq_num.h>
#include <regs_base.h>

#include "mbox_cfg.h"

/* mbox pre-alloc share memory for each processor
 * Tx buffer base address must keep consistent with
 * the start address of the "mb_shm" segment in link script */
#define MBOX_TX_BUFF_BASE (0x00CF0000)

sdrv_mbox_t g_sdrv_mbox_dev;

sdrv_mbox_config_t sdrv_mbox_cfg = {
    .base = MB_REG_BASE,
    .mem_base = MBOX_TX_BUFF_BASE,
    .irq = MB_MU_MESSAGE_READY_0_INTR_NUM,
    .wakeup_irq = MB_MU_WAKEUP_0_INTR_NUM,
};
