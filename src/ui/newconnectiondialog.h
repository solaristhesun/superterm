#ifndef NEWCONNECTIONDIALOG_H
#define NEWCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class NewConnectionDialog;
}

class NewConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnectionDialog(QWidget *parent = 0);
    ~NewConnectionDialog();

private:
    Ui::NewConnectionDialog *ui;
};

#endif // NEWCONNECTIONDIALOG_H
