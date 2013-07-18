#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QMainWindow>

#include "dsp_core.h"

namespace Ui {
class Debugger;
}

class QTableWidgetItem;

class Debugger : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit Debugger(QWidget *parent = 0);
	~Debugger();

public slots:
	void initData( dsp_core_t* pCore );
	void updateData( const dsp_core_t* pCore );

signals:
	void stepInto();
	void stepOver();
	void stepSkip();
	void closed();

protected:
	void closeEvent( QCloseEvent* event );

private slots:
	void updateP();
	void updateX();
	void updateY();
	void updateL();

private:
	QTableWidgetItem* itemAtPc( Uint16 pc ) const;

	Ui::Debugger *ui;

	dsp_core_t* m_pCurrentDspCore;
};

#endif // DEBUGGER_H
