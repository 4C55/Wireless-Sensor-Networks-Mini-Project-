CONTIKI_PROJECT = Producer
all: $(CONTIKI_PROJECT)


PROJECTDIRS += ../Shared/File
PROJECTDIRS += ../Shared/Types
PROJECTDIRS += ../Shared/CircullarBuffer
PROJECTDIRS += ../Shared/MessageHandler
PROJECTDIRS += Message

PROJECT_SOURCEFILES += file.c
PROJECT_SOURCEFILES += message.c
PROJECT_SOURCEFILES += circullar_buffer.c
PROJECT_SOURCEFILES += message_handler.c
CONTIKI = ../..

include $(CONTIKI)/Makefile.include
