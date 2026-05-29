// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "i2c_master.h"
#include "mcp23017.h"
#include "pibookintosh.h"

/* Pin definitions */
#define LED_PIN           GP25
#define COL_PINS          {GP6, GP7, GP8, GP9, GP10, GP11, GP12, GP13}
#define META_PINS         {GP16, GP17, GP18, GP19, GP20, GP21, GP22}
#define NUM_COLS          8
#define NUM_META_KEYS     7

static const pin_t column_pins[NUM_COLS] = COL_PINS;
static const pin_t meta_pins[NUM_META_KEYS] = META_PINS;
static matrix_row_t pibookintosh_matrix[MATRIX_ROWS] __attribute__((used));
static matrix_row_t meta_row = 0;

#define ACTIVE_HIGH true
#define ACTIVE_LOW false

/* LED pin definitions on MCP23017 GPIOB */
#define MCP_LED_POWER     0x04  // GPIOB pin 2
#define MCP_LED_CAPS      0x08  // GPIOB pin 3

static uint8_t caps_lock_status = 0;
static uint8_t power_led_status = 0;
static bool power_led_hid_controlled = false;  // Track if LED is controlled via HID

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
static matrix_row_t _read_cols(bool invert) {
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
void pibookintosh_set_power_led(bool on) {
    power_led_status = on ? MCP_LED_POWER : 0;
}

/* Set caps lock LED state */
void pibookintosh_set_caps_led(bool on) {
    caps_lock_status = on ? MCP_LED_CAPS : 0;
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
        gpio_set_pin_input_high(meta_pins[i]);
    }
    
    // Clear matrix
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        pibookintosh_matrix[i] = 0;
    }
}

static void _write_row(uint16_t row) {
    if (row < 8) {
        // Rows 0-7 use GPIOA
        mcp23017_write_data_a(1 << row);
        mcp23017_write_data_b(0x00 | caps_lock_status | power_led_status);
    } else {
        // Rows 8-9 use GPIOB (bits 0-1)
        mcp23017_write_data_a(0x00);
        mcp23017_write_data_b((1 << (row - 8)) | caps_lock_status | power_led_status);
    }
}
/* Matrix scan */
uint8_t pibookintosh_matrix_scan(void) {
    bool changed = false;
    
    // Scan each row
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        // Activate row via MCP23017
        _write_row(row);
        
        // Small delay for settling
        wait_us(30);
        
        // Read columns
        matrix_row_t cols = _read_cols(ACTIVE_HIGH);
        
        // Check if state changed
        if (pibookintosh_matrix[row] != cols) {
            pibookintosh_matrix[row] = cols;
            changed = true;
        }
    }

    // Scan meta keys into virtual row
    matrix_row_t new_meta = 0;
    for (uint8_t i = 0; i < NUM_META_KEYS; i++) {
        if (!gpio_read_pin(meta_pins[i])) {
            new_meta |= (1 << i);
        }
    }
    
    if (meta_row != new_meta) {
        meta_row = new_meta;
        changed = true;
    }

    // Blink LED periodically (unless controlled via HID)
    if (!power_led_hid_controlled) {
        uint32_t elapsed = timer_read32();
        if ((elapsed / 500) % 2 == 0) {
            gpio_write_pin_high(LED_PIN);
            pibookintosh_set_power_led(true);
        } else {
            gpio_write_pin_low(LED_PIN);
            pibookintosh_set_power_led(false);
        }
    }
    // Deactivate all rows
    mcp23017_write_data_a(0x00);
    mcp23017_write_data_b(0x00 | caps_lock_status | power_led_status);
    
    return changed;
}

/* Get matrix row */
uint8_t pibookintosh_matrix_get_row(uint8_t row) {
    if (row < MATRIX_ROWS - 1) {
        return pibookintosh_matrix[row];
    } else if (row == MATRIX_ROWS - 1) {
        return meta_row;
    }
    return 0;
}

/* Raw HID handler for LED control */
void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length < 2) return;
    
    uint8_t command = data[0];
    uint8_t value = data[1];
    
    if (command == 0x01) {  // LED control command
        power_led_hid_controlled = true;
        if (value == 0x00) {  // LED off
            gpio_write_pin_low(LED_PIN);
            pibookintosh_set_power_led(false);
        } else if (value == 0x01) {  // LED on
            gpio_write_pin_high(LED_PIN);
            pibookintosh_set_power_led(true);
        }
    } else if (command == 0x02) {  // Release HID control (return to firmware blinking)
        power_led_hid_controlled = false;
    }
    
    // Send acknowledgment back
    uint8_t response[32] = {0};
    response[0] = command;
    response[1] = 0xAA;  // ACK byte
    raw_hid_send(response, 32);
}
