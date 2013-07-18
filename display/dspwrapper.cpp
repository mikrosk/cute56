#include "dspwrapper.h"

#include <QtEndian>
#include <QFile>
#include <QByteArray>
#include <QMutexLocker>

#include "dsp_cpu.h"

// following functions are called from atari thread but it doesn't matter
// that much as Qt only reads DSP regs and memory so there's no real
// race condition when it comes to these two threads
static qint8 dspReceiveByte( const void* pObject )
{
	return ((DspWrapper*)pObject)->receiveByte();
}

static qint16 dspReceiveWord( const void* pObject )
{
	return ((DspWrapper*)pObject)->receiveWord();
}

static qint32 dspReceiveSignedLong( const void* pObject )
{
	return ((DspWrapper*)pObject)->receiveSignedLong();
}

static quint32 dspReceiveUnsignedLong( const void* pObject )
{
	return ((DspWrapper*)pObject)->receiveUnsignedLong();
}

static void dspSendLong( const void* pObject, qint32 value )
{
	return ((DspWrapper*)pObject)->sendLong( value );
}

DspWrapper::DspWrapper( QObject* parent )
	: QObject( parent )
	, m_debugOn( false )
{
}

quint32 DspWrapper::getDspWord( const char* pDspBinary )
{
	quint32 dspWord;
	dspWord = qFromBigEndian<quint32>( (uchar*)pDspBinary );
	dspWord >>= 8;

	return dspWord;
}

void DspWrapper::loadBinary( const QString& path )
{
	QFile file( path );
	file.open( QFile::ReadOnly );
	QByteArray fileContent = file.readAll();
	if( fileContent.isEmpty() )
	{
		return;
	}

	const char* pDspBinary = fileContent.constData();
	int cBytes = fileContent.size();

	// real init
	const char* pDspBinaryEnd = pDspBinary + cBytes;

	dsp_core_init( &dsp_core );
	dsp_core_reset( &dsp_core );

	while( pDspBinary < pDspBinaryEnd )
	{
		quint32 dspWordType;
		quint32 dspWordStart;
		quint32 dspWordBytes;

		dspWordType = getDspWord( pDspBinary );
		pDspBinary += 3;

		// 0: p 1: x 2: y
		// emulator uses xyp indices
		if( dspWordType == 0 )
		{
			dspWordType = DSP_SPACE_P;
		}
		else if( dspWordType == 1 )
		{
			dspWordType = DSP_SPACE_X;
		}
		else if( dspWordType == 2 )
		{
			dspWordType = DSP_SPACE_Y;
		}
		else
		{
			return;
		}

		dspWordStart = getDspWord( pDspBinary );
		pDspBinary += 3;

		dspWordBytes = getDspWord( pDspBinary );
		pDspBinary += 3;

		while( dspWordBytes-- > 0 )
		{
			write_memory_raw( dspWordType, dspWordStart++, getDspWord( pDspBinary ) );
			pDspBinary += 3;
		}
	}

	dsp_core.running = 1;

	static DspWrapperInfo info;
	info.pObject = this;
	info.dspReceiveByte = dspReceiveByte;
	info.dspReceiveWord = dspReceiveWord;
	info.dspReceiveSignedLong = dspReceiveSignedLong;
	info.dspReceiveUnsignedLong = dspReceiveUnsignedLong;
	info.dspSendLong = dspSendLong;

	emit dspInitialized( &info );
}

void DspWrapper::executeInstruction()
{
	QMutexLocker locker( &m_mutex );
	if( m_debugOn )
	{
		m_nextInstruction.wait( &m_mutex );
	}
	dsp56k_execute_instruction();
	if( m_debugOn )
	{
		emit debugStep( &dsp_core );
	}
}

void DspWrapper::waitForReceive()
{
	while( ( dsp_core_read_host( &dsp_core, CPU_HOST_ISR ) & ( 1 << CPU_HOST_ISR_RXDF ) ) == 0 )
	{
		executeInstruction();
	}
}

void DspWrapper::waitForTransmit()
{
	while( ( dsp_core_read_host( &dsp_core, CPU_HOST_ISR ) & ( 1 << CPU_HOST_ISR_TXDE ) ) == 0 )
	{
		executeInstruction();
	}
}

void DspWrapper::enableDebug( bool enable )
{
	QMutexLocker locker( &m_mutex );
	if( m_debugOn && !enable )
	{
		m_nextInstruction.wakeAll();
	}
	else if( !m_debugOn && enable )
	{
		emit debugStarted( &dsp_core );
	}
	m_debugOn = enable;
}

void DspWrapper::debugInto()
{
	m_nextInstruction.wakeAll();
}

void DspWrapper::debugOver()
{

}

void DspWrapper::debugSkip()
{

}

qint8 DspWrapper::receiveByte( void )
{
	waitForReceive();

	return dsp_core_read_host( &dsp_core, CPU_HOST_RXL );
}

qint16 DspWrapper::receiveWord( void )
{
	Sint16 dspWord;

	waitForReceive();

	dspWord = dsp_core_read_host( &dsp_core, CPU_HOST_RXM );
	dspWord <<= 8;
	dspWord |= dsp_core_read_host( &dsp_core, CPU_HOST_RXL );

	return dspWord;
}

qint32 DspWrapper::receiveSignedLong( void )
{
	Sint32 dspWord;

	waitForReceive();

	dspWord = (Sint8)dsp_core_read_host( &dsp_core, CPU_HOST_RXH );
	dspWord <<= 8;
	dspWord |= dsp_core_read_host( &dsp_core, CPU_HOST_RXM );
	dspWord <<= 8;
	dspWord |= dsp_core_read_host( &dsp_core, CPU_HOST_RXL );

	return dspWord;
}
quint32 DspWrapper::receiveUnsignedLong( void )
{
	Uint32 dspWord;

	waitForReceive();

	dspWord = dsp_core_read_host( &dsp_core, CPU_HOST_RXH );
	dspWord <<= 8;
	dspWord |= dsp_core_read_host( &dsp_core, CPU_HOST_RXM );
	dspWord <<= 8;
	dspWord |= dsp_core_read_host( &dsp_core, CPU_HOST_RXL );

	return dspWord;
}
void DspWrapper::sendLong( qint32 value )
{
	waitForTransmit();

	dsp_core_write_host( &dsp_core, CPU_HOST_TRXH, ( value >> 16 ) & 0xff );
	dsp_core_write_host( &dsp_core, CPU_HOST_TRXM, ( value >> 8 ) & 0xff );
	dsp_core_write_host( &dsp_core, CPU_HOST_TRXL, value & 0xff );
}
