#include "glwidget.h"

#include <QDebug>
#include <QTimer>

#include "atarithread.h"

GLWidget::GLWidget( QWidget* parent )
	: QGLWidget( parent )
{
	QTimer* timer = new QTimer( this );
	connect( timer, SIGNAL( timeout() ), this, SLOT( updateGL() ) );
	timer->start( 1000 / SCREEN_RATE );
}

void GLWidget::initializeGL()
{
}

void GLWidget::resizeGL( int w, int h )
{
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, w, 0, h, -1, 1 );
}

void GLWidget::paintGL()
{
	const Bitmap* pScreen = AtariThread::screenBitmap();
	if( pScreen != 0 )
	{
        //glRasterPos2f( 0, 2 * pScreen->height - 0.1 );
        glRasterPos2i( 0, 2 * pScreen->height );
		glPixelZoom( 2, -2 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glDrawPixels( pScreen->width, pScreen->height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pScreen->pixels.pUnknown );

		setMinimumSize( 2 * pScreen->width, 2 * pScreen->height );
		setMaximumSize( 2 * pScreen->width, 2 * pScreen->height );

		QWidget* parent = dynamic_cast<QWidget*>( this->parent() );
		if( parent != 0 )
		{
			parent->resize( 2 * pScreen->width, 2 * pScreen->height );
		}
	}

	emit vbl();
}
