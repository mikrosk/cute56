#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

class QImage;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
	explicit GLWidget(QWidget* parent = 0);

protected:
    virtual void paintGL();

signals:
	void vbl();

private:
	QImage* m_pScreenImage = { nullptr };
};

#endif // GLWIDGET_H
