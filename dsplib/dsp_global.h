#ifndef DSP_GLOBAL_H
#define DSP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DSP_LIBRARY)
#  define DSPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DSPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DSP_GLOBAL_H
