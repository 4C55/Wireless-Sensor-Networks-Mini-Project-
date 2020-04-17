CONTIKI_PROJECT = Producer
all: $(CONTIKI_PROJECT)


PROJECTDIRS += ../Shared/File
PROJECTDIRS += ../Shared/Types
PROJECTDIRS += Connection
PROJECT_SOURCEFILES += file.c
PROJECT_SOURCEFILES += connection.c
CONTIKI = ../..

MAKE_MAC ?= MAKE_MAC_CSMA
MAKE_NET = MAKE_NET_NULLNET
include $(CONTIKI)/Makefile.include
