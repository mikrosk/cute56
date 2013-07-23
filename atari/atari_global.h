#ifndef ATARI_GLOBAL_H
#define ATARI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ATARI_LIBRARY)
#  define ATARISHARED_EXPORT Q_DECL_EXPORT
#else
#  define ATARISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ATARI_GLOBAL_H
