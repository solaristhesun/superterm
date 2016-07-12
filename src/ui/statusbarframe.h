#ifndef STATUSBARFRAME_H
#define STATUSBARFRAME_H

#include <QFrame>

namespace Ui {
class CStatusBarFrame;
}

class CStatusBarFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CStatusBarFrame(QWidget *parent = 0);
    ~CStatusBarFrame();

    void showMessage(const QString & message, const int timeout = 0);
    void showProgressMessage(const QString& message);
    void hideProgressMessage();

signals:
    void cancelReconnection();

private:
    Ui::CStatusBarFrame *m_ui;

    void setStyleProperty(const char* name, const QVariant& variant);
};

#endif // STATUSBARFRAME_H
