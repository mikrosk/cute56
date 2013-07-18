#ifndef MODULEDIALOG_H
#define MODULEDIALOG_H

#include <QDialog>

namespace Ui {
    class ModuleDialog;
}

class ModuleDialog : public QDialog
{
    Q_OBJECT

public:
    ModuleDialog(const QString& title, QWidget *parent = 0);
    QString path() const;

    ~ModuleDialog();

private slots:
    void openFileDialog();
    void storePath();

private:
    Ui::ModuleDialog *ui;
};

#endif // MODULEDIALOG_H
