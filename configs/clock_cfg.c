#include <clock_ip.h>
#include <sdrv_ckgen.h>

const sdrv_ckgen_config_t g_clock_config = {
    .pre_bus_config = NULL,
    .pll_config = &g_pll_config,
    .bus_config = &g_bus_config,
    .core_config = NULL,
    .ip_config = &g_ip_config,
    .gating_config = NULL,
    .enable_config = NULL,
};
