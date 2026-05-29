// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Debounce reduces chatter (unintended double-presses) - set 0 if not needed */
#define DEBOUNCE 5

/* Custom matrix with MCP23017 expander */
#define CUSTOM_MATRIX_ENABLE
#define MATRIX_ROWS 11 // 10+1 for meta key row
#define MATRIX_COLS 8

/* I2C settings for MCP23017 */
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP26
#define I2C1_SCL_PIN GP27

#define FORCE_NKRO