#include "ui_consoletab.h"
#include "consoletabmenu.h"

ConsoleTabMenu::ConsoleTabMenu(Ui::ConsoleTab* ui)
    : windowsMenu_(new QMenu(this))
{
    windowsMenu_->setTitle("Align windows");
    windowsMenu_->addAction(ui->actionAlignVertical);
    windowsMenu_->addAction(ui->actionAlignHorizontal);

    this->addAction(ui->actionConnection);
    this->addAction(ui->actionLogging);
    this->addAction(ui->actionToggleTimeStamps);
    this->addAction(ui->actionHighlight);
    this->addSeparator();
    this->addAction(ui->actionToggleAutoscroll);
    this->addSeparator();
    this->addAction(ui->actionChangeFont);
    this->addAction(ui->actionChangeFontColor);
    this->addAction(ui->actionChangeColor);
    this->addSeparator();
    this->addAction(ui->actionClear);
    this->addSeparator();
    this->addMenu(windowsMenu_);
    this->addSeparator();
    this->addAction(ui->actionFullscreen);
    this->addSeparator();
    this->addAction(ui->actionAbout);
    ui->actionToggleAutoscroll->setChecked(true);
}
