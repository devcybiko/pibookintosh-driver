#include "quantum.h"
#include "pitouch.h"
#include "uart.h"

// initialize the PiTouch object
void pitouch_init(PiTouch *touch) {
    touch->dx_scale = -10;
    touch->dy_scale = -10;
    touch->dx = 0;  // Relative X movement
    touch->dy = 0;  // Relative Y movement
    touch->buttons = 0;  // Button state
    for (int i = 0; i < 5; i++) {
        touch->packet[i] = 0;
    }
    touch->packet_index = 0;
    touch->baud = 9600; // Default baud rate
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
        mouse_report.y = touch->dy_scale * touch->dy;  // Invert Y if needed (depends on touchpad orientation)
        mouse_report.buttons = touch->buttons;
        host_mouse_send(&mouse_report);
    }
}

void pitouch_task(PiTouch *touch) {
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

            touch->buttons = touch->packet[0] & 0x07;         // Button state            
            touch->packet_index = 0;
            pitouch_callback(touch);
            // Reset movement after sending
            touch->dx = 0;
            touch->dy = 0;
        }
    }
}

