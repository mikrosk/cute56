#ifndef DSP_GLOBAL_H
#define DSP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WIN32)
#if defined(DSP_LIBRARY)
#  define DSPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DSPSHARED_EXPORT Q_DECL_IMPORT
#endif
#else
// we don't want this C++ mess on C compilers
#  define DSPSHARED_EXPORT
#endif

#endif // DSP_GLOBAL_H
