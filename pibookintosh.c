// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "i2c_master.h"
#include "mcp23017.h"
#include "pibookintosh.h"

/* Pin definitions */
#define LED_PIN           GP25
#define COL_PINS          {GP6, GP7, GP8, GP9, GP10, GP11, GP12, GP13}
#define META_PINS         {GP14, GP15, GP16, GP17, GP18, GP19, GP20}
#define NUM_COLS          8
#define NUM_META_KEYS     7

static const pin_t column_pins[NUM_COLS] = COL_PINS;
static const pin_t meta_pins[NUM_META_KEYS] = META_PINS;
static matrix_row_t pibookintosh_matrix[MATRIX_ROWS] __attribute__((used));

#define ACTIVE_HIGH true
#define ACTIVE_LOW false

/* ===== Helper Functions ===== */

/* Pre-initialization: I2C, MCP23017, and GPIO setup */
void pibookintosh_pre_init_kb(void) {
    // Initialize I2C
    i2c_init();
    
    // Initialize MCP23017
    mcp23017_init(ACTIVE_LOW);
    
    // Initialize pico LED
    gpio_set_pin_output(LED_PIN);
}

/* Read column inputs from Pico GPIO */
static matrix_row_t read_cols(bool invert) {
    matrix_row_t cols = 0;
    for (uint8_t i = 0; i < NUM_COLS; i++) {
        if (gpio_read_pin(column_pins[i])) {
            cols |= (1 << i);
        }
    }
    if (!invert) {
        cols = ~cols;
    }
    return cols;
}

/* LED control helpers */
void pibookintosh_set_led_power(bool on) {
    mcp23017_set_led_power(on);
}

void pibookintosh_set_led_caps(bool on) {
    mcp23017_set_led_caps(on);
}

/* ===== Matrix Implementation ===== */

/* Matrix initialization */
void pibookintosh_matrix_init(void) {
    // Initialize column pins as inputs
    for (uint8_t i = 0; i < NUM_COLS; i++) {
        gpio_set_pin_input_low(column_pins[i]);
    }
    
    // Initialize meta-key pins as inputs
    for (uint8_t i = 0; i < NUM_META_KEYS; i++) {
        gpio_set_pin_input_low(meta_pins[i]);
    }
    
    // Clear matrix
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        pibookintosh_matrix[i] = 0;
    }
}

/* Matrix scan */
uint8_t pibookintosh_matrix_scan(void) {
    bool changed = false;
    
    // Scan each row
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        // Activate row via MCP23017
        if (row < 8) {
            // Rows 0-7 use GPIOA
            mcp23017_write_data_a(1 << row);
            mcp23017_write_data_b(0x00);
        } else {
            // Rows 8-9 use GPIOB (bits 0-1)
            mcp23017_write_data_a(0x00);
            mcp23017_write_data_b(1 << (row - 8));
        }
        
        // Small delay for settling
        wait_us(30);
        
        // Read columns
        matrix_row_t cols = read_cols(ACTIVE_HIGH);
        
        // Check if state changed
        if (pibookintosh_matrix[row] != cols) {
            pibookintosh_matrix[row] = cols;
            changed = true;
        }
    }
    
    // Deactivate all rows
    mcp23017_write_data_a(0x00);
    mcp23017_write_data_b(0x00);
    
    // Blink LED periodically
    uint32_t elapsed = timer_read32();
    if ((elapsed / 500) % 2 == 0) {
        gpio_write_pin_high(LED_PIN);
    } else {
        gpio_write_pin_low(LED_PIN);
    }
    
    return changed;
}

/* Get matrix row */
uint8_t pibookintosh_matrix_get_row(uint8_t row) {
    if (row < MATRIX_ROWS) {
        return pibookintosh_matrix[row];
    }
    return 0;
}
