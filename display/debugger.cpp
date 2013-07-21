#include "debugger.h"
#include "ui_debugger.h"

#include <QTableWidgetItem>

#include "dsp_cpu.h"
#include "dsp_disasm.h"

Debugger::Debugger(QWidget *parent)
	: QMainWindow( parent )
	, ui(new Ui::Debugger)
{
	ui->setupUi(this);

	connect( ui->actionStep_Into, SIGNAL(triggered()), this, SIGNAL(stepInto()) );
	connect( ui->actionStep_Over, SIGNAL(triggered()), this, SIGNAL(stepOver()) );
	connect( ui->actionStep_Skip, SIGNAL(triggered()), this, SIGNAL(stepSkip()) );

	connect( ui->radioButtonP, SIGNAL(clicked()), this, SLOT(updateP()) );
	connect( ui->radioButtonX, SIGNAL(clicked()), this, SLOT(updateX()) );
	connect( ui->radioButtonY, SIGNAL(clicked()), this, SLOT(updateY()) );
	connect( ui->radioButtonL, SIGNAL(clicked()), this, SLOT(updateL()) );

	connect( ui->lineEditX0, SIGNAL(editingFinished()), this, SLOT(setRegisterX0()) );
	connect( ui->lineEditX1, SIGNAL(editingFinished()), this, SLOT(setRegisterX1()) );
	connect( ui->lineEditY0, SIGNAL(editingFinished()), this, SLOT(setRegisterY0()) );
	connect( ui->lineEditY1, SIGNAL(editingFinished()), this, SLOT(setRegisterY1()) );
	connect( ui->lineEditA0, SIGNAL(editingFinished()), this, SLOT(setRegisterA0()) );
	connect( ui->lineEditA1, SIGNAL(editingFinished()), this, SLOT(setRegisterA1()) );
	connect( ui->lineEditA2, SIGNAL(editingFinished()), this, SLOT(setRegisterA2()) );
	connect( ui->lineEditB0, SIGNAL(editingFinished()), this, SLOT(setRegisterB0()) );
	connect( ui->lineEditB1, SIGNAL(editingFinished()), this, SLOT(setRegisterB1()) );
	connect( ui->lineEditB2, SIGNAL(editingFinished()), this, SLOT(setRegisterB2()) );
	connect( ui->lineEditR0, SIGNAL(editingFinished()), this, SLOT(setRegisterR0()) );
	connect( ui->lineEditR1, SIGNAL(editingFinished()), this, SLOT(setRegisterR1()) );
	connect( ui->lineEditR2, SIGNAL(editingFinished()), this, SLOT(setRegisterR2()) );
	connect( ui->lineEditR3, SIGNAL(editingFinished()), this, SLOT(setRegisterR3()) );
	connect( ui->lineEditR4, SIGNAL(editingFinished()), this, SLOT(setRegisterR4()) );
	connect( ui->lineEditR5, SIGNAL(editingFinished()), this, SLOT(setRegisterR5()) );
	connect( ui->lineEditR6, SIGNAL(editingFinished()), this, SLOT(setRegisterR6()) );
	connect( ui->lineEditR7, SIGNAL(editingFinished()), this, SLOT(setRegisterR7()) );
	connect( ui->lineEditN0, SIGNAL(editingFinished()), this, SLOT(setRegisterN0()) );
	connect( ui->lineEditN1, SIGNAL(editingFinished()), this, SLOT(setRegisterN1()) );
	connect( ui->lineEditN2, SIGNAL(editingFinished()), this, SLOT(setRegisterN2()) );
	connect( ui->lineEditN3, SIGNAL(editingFinished()), this, SLOT(setRegisterN3()) );
	connect( ui->lineEditN4, SIGNAL(editingFinished()), this, SLOT(setRegisterN4()) );
	connect( ui->lineEditN5, SIGNAL(editingFinished()), this, SLOT(setRegisterN5()) );
	connect( ui->lineEditN6, SIGNAL(editingFinished()), this, SLOT(setRegisterN6()) );
	connect( ui->lineEditN7, SIGNAL(editingFinished()), this, SLOT(setRegisterN7()) );
	connect( ui->lineEditM0, SIGNAL(editingFinished()), this, SLOT(setRegisterM0()) );
	connect( ui->lineEditM1, SIGNAL(editingFinished()), this, SLOT(setRegisterM1()) );
	connect( ui->lineEditM2, SIGNAL(editingFinished()), this, SLOT(setRegisterM2()) );
	connect( ui->lineEditM3, SIGNAL(editingFinished()), this, SLOT(setRegisterM3()) );
	connect( ui->lineEditM4, SIGNAL(editingFinished()), this, SLOT(setRegisterM4()) );
	connect( ui->lineEditM5, SIGNAL(editingFinished()), this, SLOT(setRegisterM5()) );
	connect( ui->lineEditM6, SIGNAL(editingFinished()), this, SLOT(setRegisterM6()) );
	connect( ui->lineEditM7, SIGNAL(editingFinished()), this, SLOT(setRegisterM7()) );
	connect( ui->lineEditSR, SIGNAL(editingFinished()), this, SLOT(setRegisterSR()) );
	connect( ui->lineEditPC, SIGNAL(editingFinished()), this, SLOT(setRegisterPC()) );
}

Debugger::~Debugger()
{
	delete ui;
}

void Debugger::initData( dsp_core_t* pCore )
{
	m_pCurrentDspCore = pCore;

	char buff[1023+1];
	// some disasm parts depend on correct pc value...
	Uint16 pc = pCore->pc;
	for( Uint16 addr = pCore->pc = 0x0000, len = dsp56k_disasm_addr( addr, buff );
	     addr < 0x1000;
	     addr += len, pCore->pc += len, len = dsp56k_disasm_addr( addr, buff ) )
	{
		QStringList tokens = QString( buff ).split( '\t', QString::SkipEmptyParts );

		int row = ui->tableWidgetCode->rowCount();
		ui->tableWidgetCode->setRowCount( row + 1 );

		ui->tableWidgetCode->setItem( row, 0, new QTableWidgetItem( QString::number( addr, 16 ).rightJustified( 4, '0' ).toUpper() ) );
		// TODO: what about smc code?
		ui->tableWidgetCode->setItem( row, 1, new QTableWidgetItem( tokens.takeFirst().rightJustified( 6, '0' ).toUpper() ) );
		if( len > 1 )
		{
			ui->tableWidgetCode->setItem( row, 2, new QTableWidgetItem( tokens.takeFirst().rightJustified( 6, '0' ).toUpper() ) );
		}
		for( int col = 3; !tokens.isEmpty(); ++col )
		{
			bool isX = tokens.first().contains( "x:" );
			bool isY = tokens.first().contains( "y:" );

			if( ( col == 4 && ( isX || isY ) )
			    || ( col == 5 && isY ) )
			{
				continue;
			}

			ui->tableWidgetCode->setItem( row, col, new QTableWidgetItem( tokens.takeFirst().simplified().replace( "0x", "$" ) ) );
		}
	}

	ui->tableWidgetCode->resizeRowsToContents();

	// strange workaround...
	ui->tableWidgetCode->setVisible( false );
	ui->tableWidgetCode->resizeColumnsToContents();
	ui->tableWidgetCode->setVisible( true );

	pCore->pc = pc;

	updateData( pCore );
}

void Debugger::updateData( const dsp_core_t* pCore )
{
	int regs = sizeof( pCore->registers ) / sizeof( pCore->registers[0] );

	for( int i = 0; i < regs; ++i )
	{
		QString str = QString::number( pCore->registers[i], 16 );

		int justify;
		if( i == DSP_REG_X0 || i == DSP_REG_X1 || i == DSP_REG_Y0 || i == DSP_REG_Y1
		    || i == DSP_REG_A0 || i == DSP_REG_B0 || i == DSP_REG_A1 || i == DSP_REG_B1 )
		{
			justify = 6;
		}
		else if( i == DSP_REG_A2 || i == DSP_REG_B2 )
		{
			justify = 2;
		}
		else
		{
			justify = 4;
		}
		str = str.rightJustified( justify, '0' ).toUpper();

		switch( i )
		{
		case DSP_REG_X0:
			ui->lineEditX0->setText( str );
			break;
		case DSP_REG_X1:
			ui->lineEditX1->setText( str );
			break;
		case DSP_REG_Y0:
			ui->lineEditY0->setText( str );
			break;
		case DSP_REG_Y1:
			ui->lineEditY1->setText( str );
			break;
		case DSP_REG_A0:
			ui->lineEditA0->setText( str );
			break;
		case DSP_REG_B0:
			ui->lineEditB0->setText( str );
			break;
		case DSP_REG_A2:
			ui->lineEditA2->setText( str );
			break;
		case DSP_REG_B2:
			ui->lineEditB2->setText( str );
			break;
		case DSP_REG_A1:
			ui->lineEditA1->setText( str );
			break;
		case DSP_REG_B1:
			ui->lineEditB1->setText( str );
			break;

		case DSP_REG_R0:
			ui->lineEditR0->setText( str );
			break;
		case DSP_REG_R1:
			ui->lineEditR1->setText( str );
			break;
		case DSP_REG_R2:
			ui->lineEditR2->setText( str );
			break;
		case DSP_REG_R3:
			ui->lineEditR3->setText( str );
			break;
		case DSP_REG_R4:
			ui->lineEditR4->setText( str );
			break;
		case DSP_REG_R5:
			ui->lineEditR5->setText( str );
			break;
		case DSP_REG_R6:
			ui->lineEditR6->setText( str );
			break;
		case DSP_REG_R7:
			ui->lineEditR7->setText( str );
			break;

		case DSP_REG_N0:
			ui->lineEditN0->setText( str );
			break;
		case DSP_REG_N1:
			ui->lineEditN1->setText( str );
			break;
		case DSP_REG_N2:
			ui->lineEditN2->setText( str );
			break;
		case DSP_REG_N3:
			ui->lineEditN3->setText( str );
			break;
		case DSP_REG_N4:
			ui->lineEditN4->setText( str );
			break;
		case DSP_REG_N5:
			ui->lineEditN5->setText( str );
			break;
		case DSP_REG_N6:
			ui->lineEditN6->setText( str );
			break;
		case DSP_REG_N7:
			ui->lineEditN7->setText( str );
			break;

		case DSP_REG_M0:
			ui->lineEditM0->setText( str );
			break;
		case DSP_REG_M1:
			ui->lineEditM1->setText( str );
			break;
		case DSP_REG_M2:
			ui->lineEditM2->setText( str );
			break;
		case DSP_REG_M3:
			ui->lineEditM3->setText( str );
			break;
		case DSP_REG_M4:
			ui->lineEditM4->setText( str );
			break;
		case DSP_REG_M5:
			ui->lineEditM5->setText( str );
			break;
		case DSP_REG_M6:
			ui->lineEditM6->setText( str );
			break;
		case DSP_REG_M7:
			ui->lineEditM7->setText( str );
			break;

		case DSP_REG_SR:
			ui->lineEditSR->setText( str );
			break;

		}
	}

	QString str = QString::number( pCore->pc, 16 );
	str = str.rightJustified( 4, '0' ).toUpper();
	ui->lineEditPC->setText( str );

	QTableWidgetItem* pItemPc = itemAtPc( pCore->pc );
	ui->tableWidgetCode->scrollToItem( pItemPc, QTableWidget::PositionAtTop );
	ui->tableWidgetCode->setCurrentItem( pItemPc );

	if( ui->radioButtonP->isChecked() )
	{
		updateP();
	}
	else if( ui->radioButtonX->isChecked() )
	{
		updateX();
	}
	else if( ui->radioButtonY->isChecked() )
	{
		updateY();
	}
	else if( ui->radioButtonL->isChecked() )
	{
		updateL();
	}
}

void Debugger::closeEvent( QCloseEvent* event )
{
	emit closed();

	QMainWindow::closeEvent( event );
}

void Debugger::updateP()
{
	ui->tableWidgetMemory->setRowCount( 0 );
	for( Uint16 addr = 0; addr < 0x1000 / 2; addr += 2 )
	{
		Uint32 word1;
		Uint32 word2;
		if( addr < 0x0200 )
		{
			word1 = m_pCurrentDspCore->ramint[DSP_SPACE_P][addr];
			word2 = m_pCurrentDspCore->ramint[DSP_SPACE_P][addr+1];
		}
		else
		{
			word1 = m_pCurrentDspCore->ramext[addr];
			word2 = m_pCurrentDspCore->ramext[addr+1];
		}

		int row = ui->tableWidgetMemory->rowCount();
		ui->tableWidgetMemory->setRowCount( row + 1 );

		ui->tableWidgetMemory->setItem( row, 0, new QTableWidgetItem( QString::number( addr, 16 ).rightJustified( 4, '0' ).toUpper().append( ':' ) ) );
		ui->tableWidgetMemory->setItem( row, 1, new QTableWidgetItem( QString::number( word1, 16 ).rightJustified( 6, '0' ).toUpper() ) );
		ui->tableWidgetMemory->setItem( row, 2, new QTableWidgetItem( QString::number( word2, 16 ).rightJustified( 6, '0' ).toUpper() ) );
	}

	ui->tableWidgetMemory->resizeRowsToContents();

	// strange workaround...
	ui->tableWidgetMemory->setVisible( false );
	ui->tableWidgetMemory->resizeColumnsToContents();
	ui->tableWidgetMemory->setVisible( true );
}

void Debugger::updateX()
{
	ui->tableWidgetMemory->setRowCount( 0 );
	for( Uint16 addr = 0x0000; addr < 0x4000 / 2; addr += 2 )
	{
		Uint32 word1;
		Uint32 word2;
		if( addr < 0x0100 )
		{
			word1 = m_pCurrentDspCore->ramint[DSP_SPACE_X][addr];
			word2 = m_pCurrentDspCore->ramint[DSP_SPACE_X][addr+1];
		}
		else
		{
			word1 = m_pCurrentDspCore->ramext[addr+0x4000];
			word2 = m_pCurrentDspCore->ramext[addr+0x4000+1];
		}

		int row = ui->tableWidgetMemory->rowCount();
		ui->tableWidgetMemory->setRowCount( row + 1 );

		ui->tableWidgetMemory->setItem( row, 0, new QTableWidgetItem( QString::number( addr, 16 ).rightJustified( 4, '0' ).toUpper().append( ':' ) ) );
		ui->tableWidgetMemory->setItem( row, 1, new QTableWidgetItem( QString::number( word1, 16 ).rightJustified( 6, '0' ).toUpper() ) );
		ui->tableWidgetMemory->setItem( row, 2, new QTableWidgetItem( QString::number( word2, 16 ).rightJustified( 6, '0' ).toUpper() ) );
	}

	ui->tableWidgetMemory->resizeRowsToContents();

	// strange workaround...
	ui->tableWidgetMemory->setVisible( false );
	ui->tableWidgetMemory->resizeColumnsToContents();
	ui->tableWidgetMemory->setVisible( true );
}

void Debugger::updateY()
{
	ui->tableWidgetMemory->setRowCount( 0 );
	for( Uint16 addr = 0x0000; addr < 0x4000 / 2; addr += 2 )
	{
		Uint32 word1;
		Uint32 word2;
		if( addr < 0x0100 )
		{
			word1 = m_pCurrentDspCore->ramint[DSP_SPACE_Y][addr];
			word2 = m_pCurrentDspCore->ramint[DSP_SPACE_Y][addr+1];
		}
		else
		{
			word1 = m_pCurrentDspCore->ramext[addr];
			word2 = m_pCurrentDspCore->ramext[addr+1];
		}

		int row = ui->tableWidgetMemory->rowCount();
		ui->tableWidgetMemory->setRowCount( row + 1 );

		ui->tableWidgetMemory->setItem( row, 0, new QTableWidgetItem( QString::number( addr, 16 ).rightJustified( 4, '0' ).toUpper().append( ':' ) ) );
		ui->tableWidgetMemory->setItem( row, 1, new QTableWidgetItem( QString::number( word1, 16 ).rightJustified( 6, '0' ).toUpper() ) );
		ui->tableWidgetMemory->setItem( row, 2, new QTableWidgetItem( QString::number( word2, 16 ).rightJustified( 6, '0' ).toUpper() ) );
	}

	ui->tableWidgetMemory->resizeRowsToContents();

	// strange workaround...
	ui->tableWidgetMemory->setVisible( false );
	ui->tableWidgetMemory->resizeColumnsToContents();
	ui->tableWidgetMemory->setVisible( true );
}

void Debugger::updateL()
{
	ui->tableWidgetMemory->setRowCount( 0 );
	for( Uint16 addr = 0x0000; addr < 0x4000; addr += 2 )
	{
		Uint32 word1;
		Uint32 word2;
		if( addr < 0x0100 )
		{
			word1 = m_pCurrentDspCore->ramint[DSP_SPACE_X][addr];
			word2 = m_pCurrentDspCore->ramint[DSP_SPACE_Y][addr];
		}
		else
		{
			word1 = m_pCurrentDspCore->ramext[addr+0x4000];
			word2 = m_pCurrentDspCore->ramext[addr];
		}

		int row = ui->tableWidgetMemory->rowCount();
		ui->tableWidgetMemory->setRowCount( row + 1 );

		ui->tableWidgetMemory->setItem( row, 0, new QTableWidgetItem( QString::number( addr, 16 ).rightJustified( 4, '0' ).toUpper().append( ':' ) ) );
		ui->tableWidgetMemory->setItem( row, 1, new QTableWidgetItem( QString::number( word1, 16 ).rightJustified( 6, '0' ).toUpper() ) );
		ui->tableWidgetMemory->setItem( row, 2, new QTableWidgetItem( QString::number( word2, 16 ).rightJustified( 6, '0' ).toUpper() ) );
	}

	ui->tableWidgetMemory->resizeRowsToContents();

	// strange workaround...
	ui->tableWidgetMemory->setVisible( false );
	ui->tableWidgetMemory->resizeColumnsToContents();
	ui->tableWidgetMemory->setVisible( true );
}

QTableWidgetItem* Debugger::itemAtPc( Uint16 pc ) const
{
	for( int i = 0; i < ui->tableWidgetCode->rowCount(); ++i )
	{
		QTableWidgetItem* pItem = ui->tableWidgetCode->item( i, 0 );
		if( pItem->text().toInt( 0, 16 ) == pc )
		{
			return pItem;
		}
	}

	return 0;
}

#define DEFINE_SET_REGISTER( reg )		\
	void Debugger::setRegister##reg()	\
	{					\
		m_pCurrentDspCore->registers[DSP_REG_##reg] = ui->lineEdit##reg->text().toInt( 0, 16 ); \
	}

DEFINE_SET_REGISTER( X0 )
DEFINE_SET_REGISTER( X1 )
DEFINE_SET_REGISTER( Y0 )
DEFINE_SET_REGISTER( Y1 )
DEFINE_SET_REGISTER( A0 )
DEFINE_SET_REGISTER( A1 )
DEFINE_SET_REGISTER( A2 )
DEFINE_SET_REGISTER( B0 )
DEFINE_SET_REGISTER( B1 )
DEFINE_SET_REGISTER( B2 )
DEFINE_SET_REGISTER( R0 )
DEFINE_SET_REGISTER( R1 )
DEFINE_SET_REGISTER( R2 )
DEFINE_SET_REGISTER( R3 )
DEFINE_SET_REGISTER( R4 )
DEFINE_SET_REGISTER( R5 )
DEFINE_SET_REGISTER( R6 )
DEFINE_SET_REGISTER( R7 )
DEFINE_SET_REGISTER( N0 )
DEFINE_SET_REGISTER( N1 )
DEFINE_SET_REGISTER( N2 )
DEFINE_SET_REGISTER( N3 )
DEFINE_SET_REGISTER( N4 )
DEFINE_SET_REGISTER( N5 )
DEFINE_SET_REGISTER( N6 )
DEFINE_SET_REGISTER( N7 )
DEFINE_SET_REGISTER( M0 )
DEFINE_SET_REGISTER( M1 )
DEFINE_SET_REGISTER( M2 )
DEFINE_SET_REGISTER( M3 )
DEFINE_SET_REGISTER( M4 )
DEFINE_SET_REGISTER( M5 )
DEFINE_SET_REGISTER( M6 )
DEFINE_SET_REGISTER( M7 )
DEFINE_SET_REGISTER( SR )

void Debugger::setRegisterPC()
{
	m_pCurrentDspCore->pc = ui->lineEditPC->text().toInt( 0, 16 );

	QTableWidgetItem* pItemPc = itemAtPc( m_pCurrentDspCore->pc );
	ui->tableWidgetCode->scrollToItem( pItemPc, QTableWidget::PositionAtTop );
	ui->tableWidgetCode->setCurrentItem( pItemPc );
}
