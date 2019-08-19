CFLAGS = -L/usr/local/lib -fPIC
INCLUDE_DIRS = -I /usr/local/include/libxml2 -I ./include -I ./utils/memory/include -I ./utils/log
TARGET = apn_parser
LIBS = -lxml2
CC = gcc

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
	sudo cp ./include/volteConfig.h /usr/local/include
	sudo cp ./include/apnConfig.h /usr/local/include
	sudo cp $(TARGET) /usr/lib

clean:
	sudo rm -rf /usr/local/include/volteConfig.h
	sudo rm -rf /usr/local/include/apnConfig.h
	sudo rm -rf /usr/lib/$(TARGET)
	rm -f *.o
	rm -f $(TARGET)