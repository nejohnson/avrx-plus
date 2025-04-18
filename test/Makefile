##############################################################################
##
## Makefile for avrx-plus test suite
##
##############################################################################

CC = avr-gcc

LIBS = ../avrx-plus.a

CFLAGS = -g -Wall -mmcu=atmega8 -fomit-frame-pointer -I../inc

SIMPATH = ../../simulavr-mingw/app
SIMULAVR = $(SIMPATH)/simulavr
SIMULAVROPTS = -d atmega8 -W 0x20,- -R 0x22,- -T exit -F 8000000
TRACEOPTS = -t trace.txt

TESTS = axp_test_1 axp_test_2 axp_test_3 axp_test_4 axp_test_5

TESTEXE = $(addsuffix .elf, $(TESTS))

##############################################################################

all: $(TESTEXE)

%.elf : %.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

##############################################################################
## Run targets
##############################################################################

run1: axp_test_1.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<

run2: axp_test_2.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<

run3: axp_test_3.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<

run4: axp_test_4.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<

run5: axp_test_5.elf
	@echo "Running simulation..."
	$(SIMULAVR) $(SIMULAVROPTS) -f $<

##############################################################################
## Cleaning up the mess
##############################################################################

clean:
	rm -f *.elf
	rm -f trace.txt
