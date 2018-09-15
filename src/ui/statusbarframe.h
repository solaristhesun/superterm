#ifndef STATUSBARFRAME_H
#define STATUSBARFRAME_H

#include <QFrame>

namespace Ui
{
class CStatusBarFrame;
}

class CStatusBarFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CStatusBarFrame(QWidget* parent = nullptr);
    ~CStatusBarFrame();

    void showMessage(const QString& message, const int timeout = 0);
    void showProgressMessage(const QString& message);
    void hideProgressMessage();

public slots:
    void hideOnTimeout();

signals:
    void cancelReconnection();

private:
    Ui::CStatusBarFrame* m_ui;

    void setStyleProperty(const char* name, const QVariant& variant);
    void setupWaitingSpinner();
};

#endif // STATUSBARFRAME_H
