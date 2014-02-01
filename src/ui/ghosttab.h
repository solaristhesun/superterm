#ifndef GHOSTTAB_H
#define GHOSTTAB_H

#include <QWidget>

class ConsoleTabBar;

class GhostTab : public QWidget
{
    Q_OBJECT
public:
    explicit GhostTab(ConsoleTabBar *parent, const QPoint& pos);

    // Offset between the position of the cursor on the tab bar and the upper
    // left position of the tab bar's window.
    const QPoint offset() { return m_offset; }

    // Index of the original tab in the tab bar
    const int index() { return m_index; }

    // Move the widget into the given position taking in account the current
    // offset() value
    void moveWithOffset(const QPoint&);

    // Return True if the difference between the position of the original
    // widget and given point is greater than the
    // QApplication::startDragDistance() value
    bool dragStarted(const QPoint&);
signals:

public slots:
private:
    QPoint m_offset;
    int m_index;
    QPoint m_originPos;
};

#endif // GHOSTTAB_H
