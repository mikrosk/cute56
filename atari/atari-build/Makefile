TARGET	= atari.tos

CC	= m68k-atari-mint-gcc
AS	= vasm
DSPAS	= asm56k

CFLAGS	= -std=c99 -g -O2 -Wall -m68020-60 -I.
ASFLAGS	= -m68030 -m68882 -quiet -Faout -x
DSPFLAGS=

LDFLAGS	= -m68020-60 -Wl,--traditional-format

CSOURCES= \
	../atari.c \
	../dsp.c
	
ASMSOURCES= \
	screen-asm.s \
	timerd-asm.s \
	timervbl-asm.s \
	dsp_fix-asm.s

DSPSOURCES= \
	../calc.asm

COBJS	= $(CSOURCES:%.c=%.o)
SOBJS	= $(ASMSOURCES:%.s=%.o)
DSPOBJS	= $(DSPSOURCES:%.asm=%.p56)

default: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.p56: %.asm
	$(DSPAS) $(DSPFLAGS) -p $@ $<
	cp $@ $(PWD)

$(TARGET): $(DSPOBJS) $(SOBJS) $(COBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(SOBJS) $(COBJS)

clean:
	rm -f $(TARGET) $(DSPOBJS) $(SOBJS) $(COBJS) *~
