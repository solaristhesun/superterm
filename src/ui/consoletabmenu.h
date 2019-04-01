#ifndef CONSOLETABMENU_H
#define CONSOLETABMENU_H

#include <QMenu>

namespace Ui
{
class ConsoleTab;
}

class ConsoleTabMenu: public QMenu
{
public:
    ConsoleTabMenu(Ui::ConsoleTab* ui);
private:
    QMenu* windowsMenu_;
};

#endif // CONSOLETABMENU_H
