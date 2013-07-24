#ifndef ATARI_GLOBAL_H
#define ATARI_GLOBAL_H

#if defined(HOST) && defined(WIN32)
#include <QtCore/qglobal.h>
#if defined(ATARI_LIBRARY)
#  define ATARISHARED_EXPORT Q_DECL_EXPORT
#else
#  define ATARISHARED_EXPORT Q_DECL_IMPORT
#endif
#else
// we don't want this C++ mess on C compilers
#  define ATARISHARED_EXPORT
#endif

#endif // ATARI_GLOBAL_H
