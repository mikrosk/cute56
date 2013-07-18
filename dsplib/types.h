#ifndef DSP_TYPES_H
#define DSP_TYPES_H

#if defined _MSC_VER && !defined __cplusplus && !defined inline
#define inline __inline
typedef enum { false, true } bool;
#endif

typedef signed char		Sint8;
typedef signed short	Sint16;
typedef signed int		Sint32;

typedef unsigned char	Uint8;
typedef unsigned short	Uint16;
typedef unsigned int	Uint32;

#endif
