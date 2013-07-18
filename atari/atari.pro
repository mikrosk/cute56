QT	-= core gui

TARGET	= atari
TEMPLATE= lib

DEFINES	+= HOST
QMAKE_CFLAGS += -std=c99

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
dspp56.commands	= ~/bin/asm56k -p ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} -I${QMAKE_FILE_IN_PATH}
dspp56.input	= DSP_SOURCES
dspp56.CONFIG	= no_link target_predeps

dsplod.output	= ${QMAKE_FILE_BASE}.lod
dsplod.commands	= ~/bin/asm56k -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} -I${QMAKE_FILE_IN_PATH}
dsplod.input	= DSP_SOURCES
dsplod.CONFIG	= no_link target_predeps

QMAKE_EXTRA_COMPILERS += dspp56 dsplod

OTHER_FILES += \
    calc.asm
