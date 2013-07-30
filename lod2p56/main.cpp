#include <QCoreApplication>

#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QtEndian>
#include <QFileInfo>

int main( int argc, char *argv[] )
{
	QCoreApplication a( argc, argv );

	qDebug() << "lod2p56 converter 0.1";
	qDebug() << "(c) 2013 Miro Kropacek";
	qDebug() << "miro.kropacek@gmail.com";
	qDebug() << "";

	if( a.arguments().count() != 2 )
	{
		qCritical() << "Usage:" << a.arguments().first() << "<lod file>";
		exit( EXIT_FAILURE );
	}

	QString filename = a.arguments().last();

	QFile inFile( filename );
	QFile outFile( QFileInfo( filename ).path() + '/' + QFileInfo( filename ).baseName().append( ".p56" ) );
	if( !inFile.open( QFile::ReadOnly | QFile::Text ) || !outFile.open( QFile::WriteOnly ) )
	{
		qCritical() << "Open of the input/output file failed.";
		exit( EXIT_FAILURE );
	}

	quint32 memoryType;
	quint32 addr;
	QByteArray data;
	bool isData = false;
	while( !inFile.atEnd() )
	{
		QString line = inFile.readLine().simplified();
		if( line.isEmpty() )
		{
			continue;
		}
		QStringList tokens = line.split( ' ', QString::SkipEmptyParts );

		if( tokens.first().startsWith( '_' ) && isData )
		{
			// flush
			uchar buf[4];

			qToBigEndian<quint32>( memoryType, buf );
			outFile.write( (char*)buf+1, 3 );

			qToBigEndian<quint32>( addr, buf );
			outFile.write( (char*)buf+1, 3 );

			qToBigEndian<quint32>( data.size() / 3, buf );
			outFile.write( (char*)buf+1, 3 );

			outFile.write( data.data(), data.size() );

			isData = false;
		}

		if( tokens.first() == "_START" || tokens.first() == "_END" )
		{
			isData = false;
		}
		else if( tokens.first() == "_DATA" )
		{
			// _DATA {P,X,Y} <addr>
			tokens.pop_front();

			if( tokens.first() == "P" )
			{
				memoryType = 0;
			}
			else if( tokens.first() == "X" )
			{
				memoryType = 1;
			}
			else if( tokens.first() == "Y" )
			{
				memoryType = 2;
			}
			else
			{
				qCritical() << "Unknown memory type" << tokens.first();
				exit( EXIT_FAILURE );
			}

			tokens.pop_front();
			addr = tokens.first().toInt( 0, 16 );

			isData = true;
			data.clear();
		}
		else if( tokens.first() == "_SYMBOL" )
		{
			// _SYMBOL {P,X,Y,N}
			isData = false;
		}
		else if( tokens.first().startsWith( '_' ) )
		{
			qWarning() << "Unknown section" << tokens.first();
			isData = false;
		}
		else
		{
			foreach( QString token, tokens )
			{
				uchar buf[4];
				qToBigEndian<quint32>( token.toInt( 0, 16 ), buf );
				data.append( (char*)buf+1, 3 );
			}
		}
	}

	return 0;
}
