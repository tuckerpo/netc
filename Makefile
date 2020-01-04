CC = gcc
CFLAGS = -g -Wall -Werror -I. 

TARGET = echo

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)

