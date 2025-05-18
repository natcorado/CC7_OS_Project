arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding process2.c -o process2.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding os.c -o os.o

arm-none-eabi-ld -T memmap startup.o os.o process2.o -o process2.elf
arm-none-eabi-objcopy process2.elf -O binary process2.bin