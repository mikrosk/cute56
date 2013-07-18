#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget( QWidget* parent = 0 );

protected:
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();

signals:
	void vbl();
};

#endif // GLWIDGET_H
