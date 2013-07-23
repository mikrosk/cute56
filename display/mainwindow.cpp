#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

#include "moduledialog.h"
#include "glwidget.h"

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    m_dspWrapper.moveToThread( &m_dspThread );

    // connect atari thread
    connect( this, SIGNAL(atariPathAvailable(QString)), &m_atariThread, SLOT(loadBinary(QString)) );
    connect( &m_dspWrapper, SIGNAL(dspInitialized(const DspWrapperInfo*)), &m_atariThread, SLOT(setDspWrapper(const DspWrapperInfo*)) );
    m_atariThread.start();

    // connect dsp thread
    connect( this, SIGNAL(dspPathAvailable(QString)), &m_dspWrapper, SLOT(loadBinary(QString)) );
    m_dspThread.start();

    // create GL Widget
    GLWidget* pGlWidget = new GLWidget( this );
    connect( pGlWidget, SIGNAL(vbl()), &m_atariThread, SLOT(timerVbl()) );
    setCentralWidget( pGlWidget );

    // setup timers
    QTimer* pTimer = new QTimer( this );
    connect( pTimer, SIGNAL(timeout()), &m_atariThread, SLOT(timerD()) );
    pTimer->start( 1000 / TIMER_D_RATE );

    // debug signals
    connect( ui->action_Enable, SIGNAL(triggered(bool)), &m_dspWrapper, SLOT(enableDebug(bool)) );
    connect( &m_dspDebugger, SIGNAL(stepInto()), &m_dspWrapper, SLOT(debugInto()) );
    connect( &m_dspDebugger, SIGNAL(stepOver()), &m_dspWrapper, SLOT(debugOver()) );
    connect( &m_dspDebugger, SIGNAL(stepSkip()), &m_dspWrapper, SLOT(debugSkip()) );

    connect( &m_dspWrapper, SIGNAL(debugStarted(dsp_core_t*)), &m_dspDebugger, SLOT(initData(dsp_core_t*)) );
    connect( &m_dspWrapper, SIGNAL(debugStep(const dsp_core_t*)), &m_dspDebugger, SLOT(updateData(const dsp_core_t*)) );

    // misc stuff
    connect( ui->actionProgram, SIGNAL(triggered()), this, SLOT(programDialog()) );
    connect( ui->actionDSP_Engine, SIGNAL(triggered()), this, SLOT(dspEngineDialog()) );
    connect( ui->action_Enable, SIGNAL(triggered(bool)), &m_dspDebugger, SLOT(setVisible(bool)) );
    connect( &m_dspDebugger, SIGNAL(closed()), ui->action_Enable, SLOT(trigger()) );

    // load defaults
    programDialog( false );
    dspEngineDialog( false );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::programDialog( bool show )
{
    ModuleDialog dlg( "Atari executable", this );
    if( !show || dlg.exec() == QDialog::Accepted )
    {
	emit atariPathAvailable( dlg.path() );
    }
}

void MainWindow::dspEngineDialog( bool show )
{
    ModuleDialog dlg( "DSP Binary", this );
    if( !show || dlg.exec() == QDialog::Accepted )
    {
	emit dspPathAvailable( dlg.path() );
    }
}
