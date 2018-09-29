#include <QDesktopServices>
#include <QUrl>

#include "notificationbar.h"
#include "ui_notificationbar.h"

NotificationBar::NotificationBar(QWidget *parent)
    : QFrame(parent)
    , ui_(new Ui::NotificationBar)
{
    ui_->setupUi(this);
}

NotificationBar::~NotificationBar()
{
    delete ui_;
}

void NotificationBar::setNotificationText(QString text)
{
    ui_->label->setText(text);
}

void NotificationBar::setLink(QString link)
{
    link_ = link;
}

void NotificationBar::openLinkInBrowser()
{
    QDesktopServices::openUrl(QUrl(link_));
}

// EOF <stefan@scheler.com>
