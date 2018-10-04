#ifndef PORTITEMVIEW_H
#define PORTITEMVIEW_H

#include <QFrame>

namespace Ui
{
class PortItemView;
}

class PortItemView : public QFrame
{
    Q_OBJECT

public:
    explicit PortItemView(QWidget* parent = nullptr);
    ~PortItemView();

    void setPortName(const QString& portName);
    void setDescription(const QString& description);
    void setBusyFlag(const bool bBusy);

private:
    Ui::PortItemView* ui_;
};

#endif // PORTITEMVIEW_H
