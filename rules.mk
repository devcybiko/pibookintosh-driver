# MCU name
MCU = RP2040

# Bootloader selection
BOOTLOADER = rp2040

# Enable custom matrix scanning
CUSTOM_MATRIX = yes

# Enable I2C driver
I2C_DRIVER_REQUIRED = yes

# Project specific files
SRC += mcp23017.c qmk_callbacks.c

EXTRAKEY_ENABLE = yes
RAW_ENABLE = yes