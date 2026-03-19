#include "pinmux_cfg.h"

const pin_settings_config_t g_pin_init_config[NUM_OF_CONFIGURED_PINS] = {
    /* Pin 100, GPIO_E4, Mux:UART15.TXD */
    {
        .pin_index         =  GPIO_E4,
        .mux               =  PIN_MUX_ALT4,
        .open_drain        =  PIN_PUSH_PULL,
        .pull_config       =  PIN_PULL_UP,
        .drive_strength    =  PIN_DS_2MA,
        .slew_rate         =  PIN_SR_FAST,
        .input_select      =  PIN_IS_CMOS,
        .data_direction    =  PIN_OUTPUT_DIRECTION,
        .interrupt_config  =  PIN_INTERRUPT_DISABLED,
        .initial_value     =  PIN_LEVEL_LOW,
        .force_input       =  PIN_FORCE_INPUT_NORMAL,
        .mode_select       =  PIN_MODE_DIGITAL,
    },

    /* Pin 101, GPIO_E5, Mux:UART15.RXD */
    {
        .pin_index         =  GPIO_E5,
        .mux               =  PIN_MUX_ALT4,
        .open_drain        =  PIN_PUSH_PULL,
        .pull_config       =  PIN_PULL_UP,
        .drive_strength    =  PIN_DS_2MA,
        .slew_rate         =  PIN_SR_FAST,
        .input_select      =  PIN_IS_CMOS_SCHMITT,
        .data_direction    =  PIN_OUTPUT_DIRECTION,
        .interrupt_config  =  PIN_INTERRUPT_DISABLED,
        .initial_value     =  PIN_LEVEL_LOW,
        .force_input       =  PIN_FORCE_INPUT_NORMAL,
        .mode_select       =  PIN_MODE_DIGITAL,
    },

    /* Pin 166, GPIO_M6, Mux:UART18.TXD */
    {
        .pin_index         =  GPIO_M6,
        .mux               =  PIN_MUX_ALT4,
        .open_drain        =  PIN_PUSH_PULL,
        .pull_config       =  PIN_NOPULL,
        .drive_strength    =  PIN_DS_2MA,
        .slew_rate         =  PIN_SR_FAST,
        .input_select      =  PIN_IS_CMOS,
        .data_direction    =  PIN_OUTPUT_DIRECTION,
        .interrupt_config  =  PIN_INTERRUPT_DISABLED,
        .initial_value     =  PIN_LEVEL_LOW,
        .force_input       =  PIN_FORCE_INPUT_NORMAL,
        .mode_select       =  PIN_MODE_DIGITAL,
    },

    /* Pin 167, GPIO_M7, Mux:UART18.RXD */
    {
        .pin_index         =  GPIO_M7,
        .mux               =  PIN_MUX_ALT4,
        .open_drain        =  PIN_PUSH_PULL,
        .pull_config       =  PIN_NOPULL,
        .drive_strength    =  PIN_DS_2MA,
        .slew_rate         =  PIN_SR_FAST,
        .input_select      =  PIN_IS_CMOS_SCHMITT,
        .data_direction    =  PIN_OUTPUT_DIRECTION,
        .interrupt_config  =  PIN_INTERRUPT_DISABLED,
        .initial_value     =  PIN_LEVEL_LOW,
        .force_input       =  PIN_FORCE_INPUT_NORMAL,
        .mode_select       =  PIN_MODE_DIGITAL,
    },

    /* Pin 142, GPIO_S14, Mux:UART19.TXD */
    {
        .pin_index         =  GPIO_S14,
        .mux               =  PIN_MUX_ALT4,
        .open_drain        =  PIN_PUSH_PULL,
        .pull_config       =  PIN_NOPULL,
        .drive_strength    =  PIN_DS_2MA,
        .slew_rate         =  PIN_SR_FAST,
        .input_select      =  PIN_IS_CMOS,
        .data_direction    =  PIN_OUTPUT_DIRECTION,
        .interrupt_config  =  PIN_INTERRUPT_DISABLED,
        .initial_value     =  PIN_LEVEL_LOW,
        .force_input       =  PIN_FORCE_INPUT_NORMAL,
        .mode_select       =  PIN_MODE_DIGITAL,
    },

    /* Pin 143, GPIO_S15, Mux:UART19.RXD */
    {
        .pin_index         =  GPIO_S15,
        .mux               =  PIN_MUX_ALT4,
        .open_drain        =  PIN_PUSH_PULL,
        .pull_config       =  PIN_NOPULL,
        .drive_strength    =  PIN_DS_2MA,
        .slew_rate         =  PIN_SR_FAST,
        .input_select      =  PIN_IS_CMOS_SCHMITT,
        .data_direction    =  PIN_OUTPUT_DIRECTION,
        .interrupt_config  =  PIN_INTERRUPT_DISABLED,
        .initial_value     =  PIN_LEVEL_LOW,
        .force_input       =  PIN_FORCE_INPUT_NORMAL,
        .mode_select       =  PIN_MODE_DIGITAL,
    },
};
