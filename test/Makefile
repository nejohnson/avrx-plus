##############################################################################
##
## Makefile for avrx-plus test suite
##
##############################################################################

CC = avr-gcc

LIBS = ../avrx-plus.a

CFLAGS = -g -mmcu=atmega8 -I../inc

SIMULAVR = simulavr.exe
SIMULAVROPTS = -d atmega8 -W 0x20,- -R 0x22,- -T exit -F 8000000
TRACEOPTS = -t trace.txt

TESTS = Test1 Test2 Test3 Test4

TESTEXE = $(addsuffix .elf, $(TESTS))

##############################################################################

all: $(TESTEXE)

%.elf : %.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

##############################################################################
## Run targets
##############################################################################

run1: Test1.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<
	
run2: Test2.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<
	
run3: Test3.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<

run4: Test4.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<
	
##############################################################################
## Cleaning up the mess
##############################################################################

clean:
	rm -f Test*.elf
	rm -f trace.txt
