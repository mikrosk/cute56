TARGET	= atari.tos

CC	= m68k-atari-mint-gcc

CFLAGS	= -std=c99 -O2 -I..

SOURCES	= \
	../atari.c \
	../dsp.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.o *~
