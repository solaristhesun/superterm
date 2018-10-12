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

#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QProcess>
#include <QSerialPort>
#include <QTextEdit>
#include <QWidget>

namespace Ui
{
class ConsoleTab;
}

class QSerialPort;
class ConsoleLineBuffer;
class PortEnumerator;
class QMenu;
class PortEndpoint;
class Session;
class MainWindow;
class Message;
class HighlightingsModel;

class ConsoleTab : public QWidget
{
    Q_OBJECT

public:
    ConsoleTab(PortEnumerator* pe, Session* session = nullptr);
    ~ConsoleTab();

    void hideButton(int index);

    void    setConsoleFont(const QFont& font);
    void    setColor(const QColor& backGroundColor, const QColor& foreGroundColor);
    QString getLabel() const;
    void    setLabel(const QString& label);
    QSize   getViewPortSize() const;
    QSize   getCharWidth() const;
    void    disconnectEndpoint();

    bool eventFilter(QObject* obj, QEvent* event);

public slots:
    void onConnectClicked();
    void onKeyPressed(QKeyEvent* e);
    void showContextMenu(const QPoint& pt);
    void showColorDialog();
    void showFontDialog();
    void showFontColorDialog();
    void showError(QSerialPort::SerialPortError error);
    void updateHighlightings();
    void showAboutDialog();
    void toggleFullScreen();
    void toggleAutoScroll();
    void showLogToFileFrame();
    void startLogging();
    void stopLogging();
    void showSaveDialog();
    void onEndpointData(const Message& message);
    void onEndpointConnected();
    void onEndpointDisconnected(int returnCode);
    void onReconnectionSignal(const Message& message);
    void onReconnectionCancel();
    void onConfigurationChanged(const QString& config);
    void showRenameTabDialog();
    void onRenameTab();
    void clearTab();
    void toggleTimeStamps();

signals:
    void labelChanged(const QString&);

private:
    Ui::ConsoleTab*     ui_;
    MainWindow*         mainWindow_;
    HighlightingsModel* highlightingsModel_;
    ConsoleLineBuffer*  lineBuffer_;
    QString             tabLabel_;
    PortEndpoint*       portEndpoint_;
    Session*            session_;
    QMenu*              contextMenu_;
    int                 lastTabIndex_;

    void fillComboBoxes();
    void createContextMenu();
    void escapeSpecialChars(QByteArray& data);
    void destroySession();
};

#endif // CONSOLETAB_H
