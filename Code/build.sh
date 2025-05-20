#!/bin/bash

# Assemble startup.s
arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o
arm-none-eabi-as --warn --fatal-warnings context_switch.s -o context_switch.o

# Compile interrupts.c
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding os.c -o os.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding uart.c -o uart.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding process1.c -o process1.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding process2.c -o process2.o

# Link with memmap.ld
arm-none-eabi-ld -T memmap.ld startup.o context_switch.o uart.o os.o -o os.elf
arm-none-eabi-ld -T memmap_p1.ld startup.o context_switch.o uart.o process1.o -o process1.elf
arm-none-eabi-ld -T memmap_p2.ld startup.o context_switch.o uart.o process2.o -o process2.elf

# Generate binary
arm-none-eabi-objcopy -O binary os.elf os.bin
arm-none-eabi-objcopy process1.elf -O binary process1.bin
arm-none-eabi-objcopy process2.elf -O binary process2.bin

# Remove -o files
rm -f *.o
# Remove -elf files
rm -f *.elf

echo "Build complete!"