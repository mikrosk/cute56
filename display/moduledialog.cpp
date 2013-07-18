#include "moduledialog.h"
#include "ui_moduledialog.h"

#include <QFileDialog>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

ModuleDialog::ModuleDialog(const QString& title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleDialog)
{
    ui->setupUi(this);

    setWindowTitle( title );

    QSettings settings;
    ui->lineEditPath->setText( settings.value( windowTitle() + "/path" ).toString() );

    connect( ui->pushButtonBrowse, SIGNAL(clicked()), this, SLOT(openFileDialog()) );
    connect( ui->buttonBox, SIGNAL(accepted()), this, SLOT(storePath()) );
}

ModuleDialog::~ModuleDialog()
{
    delete ui;
}

QString ModuleDialog::path() const
{
    return ui->lineEditPath->text();
}

void ModuleDialog::openFileDialog()
{
    QString dir = QFileInfo( path() ).dir().path();

    QString path = QFileDialog::getOpenFileName( this, windowTitle(), dir );
    if( !path.isEmpty() )
    {
        ui->lineEditPath->setText( path );
    }
}

void ModuleDialog::storePath()
{
    QSettings settings;
    settings.setValue( windowTitle() + "/path", path() );
}
