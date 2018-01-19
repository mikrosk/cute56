TEMPLATE= lib
TARGET	= dsp
QT      -= gui

win32:{
    win32-g++:
    else:QMAKE_CFLAGS += /D_CRT_SECURE_NO_WARNINGS
}


DEFINES += \
    DSP_LIBRARY

HEADERS += \
    types.h \
    dsp_disasm.h \
    dsp_cpu.h \
    dsp_core.h \
    dsp_global.h

SOURCES += \
    dsp_disasm.c \
    dsp_cpu.c \
    dsp_core.c
