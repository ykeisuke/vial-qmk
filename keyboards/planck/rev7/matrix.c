/*
 * Copyright 2018-2023 Jack Humbert <jack.humb@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gpio.h"
#include "hal_pal.h"
#include "hal_pal_lld.h"
#include "quantum.h"
#include <math.h>
#include "encoder.h"

// STM32-specific watchdog config calculations
// timeout = 31.25us * PR * (RL + 1)
#define _STM32_IWDG_LSI(us) ((us) / 31.25)
#define STM32_IWDG_PR_US(us) (uint8_t)(log(_STM32_IWDG_LSI(us)) / log(2) - 11)
#define STM32_IWDG_PR_MS(s) STM32_IWDG_PR_US(s * 1000.0)
#define STM32_IWDG_PR_S(s) STM32_IWDG_PR_US(s * 1000000.0)
#define _STM32_IWDG_SCALAR(us) (2 << ((uint8_t)STM32_IWDG_PR_US(us) + 1))
#define STM32_IWDG_RL_US(us) (uint64_t)(_STM32_IWDG_LSI(us)) / _STM32_IWDG_SCALAR(us)
#define STM32_IWDG_RL_MS(s) STM32_IWDG_RL_US(s * 1000.0)
#define STM32_IWDG_RL_S(s) STM32_IWDG_RL_US(s * 1000000.0)

#if !defined(PLANCK_WATCHDOG_TIMEOUT)
#   define PLANCK_WATCHDOG_TIMEOUT 1.0
#endif

#if !defined(ENCODER_SETTLE_PIN_DELAY)
#   define ENCODER_SETTLE_PIN_DELAY 20
#endif

/* matrix state(1:on, 0:off) */
static pin_t matrix_row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static pin_t matrix_col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

static matrix_row_t matrix_inverted[MATRIX_COLS];


#ifdef ENCODER_ENABLE
int8_t  encoder_LUT[]     = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
uint8_t encoder_state[8]  = {0};
int8_t  encoder_pulses[8] = {0};
uint8_t encoder_value[8]  = {0};
#endif

void matrix_init_custom(void) {
    // actual matrix setup - cols
    for (int i = 0; i < MATRIX_COLS; i++) {
        setPinOutput(matrix_col_pins[i]);
        writePinLow(matrix_col_pins[i]);
    }

    // rows
    for (int i = 0; i < MATRIX_ROWS; i++) {
        setPinInputLow(matrix_row_pins[i]);
    }

    // encoder A & B setup
    setPinInputLow(B12);
    setPinInputLow(B13);

#ifndef PLANCK_WATCHDOG_DISABLE
    wdgInit();

    static WDGConfig wdgcfg;
    wdgcfg.pr   = STM32_IWDG_PR_S(PLANCK_WATCHDOG_TIMEOUT);
    wdgcfg.rlr  = STM32_IWDG_RL_S(PLANCK_WATCHDOG_TIMEOUT);
    wdgcfg.winr = STM32_IWDG_WIN_DISABLED;
    wdgStart(&WDGD1, &wdgcfg);
#endif
}


bool matrix_scan_custom(matrix_row_t current_matrix[]) {
#ifndef PLANCK_WATCHDOG_DISABLE
    // reset watchdog
    wdgReset(&WDGD1);
#endif

    bool changed = false;

    // actual matrix
    for (int col = 0; col < MATRIX_COLS; col++) {
        matrix_row_t data = 0;

        // strobe col
        gpio_write_pin_high(matrix_col_pins[col]);

        // need wait to settle pin state
        wait_us(ENCODER_SETTLE_PIN_DELAY);

        // read row data
        for (int row = 0; row < MATRIX_ROWS; row++) {
            data |= (gpio_read_pin(matrix_row_pins[row]) << row);
        }

        // unstrobe col
        gpio_write_pin_low(matrix_col_pins[col]);

        if (matrix_inverted[col] != data) {
            matrix_inverted[col] = data;
        }
    }

    for (int row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t old = current_matrix[row];
        current_matrix[row] = 0;
        for (int col = 0; col < MATRIX_COLS; col++) {
            current_matrix[row] |= ((matrix_inverted[col] & (1 << row) ? 1 : 0) << col);
        }
        changed |= old != current_matrix[row];
    }

    return changed;
}


#define ENCODER_CLOCKWISE true
#define ENCODER_COUNTER_CLOCKWISE false

static void encoder_handle_state_change(uint8_t index, uint8_t state) {
    uint8_t i = index;

    const uint8_t resolution = ENCODER_RESOLUTION;

    encoder_pulses[i] += encoder_LUT[state & 0xF];

    if (encoder_pulses[i] >= resolution) {
        //printf("encoder_queue_event: %d, %d\n", index, ENCODER_COUNTER_CLOCKWISE);
        encoder_queue_event(index, ENCODER_COUNTER_CLOCKWISE);
    }

    // direction is arbitrary here, but this clockwise
    if (encoder_pulses[i] <= -resolution) {
        //printf("encoder_queue_event: %d, %d\n", index, ENCODER_CLOCKWISE);
        encoder_queue_event(index, ENCODER_CLOCKWISE);
    }
    encoder_pulses[i] %= resolution;

}

void encoder_driver_task(void) {

    // set up C/rows for encoder read
    for (int i = 0; i < MATRIX_ROWS; i++) {
        gpio_set_pin_output(matrix_row_pins[i]);
        gpio_write_pin_high(matrix_row_pins[i]);
    }

    // set up A & B for reading
    gpio_set_pin_input_high(B12);
    gpio_set_pin_input_high(B13);

    for (int i = 0; i < MATRIX_ROWS; i++) {
        gpio_write_pin_low(matrix_row_pins[i]);
        wait_us(ENCODER_SETTLE_PIN_DELAY);
        uint8_t new_status = (palReadPad(GPIOB, 12) << 0) | (palReadPad(GPIOB, 13) << 1);
        if ((encoder_state[i] & 0x3) != new_status) {
            encoder_state[i] <<= 2;
            encoder_state[i] |= new_status;
            encoder_handle_state_change(i, encoder_state[i]);
        }
        gpio_write_pin_high(matrix_row_pins[i]);
    }

    // revert A & B to matrix state
    gpio_set_pin_input_low(B12);
    gpio_set_pin_input_low(B13);

    // revert C/rows to matrix state
    for (int i = 0; i < MATRIX_ROWS; i++) {
        gpio_set_pin_input_low(matrix_row_pins[i]);
    }

}
