#include "quantum.h"
#include "pitouch.h"
#include "uart.h"
#include <stdarg.h>

/* Debug: Send string as keyboard events */
static void debug_send_string(const char *str, ...) {
    // Format the string with variable arguments
    char buffer[128];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, sizeof(buffer), str, args);
    va_end(args);
    SEND_STRING_DELAY(buffer, 1);  // 10ms delay between keys
}

// initialize the PiTouch object
void pitouch_init(PiTouch *touch, void *user_data) {
    touch->dx_scale = 10;
    touch->dy_scale = 10;
    touch->dx = 0;  // Relative X movement Invert X if needed (depends on touchpad orientation)
    touch->dy = 0;  // Relative Y movement Invert Y if needed (depends on touchpad orientation)
    touch->buttons = 0;  // Button state
    for (int i = 0; i < 5; i++) {
        touch->packet[i] = 0;
    }
    touch->packet_index = 0;
    touch->baud = 9600; // Default baud rate
    touch->user_data = user_data; // Store user data pointer
}

// Start the PiTouch task (initialize UART)
void pitouch_start(PiTouch *touch) {
    // Initialize UART for touchpad communication
    uart_init(touch->baud);
}

// Process mouse movement and button state, send HID report
void pitouch_callback(PiTouch *touch) {
    if (touch->dx != 0 || touch->dy != 0 || touch->buttons != 0) {
        report_mouse_t mouse_report = {0};
        mouse_report.x = touch->dx_scale * touch->dx;
        mouse_report.y = touch->dy_scale * touch->dy;  
        mouse_report.buttons = touch->buttons & 0x01; // Map to left button for simplicity
        host_mouse_send(&mouse_report);
    }
}

void pitouch_task(PiTouch *touch, void (*callback)(PiTouch *)) {
    // Only read one byte per scan to avoid blocking
    if (!uart_available()) {
        return;
    }
        uint8_t byte = uart_read();
    if (byte & 0x80) {
        touch->packet[0] = byte;
        touch->packet_index = 1;
    } else if (touch->packet_index > 0 && touch->packet_index < sizeof(touch->packet)) {
        touch->packet[touch->packet_index] = byte;
        touch->packet_index++;
        
        // Process complete packet
        if (touch->packet_index == sizeof(touch->packet)) {
            // touch->packet[1] = msb for dy but we don't need it
            // touch->packet[2] = msb for dx but we don't need it
            touch->dy = (int8_t)touch->packet[3];      // Y movement (signed, negate if needed)
            touch->dx = (int8_t)touch->packet[4];      // X movement (signed)
            
            // Sign-extend from 7-bit to 8-bit signed
            if (touch->dx & 0x40) touch->dx |= 0x80;
            if (touch->dy & 0x40) touch->dy |= 0x80;

            // touch->buttons = touch->packet[0] & 0x07;         // Button state 
            touch->buttons = touch->packet[0];         // Button state (only left button)           
            touch->packet_index = 0;
            // debug_send_string("%d %d %d %d %d\n", touch->packet[0], touch->packet[1], touch->packet[2], touch->packet[3], touch->packet[4]);
            if (callback) callback(touch);
            // Reset movement after sending
            touch->dx = 0;
            touch->dy = 0;
        }
    }
}

