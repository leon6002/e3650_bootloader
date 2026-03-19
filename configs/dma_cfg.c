/** ************************************************************************************************
 * SEMIDRIVE Copyright Statement * Copyright (c) SEMIDRIVE. All rights reserved
 **
 *                                                                                                 *
 * This software and all rights therein are owned by SEMIDRIVE, and are *
 * protected by copyright law and other relevant laws, regulations and *
 * protection. Without SEMIDRIVE's prior written consent and/or related rights,
 ** please do not use this software or any potion thereof in any form or by any
 ** means. You may not reproduce, modify or distribute this software except in *
 * compliance with the License. Unless required by applicable law or agreed to *
 * in writing, software distributed under the License is distributed on * an 'AS
 *IS' basis, WITHOUT WARRANTIES OF ANY KIND, either express or implied. *
 *                                                                                                 *
 **************************************************************************************************/

#include "dma_cfg.h"
#include "regs_base.h"
#include "sdrv_dma.h"
#if CONFIG_IRQ
#include "irq.h"
#include "irq_num.h"
#endif

sdrv_dma_channel_group_t dma1_ch0_ch3_group = {
    .dma = (paddr_t)APB_DMA_SF1A_BASE,
    .id = SDRV_DMA_GROUP_CH_0_1_2_3,
#if CONFIG_IRQ
    .irq_num = DMA1_PORT0_CH_0_1_2_3_INTR_NUM,
#endif
    .vmid = 0,
};
