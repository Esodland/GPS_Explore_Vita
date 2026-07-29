TARGET = gps_spoofer
OBJS = main.o

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -O3
ASFLAGS = $(CFLAGS)

all: $(TARGET).suprx

$(TARGET).suprx: $(TARGET).velf
	vita-make-fself -c $< $@

$(TARGET).velf: $(TARGET).elf
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -nostartfiles -lm -lc -lSceSysmodule_stub -ltaihen_stub

clean:
	@rm -rf $(TARGET).suprx $(TARGET).velf $(TARGET).elf $(OBJS)
