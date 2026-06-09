#include <stdint.h>

typedef struct pitouch {
    int8_t dx_scale; // Sensitivity level (0-255)
    int8_t dy_scale;
    int8_t dx;  // Relative X movement
    int8_t dy;  // Relative Y movement
    uint8_t buttons;  // Button state
    uint8_t packet[5];
    uint8_t packet_index;
    int16_t baud; // Default baud rate
} PiTouch;

extern void pitouch_init(PiTouch *touch);
extern void pitouch_start(PiTouch *touch);
extern void pitouch_callback(PiTouch *touch);
extern void pitouch_task(PiTouch *touch);
