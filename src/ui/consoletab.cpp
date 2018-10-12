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

#include <QThread>
#include <QColorDialog>
#include <QSerialPort>
#include <QDebug>
#include <iostream>
#include <QMenu>
#include <QMouseEvent>
#include <cstdio>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFontDialog>
#include <QFileDialog>
#include <QSettings>
#include <QXmlStreamWriter>
#include <QListWidgetItem>
#include <QMetaProperty>
#include <QDateTime>
#include <QDesktopWidget>
#include <QStringListModel>
#include <QItemDelegate>
#include <QPainter>
#include <QColor>
#if defined(Q_OS_WIN)
#include <windows.h>
#endif

#include "ui_consoletab.h"
#include "misc/globals.h"
#include "ui/consoletab.h"
#include "ui/mainwindow.h"
#include "ui/consoletabwidget.h"
#include "ui/highlightingsframe.h"
#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"
#include "serial/portendpoint.h"
#include "session/session.h"
#include "ipc/message.h"
#include "models/consolelinebuffer.h"
#include "models/highlightingsmodel.h"

Q_DECLARE_METATYPE(QSerialPort::DataBits)
Q_DECLARE_METATYPE(QSerialPort::StopBits)
Q_DECLARE_METATYPE(QSerialPort::Parity)
Q_DECLARE_METATYPE(QSerialPort::FlowControl)

ConsoleTab::ConsoleTab(PortEnumerator* pe, Session* session)
    : QWidget(nullptr)
    , ui_(new Ui::ConsoleTab)
    , mainWindow_(nullptr)
    , highlightingsModel_(new HighlightingsModel)
    , lineBuffer_(new ConsoleLineBuffer(highlightingsModel_))
    , tabLabel_(tr("New tab"))
    , portEndpoint_(new PortEndpoint(this))
    , session_(session)
    , contextMenu_(nullptr)
    , lastTabIndex_(0)
{
    qDebug() << "CConsoleTab::CConsoleTab()";

    ui_->setupUi(this);
    ui_->highlightingsFrame->setModel(highlightingsModel_);

    // load font from settings
    QSettings settings;

    QFont  consoleFont;
    QColor foreGroundColor = QColor(settings.value("foreground").toString());
    QColor backGroundColor = QColor(settings.value("background").toString());
    qDebug() << consoleFont;
    consoleFont.fromString(settings.value("font").toString());
    setConsoleFont(consoleFont);
    setColor(foreGroundColor, backGroundColor);

    createContextMenu();

    connect(portEndpoint_, &PortEndpoint::readyRead, this, &ConsoleTab::onEndpointData);
    connect(portEndpoint_, &PortEndpoint::connected, this, &ConsoleTab::onEndpointConnected);
    connect(portEndpoint_, &PortEndpoint::disconnected, this, &ConsoleTab::onEndpointDisconnected);
    connect(portEndpoint_, &PortEndpoint::readyRead, this, &ConsoleTab::onReconnectionSignal);
    connect(ui_->statusBar, &StatusBarFrame::cancelReconnection, this, &ConsoleTab::onReconnectionCancel);
    connect(ui_->renameTabFrame, &RenameTabFrame::applyPressed, this, &ConsoleTab::onRenameTab);
    connect(highlightingsModel_, &HighlightingsModel::highlightingChanged, this, &ConsoleTab::updateHighlightings);

    ui_->connectionBar->setPortEnumerator(pe);

    if (session)
    {
        portEndpoint_->connectEndpoint(session);

        ui_->connectionBar->loadFromSession(session);

        QList<Highlighting> highlightings;

        for (const QVariant& h : session->getHighlights())
        {
            highlightings.append(h.value<Highlighting>());
        }
        highlightingsModel_->setHighlightings(highlightings);

        if (session->getUseTimeStamps())
        {
            ui_->actionToggleTimeStamps->activate(QAction::Trigger);
        }

        if (!session->getLogFileName().isEmpty())
        {
            ui_->logPanel->setLogFileName(session->getLogFileName());
            startLogging();
        }
    }

    ui_->consoleView->setModel(lineBuffer_);
    ui_->consoleView->installEventFilter(this);
}

ConsoleTab::~ConsoleTab()
{
    qDebug() << "CConsoleTab::~CConsoleTab()";
    delete ui_;
    delete contextMenu_;
    delete lineBuffer_;
    delete portEndpoint_;
}

QString ConsoleTab::getLabel() const
{
    return tabLabel_;
}

void ConsoleTab::setLabel(const QString& label)
{
    tabLabel_ = label;
    emit labelChanged(label);
}

void ConsoleTab::toggleFullScreen()
{
    if (!QWidget::isFullScreen())
    {
        mainWindow_ = static_cast<MainWindow*>(QApplication::activeWindow());
        mainWindow_->hide();
        QWidget::setParent(nullptr);

        int screenNumber = QApplication::desktop()->screenNumber(mainWindow_);
        QWidget::setGeometry(QApplication::desktop()->screenGeometry(screenNumber));

        QWidget::showFullScreen();
        ui_->actionFullscreen->setChecked(true);
    }
    else
    {
        if (mainWindow_)
        {
            mainWindow_->attachTab(this);
            QWidget::showNormal();
            ui_->consoleView->setFocus();
            ui_->actionFullscreen->setChecked(false);
            mainWindow_->show();
        }
    }
}

void ConsoleTab::clearTab()
{
    lineBuffer_->clear();
}

void ConsoleTab::createContextMenu()
{
    contextMenu_ = new QMenu(this);
    contextMenu_->addAction(ui_->actionConnection);
    contextMenu_->addAction(ui_->actionLogging);
    contextMenu_->addAction(ui_->actionToggleTimeStamps);
    contextMenu_->addAction(ui_->actionHighlight);
    contextMenu_->addSeparator();
    contextMenu_->addAction(ui_->actionToggleAutoscroll);
    contextMenu_->addSeparator();
    contextMenu_->addAction(ui_->actionChangeFont);
    contextMenu_->addAction(ui_->actionChangeFontColor);
    contextMenu_->addAction(ui_->actionChangeColor);
    contextMenu_->addSeparator();
    contextMenu_->addAction(ui_->actionClear);
    contextMenu_->addSeparator();
    contextMenu_->addAction(ui_->actionFullscreen);
    contextMenu_->addSeparator();
    contextMenu_->addAction(ui_->actionAbout);
    ui_->actionToggleAutoscroll->setChecked(true);
}

void ConsoleTab::showContextMenu(const QPoint& pt)
{
    contextMenu_->exec(ui_->consoleView->viewport()->mapToGlobal(pt));
}

void ConsoleTab::updateHighlightings()
{
    QList<Highlighting> highlightings = highlightingsModel_->highlightings();

    if (session_)
    {
        session_->setHighlights(SerializableObject::convertToQVariantList(highlightings));

        if (session_->isPortConnected())
        {
            session_->saveToFile();
        }
    }
}

void ConsoleTab::onConfigurationChanged(const QString& config)
{
    if (!config.endsWith(".xml"))
    {
        return;
    }

    qDebug() << "[slot] onConfigurationChanged" << config;

    QString fileName = QDir(QCoreApplication::applicationDirPath()).filePath(config);

    qDebug() << fileName;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader xml(&file);

    QList<Highlighting> highlightings;

    while (!xml.atEnd())
    {
        xml.readNext();

        if (xml.isStartElement())
        {
            QString token = xml.name().toString();

            if (token == "port")
            {
                ui_->connectionBar->setDeviceName(xml.readElementText());
            }
            else if (token == "speed")
            {
                ui_->connectionBar->setBaudRate(xml.readElementText());
            }
            else if (token == "databits")
            {
                ui_->connectionBar->setDataBits(xml.readElementText());
            }
            else if (token == "parity")
            {
                ui_->connectionBar->setParity(xml.readElementText());
            }
            else if (token == "stopbits")
            {
                ui_->connectionBar->setStopBits(xml.readElementText());
            }
            else if (token == "flowcontrol")
            {
                ui_->connectionBar->setFlowControl(xml.readElementText());
            }
            else if (token == "pattern")
            {
                QXmlStreamAttributes           attr = xml.attributes();
                Highlighting hi;
                hi.pattern = xml.readElementText();
                hi.color = QColor(attr.value("color").toString());
                highlightings.append(hi);
            }
        }
    }

    highlightingsModel_->setHighlightings(highlightings);
}

void ConsoleTab::showSaveDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save configuration"),
                                                    "",
                                                    tr("superterm configurations (*.xml)"));

    if (!fileName.endsWith(".xml"))
    {
        fileName += ".xml";
    }

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("superterm");

    xmlWriter.writeStartElement("configuration");
    xmlWriter.writeTextElement("port", ui_->connectionBar->getDeviceName());
    xmlWriter.writeTextElement("speed", ui_->connectionBar->getBaudRate());
    xmlWriter.writeTextElement("databits", ui_->connectionBar->getDataBits());
    xmlWriter.writeTextElement("parity", ui_->connectionBar->getParity());
    xmlWriter.writeTextElement("stopbits", ui_->connectionBar->getStopBits());
    xmlWriter.writeTextElement("flowcontrol", ui_->connectionBar->getFlowControl());
    xmlWriter.writeEndElement();


    QList<Highlighting> h = highlightingsModel_->highlightings();

    if (h.size() > 0)
    {
        xmlWriter.writeStartElement("highlighting");
        for (int i = 0; i < h.size(); ++i)
        {
            xmlWriter.writeStartElement("pattern");
            xmlWriter.writeAttribute("color", h[i].color.name());
            xmlWriter.writeCharacters(h [i].pattern);
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }

    QString logFileName = ui_->logPanel->getLogFileName();

    if (!logFileName.isEmpty())
    {
        xmlWriter.writeStartElement("logging");
        xmlWriter.writeTextElement("filename", logFileName);
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();

    file.close();
}

void ConsoleTab::showColorDialog()
{
    QSettings settings;

    QColor initialBackground(settings.value("background").toString());
    QColor initialForeground(settings.value("foreground").toString());
    QColor rgb = QColorDialog::getColor(initialBackground, this);

    if (rgb.isValid())
    {
        setColor(initialForeground, rgb);
        settings.setValue("background", rgb.name());
    }
}

void ConsoleTab::showFontDialog()
{
    bool  ok;
    QFont font = QFontDialog::getFont(&ok, ui_->consoleView->font(), this, QString(), QFontDialog::MonospacedFonts);
    if (ok)
    {
        QSettings settings;
        settings.setValue("font", font.toString());
        setConsoleFont(font);
    }
}

void ConsoleTab::showFontColorDialog()
{
    QSettings settings;

    QColor initialBackground(settings.value("background").toString());
    QColor initialForeground(settings.value("foreground").toString());
    QColor rgb = QColorDialog::getColor(initialForeground, this);

    if (rgb.isValid())
    {
        setColor(rgb, initialBackground);
        settings.setValue("foreground", rgb.name());
    }
}

void ConsoleTab::toggleAutoScroll()
{
    ui_->consoleView->setAutoScrollToBottom(!ui_->consoleView->autoScrollToBottom());
}

void ConsoleTab::setConsoleFont(const QFont& font)
{
    ui_->consoleView->setFont(font);
}

void ConsoleTab::setColor(const QColor& textColor, const QColor& backgroundColor)
{
    ui_->consoleView->setTextColor(textColor);
    ui_->consoleView->setBackgroundColor(backgroundColor);
}

void ConsoleTab::escapeSpecialChars(QByteArray& data)
{
    for (int p = data.size() - 1; p > 0; p--)
    {
        char c = data[p];
        if (!QChar::isPrint(c) && !QChar::isSpace(c))
        {
            data.replace(p, 1, QString("\\x%1").arg((char)c & 0xFF, 2, 16, QChar('0')).toUtf8());
        }
        if (c == '\r')
        {
            data.remove(p, 1);
        }
    }
}

void ConsoleTab::toggleTimeStamps()
{
    const bool bTimestampsEnabled = !ui_->consoleView->timestampsEnabled();

    ui_->consoleView->setTimestampsEnabled(bTimestampsEnabled);

    if (session_)
    {
        session_->setUseTimeStamps(bTimestampsEnabled);
        session_->saveToFile();
    }
}

void ConsoleTab::onEndpointData(const Message& message)
{
    if (message.isCmd(Message::DataCmd))
    {
        QByteArray data = message.getPayload();

        escapeSpecialChars(data);
        lineBuffer_->append(data);
    }
}

void ConsoleTab::onConnectClicked()
{
    if (!portEndpoint_->isConnected())
    {
        if (!session_)
        {
            session_ = new Session();
        }

        session_->setDeviceName(ui_->connectionBar->getDeviceName());
        session_->setTabLabel(ui_->connectionBar->getDeviceName());
        session_->setDeviceDesc(ui_->connectionBar->getDeviceDesc());
        session_->setBaudRate(ui_->connectionBar->getBaudRate().toUInt());
        session_->setDataBits(ui_->connectionBar->getDataBits().toInt());
        session_->setParity(Globals::ParityNameMap.key(ui_->connectionBar->getParity()));
        session_->setStopBits(Globals::StopBitsNameMap.key(ui_->connectionBar->getStopBits()));
        session_->setFlowControl(Globals::FlowControlNameMap.key(ui_->connectionBar->getFlowControl()));

        portEndpoint_->connectEndpoint(session_);
    }
    else
    {
        disconnectEndpoint();
    }
}

void ConsoleTab::disconnectEndpoint()
{
    if (portEndpoint_->isConnected())
    {
        portEndpoint_->disconnectEndpoint();
        destroySession();
    }
}

void ConsoleTab::destroySession()
{
    if (session_)
    {
        delete session_;
        session_ = nullptr;
    }
}

void ConsoleTab::onEndpointDisconnected(int returnCode)
{
    qDebug() << "[slot] onEndpointDisconnected" << returnCode;

    ui_->connectionBar->onDisconnected();
    ui_->consoleView->setFocus();

    switch (returnCode)
    {
        case 1:
            {
                ui_->statusBar->showMessage(tr("Error connecting to port %1.").arg(session_->getDeviceName()));
            }
            break;
        default:
            break;
    }

    destroySession();
}

void ConsoleTab::showError(QSerialPort::SerialPortError error)
{
    qDebug() << "ERROR: " << error;
    ui_->statusBar->showMessage("ERROR: " + QString::number(error) + " opening port " );
}

bool ConsoleTab::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() ==QEvent::KeyPress)
    {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);

        if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
        {
            toggleFullScreen();
        }
        else
        {
            onKeyPressed(e);
        }

        return true;
    }

    return QObject::eventFilter(obj, event);
}

void ConsoleTab::showAboutDialog()
{
    const QString contents = QString(
        "<p><font color=#000080><font size=6><b>%1</b></font> <font size=4>(revision %2)</font></font></p>"
        "<p align=left>Copyright &copy; 2015-2018 Stefan Scheler. %3</p>"
        "<p><a href=\"%4\">%5</a></p>"
        "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</p>")
                             .arg(Globals::ApplicationFullName, QString::number(Globals::ApplicationRevision), tr("All rights reserved."), Globals::ApplicationWebsite, tr("Visit superterm website"));

    QMessageBox::about(this, tr("About superterm"), contents);
}

void ConsoleTab::onRenameTab()
{
    QString tabLabel = ui_->renameTabFrame->tabLabel();

    setLabel(tabLabel);

    if (session_)
    {
        session_->setTabLabel(tabLabel);

        if (session_->isPortConnected())
        {
            session_->saveToFile();
        }
    }
}

void ConsoleTab::showRenameTabDialog()
{
    ui_->renameTabFrame->setTabLabel(getLabel());
    ui_->renameTabFrame->show();
}

void ConsoleTab::onKeyPressed(QKeyEvent* e)
{
    QString key;

    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
    {
        key = "\r\n";
    }
    else
    {
        key = e->text();
    }

    QByteArray b(key.toLatin1());

    if (portEndpoint_->isConnected())
    {
        portEndpoint_->writeData(b);
    }
#if 0
    lineBuffer_->append(key);
#endif
#if 1
    if (key == 'Q')
    {
        for (int i = 1; i < 2; i++)
        {
            lineBuffer_->append(QByteArray("01234567890123456789012345678901234567890123456789012345678\r\n"));
            //QThread::msleep(1);
        }
    }
#endif
}

void ConsoleTab::startLogging()
{
    QString fileName = ui_->logPanel->getLogFileName();

    if (lineBuffer_->startLogging(fileName))
    {
        ui_->statusBar->showMessage(tr("Logging to %1 started.").arg(fileName), 3000);

        if (session_)
        {
            session_->setLogFileName(fileName);
            session_->saveToFile();
        }
    }
    else
    {
        ui_->statusBar->showMessage(tr("Logging to %1 failed.").arg(fileName));
    }
}

void ConsoleTab::stopLogging()
{
    lineBuffer_->stopLogging();

    if (session_)
    {
        session_->setLogFileName("");
        session_->saveToFile();
    }
}

void ConsoleTab::onEndpointConnected()
{
    qDebug() << "[slot] onEndpointConnected";

    const QString sDeviceName = session_->getDeviceName();

    ui_->connectionBar->onConnected();
    ui_->consoleView->setFocus();

    setLabel(session_->getTabLabel());

    ui_->statusBar->showMessage(tr("Successfully connected to %1.").arg(sDeviceName), 3000);

    if (session_)
    {
        session_->saveToFile();
    }
}

void ConsoleTab::onReconnectionSignal(const Message& message)
{
    const QString sDeviceName = session_->getDeviceName();

    if (message.isCmd(Message::SigCmd))
    {
        Message::Signal sig = message.getSignal();
        qDebug() << "[slot] onReconnectionSignal(" << sig << ")";

        if (sig == Message::IsConSig)
        {
            if (session_)
            {
                session_->setPortConnected(false);
            }

            ui_->statusBar->showProgressMessage(tr("Trying to reconnect to %1...").arg(sDeviceName));
        }
        else if (sig == Message::DoneConSig)
        {
            if (session_)
            {
                session_->setPortConnected(true);
            }

            ui_->statusBar->hideProgressMessage();
            ui_->statusBar->showMessage(tr("Successfully reconnected to %1.").arg(sDeviceName), 3000);
        }
    }
}

void ConsoleTab::showLogToFileFrame()
{
    if (!lineBuffer_->isLogging())
    {
        QString fileNameProposal = QCoreApplication::applicationDirPath();

        if (session_)
        {
            fileNameProposal += "/superterm_" + session_->getTabLabel() + ".log";
        }

        ui_->logPanel->setLogFileName(fileNameProposal);
    }

    ui_->logPanel->show();
}

void ConsoleTab::onReconnectionCancel()
{
    portEndpoint_->writeSignal(Message::CancelConSig);
}

QSize ConsoleTab::getViewPortSize() const
{
    return ui_->consoleView->viewport()->size();
}

QSize ConsoleTab::getCharWidth() const
{
    return ui_->consoleView->getCharWidth();
}

// EOF <stefan@scheler.com>
