/*
 * This file is part of superterm.
 *
 * Copyright (c) 2014-2018 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONNECTIONBAR_H
#define CONNECTIONBAR_H

#include <QFrame>

namespace Ui
{
class ConnectionBar;
}

class Session;
class PortEnumerator;

class ConnectionBar : public QFrame
{
    Q_OBJECT
public:
    ~ConnectionBar() override;

    void    loadFromSession(Session* session);
    void    onConnected();
    void    onDisconnected();
    QString getDeviceName() const;
    QString getDeviceDesc() const;
    QString getBaudRate() const;
    QString getDataBits() const;
    QString getParity() const;
    QString getStopBits() const;
    QString getFlowControl() const;
    void    setDeviceName(const QString& deviceName);
    void    setBaudRate(const QString& baudRate);
    void    setDataBits(const QString& dataBits);
    void    setParity(const QString& parity);
    void    setStopBits(const QString& stopBits);
    void    setFlowControl(const QString& flowControl);
    void    setPortEnumerator(PortEnumerator* pe);
    explicit ConnectionBar(QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* event) override;

public slots:
    void onMoreClicked();
    void onComboChanged();

signals:
    void connectClicked();
    void saveClicked();
    void configurationChanged(const QString& config);

private:
    Ui::ConnectionBar* ui_;

    void fillComboBoxes();
};

#endif // CONNECTIONBAR_H
