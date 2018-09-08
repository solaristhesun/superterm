#ifndef USAGETRACKER_H
#define USAGETRACKER_H

#include <QObject>

class QNetworkAccessManager;

class UsageTracker: public QObject
{
    Q_OBJECT
public:
    UsageTracker();

public slots:
    void trackUsage();

private:
    QNetworkAccessManager* manager_;

private:
    QByteArray getUserAgent() const;
};

#endif // USAGETRACKER_H
