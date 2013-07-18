#ifndef ATARITHREAD_H
#define ATARITHREAD_H

#include <QThread>

#include <QLibrary>

#include "atari.h"

class AtariThread : public QThread
{
	Q_OBJECT

public:
	AtariThread( QObject* parent = 0 );
	~AtariThread();

	static Bitmap* screenBitmap();

public slots:
	void setDspWrapper( const DspWrapperInfo* pDspWrapperInfo );
	void loadBinary( const QString& path );
	void timerD();
	void timerVbl();
	void systemExit();

protected:
	void run();

private:
	QLibrary m_library;
	const DspWrapperInfo* m_pdspWrapperInfo;

};

#endif // ATARITHREAD_H
