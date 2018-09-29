#ifndef NOTIFICATIONBAR_H
#define NOTIFICATIONBAR_H

#include <QFrame>

namespace Ui
{
class NotificationBar;
}

class NotificationBar : public QFrame
{
    Q_OBJECT

public:
    explicit NotificationBar(QWidget* parent = nullptr);
    ~NotificationBar();

    void setNotificationText(QString text);
    void setLink(QString link);

public slots:
    void openLinkInBrowser();

private:
    Ui::NotificationBar* ui_;
    QString              link_;
};

#endif // NOTIFICATIONBAR_H
