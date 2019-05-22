QT	+= core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET	= cute56
TEMPLATE = app

SOURCES += main.cpp \
	mainwindow.cpp \
	glwidget.cpp \
	moduledialog.cpp \
	dspwrapper.cpp \
	atarithread.cpp \
	debugger.cpp

HEADERS  += mainwindow.h \
	glwidget.h \
	moduledialog.h \
	dspwrapper.h \
	atarithread.h \
	debugger.h

FORMS    += mainwindow.ui \
	moduledialog.ui \
	debugger.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dsplib/release/ -ldsp -lOpenGL32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dsplib/debug/ -ldsp -lOpenGL32
else:unix: LIBS += -L$$OUT_PWD/../dsplib/ -ldsp

INCLUDEPATH += $$PWD/../dsplib
DEPENDPATH += $$PWD/../dsplib

# just to see the header file
INCLUDEPATH += $$PWD/../atari
# and to enable host features
DEFINES	+= HOST
