CFLAGS = -L/usr/local/lib -fPIC
INCLUDE_DIRS = -I /usr/local/include/libxml2 -I ./inc
TARGET = libvolte_config.so
LIBS = -lxml2
CC = gcc

OBJS = volteConfig.o \
        apnConfig.o \
        xmlAssistantAdpter.o \
        xmlAssistantImpl.o

SRCS = src/volteConfig/volteConfig.c \
        src/apnConfig/apnConfig.c \
        src/xmlAssistant/xmlAssistantAdpter.c \
        src/xmlAssistant/xmlAssistantImpl.c

all:$(OBJS)
	$(CC) -shared $(CFLAGS) $(LIBS) -o $(TARGET) $(OBJS)

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE_DIRS) -c $^

install:
	sudo cp ./inc/volteConfig.h /usr/local/include
	sudo cp ./inc/apnConfig.h /usr/local/include
	sudo cp $(TARGET) /usr/lib

clean:
	sudo rm -rf /usr/local/include/volteConfig.h
	sudo rm -rf /usr/local/include/apnConfig.h
	sudo rm -rf /usr/lib/$(TARGET)
	rm -f *.o
	rm -f $(TARGET)