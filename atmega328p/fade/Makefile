TARGET         = fade
PROGRAMMER     = arduino
PORT           = /dev/ttyACM0
MCU_TARGET     = atmega328p
AVRDUDE_TARGET = atmega328p
OPTIMIZE       = -Os
DEFS           =
LIBS           =

HZ          = 16000000L

CC=avr-gcc
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump

SRC=$(TARGET).c
OBJ=$(TARGET).o
ELF=$(TARGET).elf
HEX=$(TARGET).hex
EHEX=$(TARGET)_eeprom.hex
BIN=$(TARGET).bin
SREC=$(TARGET).srec
EBIN=$(TARGET)_eeprom.bin
ESREC=$(TARGET)_eeprom.srec
LST=$(TARGET).lst
MAP=$(TARGET).map

override CFLAGS=-DF_CPU=$(HZ) -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS=-Wl,-Map,$(TARGET).map

all: $(ELF) $(LST) text #eeprom

$(ELF): $(OBJ)
	$(CC) $(LDLAGS) -o $@ $^ $(LIBS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(OBJ) $(ELF) $(HEX) $(EHEX) $(BIN) $(EBIN) $(SREC) $(ESREC)
	$(RM) $(MAP) $(LST)

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: $(HEX) $(BIN) $(SREC)

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: $(EHEX) $(EBIN) $(ESREC)

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

#%_eeprom.srec: %.elf
#	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@

# command to program chip (invoked by running "make install")
install: $(HEX)
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v  \
         -U flash:w:$^

fuse:
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v \
	-U lfuse:w:0xc6:m -U hfuse:w:0xd9:m

ddd: gdbinit
	ddd --debugger "avr-gdb -x $(GDBINITFILE)"

gdbserver: gdbinit
	simulavr --device $(MCU_TARGET) --gdbserver

gdbinit: $(GDBINITFILE)

$(GDBINITFILE): $(PRG).hex
	@echo "file $(PRG).elf" > $(GDBINITFILE)

	@echo "target remote localhost:1212" >> $(GDBINITFILE)
	@echo "load"                         >> $(GDBINITFILE)
	@echo "break main"                   >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"
