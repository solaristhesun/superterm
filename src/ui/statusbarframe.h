#ifndef STATUSBARFRAME_H
#define STATUSBARFRAME_H

#include <QFrame>

namespace Ui {
class StatusBarFrame;
}

class StatusBarFrame : public QFrame
{
    Q_OBJECT

public:
    explicit StatusBarFrame(QWidget *parent = 0);
    ~StatusBarFrame();

    void showMessage(const QString & message, const int timeout = 0);

private:
    Ui::StatusBarFrame *m_ui;

    void setStyleProperty(const char* name, const QVariant& variant);
};

#endif // STATUSBARFRAME_H
