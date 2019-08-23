os_platform =

CFLAGS = -D __CONSEQUENT_NODE -L/usr/local/lib -fPIC -g -Wall 
ifeq ($(os_platform),macos)
INCLUDE_DIRS = -I /usr/local/opt/libxml2/include/libxml2/  -I ./include -I ./utils/memory/include -I ./utils/log
else
INCLUDE_DIRS = -I /usr/local/include/libxml2 -I ./include -I ./utils/memory/include -I ./utils/log
endif
TARGET = apn_parser
LIBS = -lxml2
ifeq ($(os_platform),macos)
CC = clang
else
CC = gcc
endif

OBJS =  apnConfig.o \
        xmlAssistantAdpter.o \
        xmlAssistantImpl.o \
	    memory.o \
	    main.o

SRCS =  src/apnConfig/apnConfig.c \
        src/xmlAssistant/xmlAssistantAdpter.c \
        src/xmlAssistant/xmlAssistantImpl.c \
	    utils/memory/src/memory.c \
	    main.c

all:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(TARGET) $(OBJS)

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE_DIRS) -c $^

install:
	sudo cp $(TARGET) /usr/lib

clean:
	sudo rm -rf /usr/lib/$(TARGET)
	rm -f *.o
	rm -f $(TARGET)