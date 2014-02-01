#include <QApplication>

#include "ghosttab.h"
#include "consoletabbar.h"

GhostTab::GhostTab(ConsoleTabBar *parent, const QPoint& pos) :
    QWidget(parent)
{
    QWidget *wnd = parent->window();
    QPalette palette;

    #if QT_VERSION >= 0x050000
    palette.setBrush(this->backgroundRole(), QBrush(wnd->grab()));
    #else
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap::grabWidget(wnd)));
    #endif

    this->setPalette(palette);
    this->setGeometry(wnd->geometry());
    this->setWindowOpacity(0.5);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);

    m_offset = parent->mapToGlobal(pos) - wnd->pos();
    m_index = parent->tabAt(pos);
    m_originPos = parent->mapToGlobal(pos);
}


void GhostTab::moveWithOffset(const QPoint& pos)
{
    QWidget::move(pos - m_offset);
}


bool GhostTab::dragStarted(const QPoint &pos)
{
    return (pos - m_originPos).manhattanLength() >= QApplication::startDragDistance();
}
