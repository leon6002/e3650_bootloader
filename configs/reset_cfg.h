#ifndef SDRV_RESET_CFG_H_
#define SDRV_RESET_CFG_H_

#include <part.h>
#include <reset_ip.h>
#include <sdrv_rstgen.h>

#define NUM_OF_RSTGEN_SIGS 5

extern const sdrv_rstgen_sig_t *board_reset_array[NUM_OF_RSTGEN_SIGS];

#endif /* SDRV_RESET_CFG_H_ */
