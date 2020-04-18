CONTIKI_PROJECT = ImageUpload
all: $(CONTIKI_PROJECT)


PROJECTDIRS += ../Shared/File
PROJECTDIRS += ../Shared/Types
PROJECTDIRS += ../Shared/Message
PROJECTDIRS += ../Shared/CircullarBuffer

PROJECT_SOURCEFILES += file.c
PROJECT_SOURCEFILES += message.c
PROJECT_SOURCEFILES += circullar_buffer.c
CONTIKI = ../..

MAKE_MAC ?= MAKE_MAC_CSMA
MAKE_NET = MAKE_NET_NULLNET

include $(CONTIKI)/Makefile.include