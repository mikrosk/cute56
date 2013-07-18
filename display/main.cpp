#include "mainwindow.h"

#include <QApplication>

int main( int argc, char* argv[] )
{
    QApplication a( argc, argv );

    a.setOrganizationName( "Mystic Bytes" );
    a.setOrganizationDomain( "mikro.atari.org" );
    a.setApplicationName( "DSP Engine Emulator" );

    MainWindow w;
    w.show();
    
    return a.exec();
}
