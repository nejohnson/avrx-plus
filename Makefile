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

CSRC_CORE = 	axpc_queuepid.c			\
		axpc_appendobj.c		\
		axpc_removeobj.c		\
		axpc_removenextobj.c

CSRC =  $(CSRC_CORE)				\
	axp_delay.c				\
	axp_halt.c				\
	axp_intsendmessage.c			\
	axp_intsetsemaphore.c			\
	axp_kernel.c				\
	axp_priority.c 				\
	axp_recvmessage.c			\
	axp_resetsemaphore.c			\
	axp_resume.c				\
	axp_runtask.c				\
	axp_sendmessage.c			\
	axp_setsemaphore.c			\
	axp_suspend.c				\
	axp_systemobj.c				\
	axp_taskexit.c 				\
	axp_initthread.c 			\
	axp_terminate.c				\
	axp_testsemaphore.c			\
	axp_waitmessage.c			\
	axp_waitsemaphore.c			\
	axp_yield.c
	

ASRC  = axp_canceltimer.S 			\
	axp_canceltimermessage.S 		\
	axp_message.S 				\
	axp_recvmessage.S 			\
	axp_reschedule.S 			\
	axp_semaphores.S 			\
	axp_starttimermessage.S 		\
	axp_suspend.S 				\
	axp_enterkernel.S 			\
	axp_leavekernel.S			\
	axp_timequeue.S 	

OBJS = $(addprefix $(BUILDDIR)/,$(CSRC:.c=.o) $(ASRC:.S=.o))

##############################################################################

OPT = s

CFLAGS  = -mmcu=$(MCU)
CFLAGS += -I./$(INCDIR)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -std=gnu99

AFLAGS = -mmcu=$(MCU) -I./$(INCDIR) -x assembler-with-cpp

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
