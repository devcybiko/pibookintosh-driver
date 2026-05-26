// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "i2c_master.h"
#include "mcp23017.h"

static bool inverted = false;

/* Write to MCP23017 register */
void mcp23017_write_data_a(uint8_t value) {
    if (inverted) {
        value = ~value;
    }
    i2c_write_register(MCP23017_ADDR, MCP23017_OLATA, &value, 1, 100);
}

void mcp23017_write_data_b(uint8_t value) {
    if (inverted) {
        value = ~value;
    }
    i2c_write_register(MCP23017_ADDR, MCP23017_OLATB, &value, 1, 100);
}

/* Write to MCP23017 register */
void mcp23017_write(uint8_t reg, uint8_t value) {
    i2c_write_register(MCP23017_ADDR, reg, &value, 1, 100);
}

/* Read from MCP23017 register */
uint8_t mcp23017_read(uint8_t reg) {
    uint8_t value = 0;
    i2c_read_register(MCP23017_ADDR, reg, &value, 1, 100);
    return value;
}

/* Initialize MCP23017 */
void mcp23017_init(bool invert) {
    inverted = invert;                      
    // Set GPIOA as all outputs (rows 0-7)
    mcp23017_write(MCP23017_IODIRA, 0x00);
    
    // Set GPIOB as all outputs (rows 8-9, LEDs)
    mcp23017_write(MCP23017_IODIRB, 0x00);
    
    // Disable pull-ups on all pins
    mcp23017_write(MCP23017_GPPUA, 0x00);
    mcp23017_write(MCP23017_GPPUB, 0x00);
    
    // Initialize all outputs to low
    mcp23017_write_data_a(0x00);
    mcp23017_write_data_b(0x00);
}

/* Set power LED state */
void mcp23017_set_led_power(bool on) {
    uint8_t val = mcp23017_read(MCP23017_OLATB);
    if (on) {
        val |= MCP_LED_POWER;
    } else {
        val &= ~MCP_LED_POWER;
    }
    mcp23017_write_data_b(val);
}

/* Set caps lock LED state */
void mcp23017_set_led_caps(bool on) {
    uint8_t val = mcp23017_read(MCP23017_OLATB);
    if (on) {
        val |= MCP_LED_CAPS;
    } else {
        val &= ~MCP_LED_CAPS;
    }
    mcp23017_write_data_b(val);
}
