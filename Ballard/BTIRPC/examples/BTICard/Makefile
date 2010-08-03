
##
#
#  BTICard Host RPC Linux Examples
#  Copyright (c) 2004-2009
#  Ballard Technology, Inc.
#  www.ballardtech.com
#  support@ballardtech.com
#  ALL RIGHTS RESERVED
#
##

ifeq (config.mk,$(wildcard config.mk))

CC = gcc

# load BOARD configuration
include config.mk

ifndef APIPATH
APIPATH = ../..
endif

APIDIR = $(APIPATH)/API

CFLAGS += -Wall

LIBS = -lbti1553-$(BOARD) -lbti429-$(BOARD) -lbti422-$(BOARD) -lbti708-$(BOARD) -lbti717-$(BOARD) -lbtiCSDB-$(BOARD) -lbtiDIO-$(BOARD) -lncurses -lpthread

ifdef DEBUG
CFLAGS += -D_DEBUG -O0 -ggdb
OUTDIR  = debug-$(BOARD)
else
CFLAGS += -DNDEBUG -O2
OUTDIR  = release-$(BOARD)
endif

ifndef BUILDSO
all clean install:
	make $@ DEBUG=1   BUILDSO=1
	make $@ RELEASE=1 BUILDSO=1
else
VPATH = src

BINS := $(patsubst %.c,$(OUTDIR)/%,$(wildcard *.c))

all : $(BINS)

$(OUTDIR)/%: %.c
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -I$(APIDIR) -L$(APIDIR) $(LIBS) -o $@ $^

clean :
	rm -rf $(OUTDIR)

endif

else
all install $(BINS) :
	@echo "System not configured!" >&2
	@echo "" >&2
	@echo "Please run 'make <BOARD>_config', where <BOARD> is one of the following:" >&2
	@echo "" >&2
	@echo "rpc-ppc   Embedded PPC RPC mode" >&2
	@echo "rpc-x86   Host Linux x86 RPC mode" >&2
	@echo "rpc-x64   Host Linux x64 RPC mode" >&2
	@echo "" >&2
	@exit 1
endif

#########################################################################
# Configuration stuff
#########################################################################

unconfig:
	rm -f src config.mk

rpc-ppc_config:	unconfig
	@echo "Configuring for PPC host via RPC..."
	@echo "BOARD = rpc-ppc" > config.mk

rpc-x86_config:	unconfig
	@echo "Configuring for x86 host via RPC..."
	@echo "BOARD = rpc-x86" > config.mk
	@echo "CFLAGS = -m32" >> config.mk

rpc-x64_config:	unconfig
	@echo "Configuring for x64 host via RPC..."
	@echo "BOARD = rpc-x64" > config.mk
	@echo "CFLAGS = -m64" >> config.mk

#########################################################################
# Clean stuff
#########################################################################

mrproper:	clean unconfig
