// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "i2c_master.h"
#include "mcp23017.h"
#include "pibookintosh.h"

/* Pin definitions for LED */
#define LED_PIN GP25

/* ===== QMK Callbacks ===== */

/**
 * Keyboard pre-initialization callback
 * Runs very early - good for GPIO/I2C/device init
 */
void keyboard_pre_init_kb(void) {
    pibookintosh_pre_init_kb();
    keyboard_pre_init_user();
}

/**
 * Matrix initialization callback
 * Initializes the keyboard matrix state
 */
void matrix_init(void) {
    pibookintosh_matrix_init();
}

/**
 * Matrix scan callback
 * Called ~100x/sec to scan keyboard matrix and LEDs
 */
uint8_t matrix_scan(void) {
    return pibookintosh_matrix_scan();
}

/**
 * Get row data callback
 * Returns the current state of a matrix row
 */
matrix_row_t matrix_get_row(uint8_t row) {
    return pibookintosh_matrix_get_row(row);
}

/**
 * LED update callback
 * Handles Caps Lock and other LED state changes
 */
bool led_update_kb(led_t led_state) {
    pibookintosh_set_caps_led(led_state.caps_lock);
    return false;  // Skip default LED handling
}

/**
 * Matrix print callback
 * Used for debugging matrix state
 */
void matrix_print(void) {
    // Minimal implementation for debugging support
}
