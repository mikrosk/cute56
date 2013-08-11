#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QMainWindow>
#include <QSet>

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
	void setBreakpoint( int row, int col );
	void setupExecution();
	void updateP();
	void updateX();
	void updateY();
	void updateL();

	void setRegisterX0();
	void setRegisterX1();
	void setRegisterY0();
	void setRegisterY1();
	void setRegisterA0();
	void setRegisterA1();
	void setRegisterA2();
	void setRegisterB0();
	void setRegisterB1();
	void setRegisterB2();
	void setRegisterR0();
	void setRegisterR1();
	void setRegisterR2();
	void setRegisterR3();
	void setRegisterR4();
	void setRegisterR5();
	void setRegisterR6();
	void setRegisterR7();
	void setRegisterN0();
	void setRegisterN1();
	void setRegisterN2();
	void setRegisterN3();
	void setRegisterN4();
	void setRegisterN5();
	void setRegisterN6();
	void setRegisterN7();
	void setRegisterM0();
	void setRegisterM1();
	void setRegisterM2();
	void setRegisterM3();
	void setRegisterM4();
	void setRegisterM5();
	void setRegisterM6();
	void setRegisterM7();
	void setRegisterSR();
	void setRegisterLC();
	void setRegisterPC();

private:
	QTableWidgetItem* itemAtPc( Uint16 pc ) const;

	Ui::Debugger *ui;

	dsp_core_t*	m_pCurrentDspCore;
	QSet<quint16>	m_breakpoints;
	bool		m_autoStepping;
};

#endif // DEBUGGER_H
