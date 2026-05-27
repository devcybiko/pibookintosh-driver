// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>

/* MCP23017 I2C Registers */
#define MCP23017_IODIRA   0x00
#define MCP23017_IODIRB   0x01
#define MCP23017_GPPUA    0x0C
#define MCP23017_GPPUB    0x0D
#define MCP23017_GPIOA    0x12
#define MCP23017_GPIOB    0x13
#define MCP23017_OLATA    0x14
#define MCP23017_OLATB    0x15

/* MCP23017 I2C address (8-bit shifted) */
#define MCP23017_ADDR     (0x20 << 1)


/**
 * Initialize MCP23017 expander
 * Sets all pins as outputs and disables pull-ups
 */
void mcp23017_init(bool invert);

/**
 * Write to MCP23017 register
 * @param reg Register address
 * @param value Value to write
 */
void mcp23017_write(uint8_t reg, uint8_t value);
void mcp23017_write_data_a(uint8_t value);
void mcp23017_write_data_b(uint8_t value);

/**
 * Read from MCP23017 register
 * @param reg Register address
 * @return Value read from register
 */
uint8_t mcp23017_read(uint8_t reg);

/**
 * Set power LED state
 * @param on true to turn on, false to turn off
 */
void mcp23017_set_led_power(bool on);

/**
 * Set caps lock LED state
 * @param on true to turn on, false to turn off
 */
void mcp23017_set_led_caps(bool on);
