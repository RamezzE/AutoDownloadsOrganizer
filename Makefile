CC = g++
CFLAGS = -std=c++17
LDFLAGS = -mwindows

# Additional libraries
LIBS = -lole32 -lshell32 -luuid

SRCS = main.cpp
OBJS = main.o
TARGET = AutoDownloadsOrganizer.exe

all: $(TARGET) cleanObj

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS) 

cleanObj:
	del $(OBJS)

clean:
	del $(OBJS) 
	del $(TARGET)
