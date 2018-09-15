#include <QDesktopServices>
#include <QUrl>

#include "notificationbar.h"
#include "ui_notificationbar.h"

CNotificationBar::CNotificationBar(QWidget *parent)
    : QFrame(parent)
    , ui_(new Ui::NotificationBar)
{
    ui_->setupUi(this);
}

CNotificationBar::~CNotificationBar()
{
    delete ui_;
}

void CNotificationBar::setNotificationText(QString text)
{
    ui_->label->setText(text);
}

void CNotificationBar::setLink(QString link)
{
    link_ = link;
}

void CNotificationBar::openLinkInBrowser()
{
    QDesktopServices::openUrl(QUrl(link_));
}

// EOF <stefan@scheler.com>
