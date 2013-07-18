#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "atarithread.h"
#include "debugger.h"
#include "dspwrapper.h"

class DspWrapper;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow();

signals:
	void dspPathAvailable( const QString& path );
	void atariPathAvailable( const QString& path );

private slots:
    void programDialog( bool show = true );
    void dspEngineDialog( bool show = true );
    
private:
    Ui::MainWindow *ui;

    AtariThread	m_atariThread;
    QThread	m_dspThread;
    DspWrapper	m_dspWrapper;
    Debugger	m_dspDebugger;
};

#endif // MAINWINDOW_H
