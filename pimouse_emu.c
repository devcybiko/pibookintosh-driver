#include "quantum.h"
#include "pimouse_emu.h"
#include "pitouch.h"

static void _led(int on_off) {
    if (on_off) {
        gpio_write_pin_high(GP25);
    } else {
        gpio_write_pin_low(GP25);
    }
}

static void _mouse(int8_t dx, int8_t dy, uint8_t buttons) {
    report_mouse_t mouse_report = {0};
    mouse_report.x = dx;
    mouse_report.y = dy;  
    mouse_report.buttons = buttons;
    host_mouse_send(&mouse_report);
}

void pimouse_init(PiMouseEmu *mouse) {
    mouse->clicked_ms = 0;
    mouse->down_ms = 0;
    mouse->wheel_ms = 0;
    mouse->drag_ms = 0;
    mouse->move_ms = 0;
    mouse->mult_dx = 1;
    mouse->mult_dy = 1;
    mouse->WHEEL_DY = -1;
    mouse->CLICK_MS = 200;
    mouse->LONG_PRESS_MS = 300;
    mouse->CLICK_PRESS_MS = 300;
    mouse->ACCELERATION = 0;
}

void pimouse_callback(PiTouch *touch) {
    PiMouseEmu *mouse = (PiMouseEmu *)touch->user_data;
    uint32_t now = timer_read32();
    _led(touch->buttons);
    // if (mouse->down_ms && !mouse->drag_ms) {
    //     if ((now - mouse->down_ms) > mouse->LONG_PRESS_MS) {
    //         mouse->wheel_ms = now;
    //         mouse->down_ms = 0;
    //     }
    // }
    if (touch->buttons) {
        // mouse press or drag
        if (touch->dx == 0 && touch->dy == 0) {
            // // mouse press / click
            // if (mouse->clicked_ms && (now - mouse->clicked_ms) < mouse->CLICK_PRESS_MS) {
            //     _mouse(0, 0, 0x01); // mouse.press(Mouse.LEFT_BUTTON)
            //     // reset click timer to prevent multiple clicks
            //     mouse->clicked_ms = 0;
            //     mouse->wheel_ms = 0;
            //     mouse->down_ms = 0;
            //     mouse->drag_ms = now;
            // } else {
            //     // mouse down
            //     mouse->mult_dx = 1;
            //     mouse->mult_dy = 1;
            //     mouse->down_ms = now;
            // }
        } else {
            // mouse move
            if (0 * mouse->wheel_ms) {
                _mouse(mouse->mult_dx, mouse->WHEEL_DY * mouse->mult_dy, 0x04); // mouse.wheel()
            } else {
                // mouse move
                _mouse(touch->dx_scale * touch->dx, touch->dy_scale * touch->dy, 0x00); // mouse.move()
                if (mouse->move_ms) {
                    uint16_t delta = now - mouse->move_ms;
                    uint16_t mult = 1;
                    if (mouse->ACCELERATION) {
                        if (delta < 120) mult = 2;
                        if (delta < 100) mult = 3;
                        if (delta < 80) mult = 4;
                        if (delta < 60) mult = 6;
                        if (delta < 40) mult = 8;
                        if (delta < 20) mult = 10;
                    }
                    mouse->mult_dx = mult;
                    mouse->mult_dy = mult;
                }
                mouse->move_ms = now;
            }
            mouse->down_ms = 0;
        }
    } else {
        // // mouse release
        // if (mouse->down_ms && (now - mouse->down_ms) < mouse->CLICK_MS) {
        //     _mouse(0, 0, 0x01); // mouse.click(Mouse.LEFT_BUTTON)
        //     mouse->clicked_ms = now;
        // } else {
        //     _mouse(0, 0, 0x00); // mouse.release(Mouse.LEFT_BUTTON)
        // }
        // mouse->down_ms = 0;
        // mouse->wheel_ms = 0;
        // mouse->drag_ms = 0;
        // mouse->move_ms = 0;
    }
}