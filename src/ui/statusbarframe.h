#ifndef STATUSBARFRAME_H
#define STATUSBARFRAME_H

#include <QFrame>

namespace Ui
{
class StatusBarFrame;
}

class StatusBarFrame : public QFrame
{
    Q_OBJECT

public:
    explicit StatusBarFrame(QWidget* parent = nullptr);
    ~StatusBarFrame();

    void showMessage(const QString& message, const int timeout = 0);
    void showProgressMessage(const QString& message);
    void hideProgressMessage();

public slots:
    void hideOnTimeout();

signals:
    void cancelReconnection();

private:
    Ui::StatusBarFrame* ui_;

    void setStyleProperty(const char* name, const QVariant& variant);
    void setupWaitingSpinner();
};

#endif // STATUSBARFRAME_H
