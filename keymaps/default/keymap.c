// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_F2,    KC_GRV,    KC_TAB,    KC_X,    KC_S,    KC_W,    KC_2,
        KC_NO,   KC_F3,    KC_NO,    KC_NO,    KC_C,    KC_D,    KC_E,    KC_3,
        KC_NO,   KC_F4,    KC_CAPS,    KC_NO,    KC_V,    KC_F,    KC_R,    KC_4,
        KC_MINS, KC_F5,    KC_LBRC,    KC_QUOT,    KC_B,    KC_G,    KC_T,    KC_5,
        KC_NO,   KC_F6,    KC_SPC,     KC_SPC,  KC_N,    KC_H,    KC_Y, KC_6,
        KC_NO,   KC_F7,    KC_NO,    KC_NO,   KC_M,    KC_J,    KC_U,   KC_7,
        KC_ENTER, KC_F10, KC_EQL,   KC_RBRC,   KC_SLSH, KC_SCLN, KC_P,   KC_0,
        KC_F11,   KC_F1,   KC_F12,   KC_DEL,   KC_Z,    KC_A,    KC_Q,   KC_1,
        KC_ENTER,   KC_F8,   KC_RIGHT,   KC_LEFT,   KC_COMM, KC_K,    KC_I,   KC_8,//?
        KC_BSLS,   KC_F9,   KC_UP,   KC_DOWN,    KC_DOT,  KC_L,    KC_O,   KC_9//?
    )
};
                                                            