QT	-= core gui

TARGET	= atari
TEMPLATE= lib

DEFINES	+= HOST
win32:QMAKE_CFLAGS += /TP
else:QMAKE_CFLAGS += -std=c99

SOURCES	+= \
	atari.c \
	dsp.c

HEADERS += \
	atari.h \
	dsp.h \
	graphics.h \
	atari_internal.h \
	dsp_internal.h

DSP_SOURCES = \
	calc.asm

# 'asm56k' points to https://bitbucket.org/sqward/asm56k build
dspp56.output	= ${QMAKE_FILE_BASE}.p56
win32:dspp56.commands	= c:\MinGW\msys\1.0\home\MiroslavK\asm56k\asm56k.exe -p ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} -I${QMAKE_FILE_IN_PATH}
else:dspp56.commands	= ~/bin/asm56k -p ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} -I${QMAKE_FILE_IN_PATH}
dspp56.input	= DSP_SOURCES
dspp56.CONFIG	= no_link target_predeps

dsplod.output	= ${QMAKE_FILE_BASE}.lod
win32:dsplod.commands	= c:\MinGW\msys\1.0\home\MiroslavK\asm56k\asm56k.exe -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} -I${QMAKE_FILE_IN_PATH}
else:dspp56.commands	= ~/bin/asm56k -p ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} -I${QMAKE_FILE_IN_PATH}
dsplod.input	= DSP_SOURCES
dsplod.CONFIG	= no_link target_predeps

QMAKE_EXTRA_COMPILERS += dspp56 dsplod

OTHER_FILES += \
    calc.asm
