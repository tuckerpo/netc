CC = gcc
CFLAGS = -g -Wall -Werror -I. 

TARGET = http

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)

