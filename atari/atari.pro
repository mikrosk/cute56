QT	-= gui

TARGET	= atari
TEMPLATE= lib

DEFINES	+= \
    HOST \
    ATARI_LIBRARY

unix:QMAKE_CFLAGS += -std=c99
else:{
    win32-g++:QMAKE_CFLAGS += -std=c99
    else:QMAKE_CFLAGS += /TP
}

SOURCES	+= \
	atari.c \
	dsp.c

HEADERS += \
	atari.h \
	dsp.h \
	graphics.h \
	atari_internal.h \
	dsp_internal.h \
    atari_global.h

DSP_SOURCES = \
	calc.asm

win32 {
	ASM56000 = C:\Program Files (x86)\Motorola\DSP\dsp\bin\asm56000.exe
	CLDLOD	 = C:\Program Files (x86)\Motorola\DSP\dsp\bin\cldlod.exe
	LOD2P56	 = $$(HOMEPATH)\bin\lod2p56.exe
        WINE     =
}
else {
        ASM56000 = ${HOME}/bin-dos/asm56000.exe
        CLDLOD	 = ${HOME}/bin-dos/cldlod.exe
        LOD2P56	 = ${HOME}/bin/lod2p56
        WINE     = wine
}

asm56000.output	= ${QMAKE_FILE_BASE}.p56
asm56000.commands = $${WINE} $$shell_quote($${ASM56000}) -a -l -b -i${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_IN} && $${WINE} $$shell_quote($${CLDLOD}) ${QMAKE_FILE_BASE}.cld > ${QMAKE_FILE_BASE}.lod && $$shell_quote($${LOD2P56}) ${QMAKE_FILE_BASE}.lod
asm56000.input	= DSP_SOURCES
asm56000.clean	= ${QMAKE_FILE_BASE}.cld ${QMAKE_FILE_BASE}.lst ${QMAKE_FILE_BASE}.lod ${QMAKE_FILE_BASE}.p56
asm56000.CONFIG	= no_link target_predeps

QMAKE_EXTRA_COMPILERS += asm56000

OTHER_FILES += \
    calc.asm
