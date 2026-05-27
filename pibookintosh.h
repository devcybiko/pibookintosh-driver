// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>

/* Keyboard pre-initialization */
void pibookintosh_pre_init_kb(void);

/* Matrix helpers needed by callbacks */
void pibookintosh_matrix_init(void);
uint8_t pibookintosh_matrix_scan(void);
uint8_t pibookintosh_matrix_get_row(uint8_t row);

/* LED helpers */
void pibookintosh_set_power_led(bool on);
void pibookintosh_set_caps_led(bool on);
