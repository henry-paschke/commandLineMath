CC = gcc
CFLAGS = -Wall
LINKERFLAGS = -lm

SRCDIR = src
OUTDIR = build

TARGET = $(OUTDIR)/cmdmath
SRCS = $(addprefix $(SRCDIR)/, main.c cmdfunctions.c token.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LINKERFLAGS)

clean:
	rm -f $(TARGET)