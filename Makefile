##############################################################################
#
# Makefile for AvrX-Plus (avrx-plus)
#
##############################################################################
#
# Copyright (C) 2025 Neil Johnson
#
# See LICENSE for license terms.
#
##############################################################################
#
# Targets:
#
# make (all)     Build AvrX-Plus library
# make install   Install AvrX-Plus library in INSTALLDIR
# make clean     Clean up the mess
# make what      Show help
#
##############################################################################

CC = avr-gcc
AR = avr-ar -rcsv

MCU = atmega8

TARGET = avrx-plus

SRCDIR     = src
INCDIR     = inc
BUILDDIR   = build
INSTALLDIR = ../lib

VPATH    = $(SRCDIR)

##############################################################################

CSRC_CORE = \
	axp_queuepid.c \
	axp_removeobj.c \
	axp_removenextobj.c \
	axp_appendobj.c

CSRC = \
	$(CSRC_CORE) \
	axp_kernel.c \
	axp_resume.c \
	axp_initthread.c \
	axp_startthread.c \
	axp_yield.c \
	axp_priority.c \
	axp_self.c \
	axp_halt.c \
	axp_exitthread.c \
	axp_terminate.c \
	axp_suspend.c \
	axp_setsem.c \
	axp_waitsem.c \
	axp_resetsem.c \
	axp_testsem.c

ASRC = \
	axp_enterkernel.S \
	axp_leavekernel.S \
	axp_setkernelstack.S

OBJS = $(addprefix $(BUILDDIR)/,$(CSRC:.c=.o) $(ASRC:.S=.o))

##############################################################################

OPT = s

CFLAGS  = -mmcu=$(MCU)
CFLAGS += -I./$(INCDIR)
CFLAGS += -O$(OPT) -g
CFLAGS += -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -fomit-frame-pointer
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -std=gnu99

AFLAGS = -mmcu=$(MCU) -I./$(INCDIR) -x assembler-with-cpp -g

##############################################################################

all:  $(TARGET).a

clean:
	rm -f $(TARGET).a
	rm -rf $(BUILDDIR)

install: $(TARGET).a | $(INSTALLDIR)
	cp $(TARGET).a $(INSTALLDIR)
	cp $(INCDIR)/avrxplus.h $(INSTALLDIR)
	
##############################################################################

%.a: $(OBJS) Makefile
	$(AR) $@ $(OBJS)
	
$(BUILDDIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o : %.S
	$(CC) -c $(AFLAGS) $< -o $@

$(OBJS): | $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(INSTALLDIR):
	mkdir -p $(INSTALLDIR)

##############################################################################

what:
	@echo "Make avrx-plus. Targets:"
	@echo "--------------------------"
	@echo "   (all)     Build AvrX-Plus library (default)"
	@echo "   install   Install AvrX-Plus library in INSTALLDIR"
	@echo "   clean     Clean up the mess"
	@echo "   what      Show this help"

##############################################################################

.PHONY : all clean install Makefile what

##############################################################################
##############################################################################
##############################################################################
