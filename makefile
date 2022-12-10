CC       = g++
CFLAGS   = -Wall -g -lpthread -lrt
LDFLAGS  = -lreadline
OBJFILES = cafe.o
TARGET   = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~