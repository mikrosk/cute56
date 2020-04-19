#include "glwidget.h"

#include <QImage>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QTimer>

#include "atarithread.h"

GLWidget::GLWidget( QWidget* parent )
	: QOpenGLWidget( parent )
{
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000 / SCREEN_RATE);
}

void GLWidget::paintGL()
{
	const Bitmap* pScreen = AtariThread::screenBitmap();
	if(!pScreen || !pScreen->pixels.pUnknown)
		return;

	if (!m_pScreenImage) {
		m_pScreenImage = new QImage(pScreen->width, pScreen->height, QImage::Format_RGB16);
		setMinimumSize(pScreen->width * 2, pScreen->height * 2);
	}

	memcpy(m_pScreenImage->bits(), pScreen->pixels.pUnknown, pScreen->width * pScreen->height * 2);

	QOpenGLPaintDevice device(width(), height());
	QPainter painter;
	painter.begin(&device);
	painter.drawImage(QRect(0, 0, width(), height()), *m_pScreenImage, QRect(0, 0, m_pScreenImage->width(), m_pScreenImage->height()));
	painter.end();

	emit vbl();
}
