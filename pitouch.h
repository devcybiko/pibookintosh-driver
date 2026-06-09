#include <stdint.h>

typedef struct pitouch {
    int8_t dx;  // Relative X movement
    int8_t dy;  // Relative Y movement
    int8_t dx_scale; // dx multiplier for sensitivity adjustment
    int8_t dy_scale; // dy multiplier for sensitivity adjustment
    uint8_t buttons;  // Button state
    int16_t baud; // Default baud rate
    uint8_t packet_index;
    uint8_t packet[5];
} PiTouch;

extern void pitouch_init(PiTouch *touch);
extern void pitouch_start(PiTouch *touch);
extern void pitouch_callback(PiTouch *touch);
extern void pitouch_task(PiTouch *touch);
