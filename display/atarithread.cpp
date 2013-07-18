#include "atarithread.h"

#include <QMessageBox>
#include <QApplication>

typedef qint32 (*MainPrototype)( const DspWrapperInfo* pDspWrapperInfo );
typedef void (*SysExitPrototype)( qint32 code );
typedef void (*TimerDPrototype)();
typedef void (*TimerVblPrototype)();
typedef Bitmap* (*ScreenGetPhysicalPrototype)();

static MainPrototype Main;
static SysExitPrototype SysExit;
static TimerDPrototype TimerDCallback;
static TimerVblPrototype TimerVblCallback;
static ScreenGetPhysicalPrototype ScreenGetPhysical;

AtariThread::AtariThread( QObject* parent )
	: QThread( parent )
	, m_pdspWrapperInfo( 0 )
{
	// TODO: mutexes between timers, reload etc
}

AtariThread::~AtariThread()
{

}

void AtariThread::setDspWrapper( const DspWrapperInfo* pDspWrapperInfo )
{
	m_pdspWrapperInfo = pDspWrapperInfo;
	if( !isRunning() )
	{
		start();
	}
}

void AtariThread::loadBinary( const QString& path )
{
	if( m_library.isLoaded() )
	{
		systemExit();
		m_library.unload();
	}
	m_library.setFileName( path );
	if( !m_library.load() )
	{
		QMessageBox::warning( 0, QApplication::applicationName(), "Failed to load Atari Library" );
		return;
	}

	Main = reinterpret_cast<MainPrototype>( m_library.resolve( "Main" ) );
	if( Main == 0 )
	{
		QMessageBox::warning( 0, QApplication::applicationName(), "Failed to resolve Main()" );
		return;
	}

	TimerDCallback = reinterpret_cast<TimerDPrototype>( m_library.resolve( "TimerDCallback" ) );
	if( TimerDCallback == 0 )
	{
		QMessageBox::warning( 0, QApplication::applicationName(), "Failed to resolve TimerDCallback()" );
		return;
	}

	TimerVblCallback = reinterpret_cast<TimerVblPrototype>( m_library.resolve( "TimerVblCallback" ) );
	if( TimerVblCallback == 0 )
	{
		QMessageBox::warning( 0, QApplication::applicationName(), "Failed to resolve TimerVblCallback()" );
		return;
	}

	SysExit = reinterpret_cast<SysExitPrototype>( m_library.resolve( "SysExit" ) );
	if( SysExit == 0 )
	{
		QMessageBox::warning( 0, QApplication::applicationName(), "Failed to resolve SysExit()" );
		return;
	}

	ScreenGetPhysical = reinterpret_cast<ScreenGetPhysicalPrototype>( m_library.resolve( "ScreenGetPhysical" ) );
	if( ScreenGetPhysical == 0 )
	{
		QMessageBox::warning( 0, QApplication::applicationName(), "Failed to resolve ScreenGetPhysical()" );
		return;
	}

	if( !isRunning() )
	{
		start();
	}
}

void AtariThread::run()
{
	if( Main != 0 && m_pdspWrapperInfo != 0 )
	{
		Main( m_pdspWrapperInfo );
	}
}

// these are called from GUI thread but we don't care that much
// it's much easier to do this way than to introduce a whole bunch
// of interrupt emulation

Bitmap* AtariThread::screenBitmap()
{
	if( ScreenGetPhysical != 0 )
	{
		return ScreenGetPhysical();
	}
	else
	{
		return 0;
	}
}

void AtariThread::timerD()
{
    if( TimerDCallback != 0 )
    {
	    TimerDCallback();
    }
}

void AtariThread::timerVbl()
{
    if( TimerVblCallback != 0 )
    {
	    TimerVblCallback();
    }
}

void AtariThread::systemExit()
{
	if( SysExit != 0 )
	{
		SysExit( 0 );
	}
}
