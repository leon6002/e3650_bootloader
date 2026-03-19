#include <reset_ip.h>

#include "reset_cfg.h"

const sdrv_rstgen_sig_t *board_reset_array[NUM_OF_RSTGEN_SIGS] = {
    &rstsig_uart15,
    &rstsig_uart18,
    &rstsig_uart19,
    &rstsig_dma_sf1_rst0,
    &rstsig_dma_sf1_rst1,
};
