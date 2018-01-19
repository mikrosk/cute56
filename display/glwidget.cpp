#include "glwidget.h"

#include <cmath>
#include <algorithm>
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
	const Bitmap* pScreen = AtariThread::screenBitmap();
	if( pScreen == 0) return;

	setMinimumSize( pScreen->width*2, pScreen->height*2);
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
	if( pScreen == 0) return;

	int winWidth = this->width();
	int winHeight = this->height();
	int height = pScreen->height;
	int width = pScreen->width;

	float zoomFactor = std::min( (winWidth/(float)width),(winHeight/(float)height) );

	int xSize = (int)std::ceil(width*zoomFactor);
	int ySize = (int)std::ceil(height*zoomFactor);
	int yPad = std::max(winHeight-ySize, 0)/2;
	int xPad = std::max(winWidth-xSize, 0)/2;

	glRasterPos2i( xPad, ySize+yPad );
	glPixelZoom( zoomFactor, -zoomFactor);

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glDrawPixels( width, height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pScreen->pixels.pUnknown );

	emit vbl();
}
