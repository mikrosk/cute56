#ifndef DSPWRAPPER_H
#define DSPWRAPPER_H

#include <QObject>

#include <QMutex>
#include <QWaitCondition>

#include "dsp_core.h"
#include "atari.h"

class DspWrapper : public QObject
{
	Q_OBJECT

public:
	explicit DspWrapper( QObject *parent = 0 );

	qint8	receiveByte( void );
	qint16	receiveWord( void );
	qint32	receiveSignedLong( void );
	quint32	receiveUnsignedLong( void );
	void sendLong( qint32 value );

signals:
	void debugStep( const dsp_core_t* pCore );
	void debugStarted( dsp_core_t* pCore );
	void dspInitialized( const DspWrapperInfo* pDspWrapperInfo );
	
public slots:
	void loadBinary( const QString& path );
	void enableDebug( bool enable );
	void debugInto();
	void debugOver();
	void debugSkip();

private:
	quint32 getDspWord( const char* pDspBinary );

	void executeInstruction();
	void waitForReceive();
	void waitForTransmit();

	bool	m_debugOn;

	dsp_core_t dsp_core;

	QMutex	m_mutex;
	QWaitCondition m_nextInstruction;
};

#endif // DSPWRAPPER_H
