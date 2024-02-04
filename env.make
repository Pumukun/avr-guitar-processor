# Arduino UNO environmental variables
MCU = atmega328p
SERIAL = /dev/ttyACM0
F_CPU = 16000000UL
BAUD  = 250000UL
SOFT_RESET = 0
LIBDIR = lib/arduino
PROGRAMMER_TYPE = Arduino
PROGRAMMER_ARGS = -F -V -P $(SERIAL) -b 115200
TOOLCHAIN = arduino
OS = lin
