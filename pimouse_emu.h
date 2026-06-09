#pragma once

#include <stdint.h>
#include "pitouch.h"

typedef struct pimouse_emu {
    uint8_t clicked_ms;
    uint8_t down_ms;
    uint8_t wheel_ms;
    uint8_t drag_ms;
    uint8_t move_ms;
    int8_t mult_dx;
    int8_t mult_dy;
    int16_t WHEEL_DY;
    int16_t CLICK_MS;
    int16_t LONG_PRESS_MS;
    int16_t CLICK_PRESS_MS;
    uint16_t ACCELERATION;

} PiMouseEmu;

void pimouse_init(PiMouseEmu *mouse);
void pimouse_callback(PiTouch *touch);
