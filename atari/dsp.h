#ifndef DSP_H
#define DSP_H

#ifdef HOST
#include <stdint.h>

typedef struct
{
	const void* pObject;
	int8_t	(*dspReceiveByte)( const void* pObject );
	int16_t	(*dspReceiveWord)( const void* pObject );
	int32_t	(*dspReceiveSignedLong)( const void* pObject );
	uint32_t (*dspReceiveUnsignedLong)( const void* pObject );
	void	(*dspSendLong)( const void* pObject, int32_t value );
} DspWrapperInfo;

void setDspWrapper( const DspWrapperInfo* pDspWrapperInfo );
#else
int loadDspBinary( const char* sPath );
#endif

#endif // DSP_H
