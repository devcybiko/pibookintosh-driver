# MCU name
MCU = RP2040

# Bootloader selection
BOOTLOADER = rp2040

# Enable custom matrix scanning
CUSTOM_MATRIX = yes

# Enable I2C driver
I2C_DRIVER_REQUIRED = yes

# Enable UART for touchpad
UART_DRIVER_REQUIRED = yes

# Enable mouse support
MOUSE_ENABLE = yes

# Project specific files
SRC += mcp23017.c qmk_callbacks.c pitouch.c

EXTRAKEY_ENABLE = yes
RAW_ENABLE = yes
CONSOLE_ENABLE = no

# Allow unused variables/functions as warnings (not errors)
CFLAGS += -Wno-error=unused-variable -Wno-error=unused-function