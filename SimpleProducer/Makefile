CONTIKI_PROJECT = SimpleProducer
all: $(CONTIKI_PROJECT)


PROJECTDIRS += ../Shared/File
PROJECTDIRS += ../Shared/Types
PROJECTDIRS += ../Shared/SinkLink
PROJECTDIRS += ../Shared/Compression

PROJECT_SOURCEFILES += file.c
PROJECT_SOURCEFILES += sink_link.c
PROJECT_SOURCEFILES += compression.c
CONTIKI = ../..

MAKE_MAC ?= MAKE_MAC_CSMA
MAKE_NET = MAKE_NET_NULLNET

include $(CONTIKI)/Makefile.include
