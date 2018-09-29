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
#include <QFile>
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

quint32 ConsoleTab::m_u32counter = 1;

Q_DECLARE_METATYPE(QSerialPort::DataBits)
Q_DECLARE_METATYPE(QSerialPort::StopBits)
Q_DECLARE_METATYPE(QSerialPort::Parity)
Q_DECLARE_METATYPE(QSerialPort::FlowControl)

ConsoleTab::ConsoleTab(PortEnumerator* pe, Session* session)
    : QWidget(nullptr)
    , m_ui(new Ui::ConsoleTab)
    , mMainWindow(nullptr)
    , lineBuffer_(new ConsoleLineBuffer)
    , mTabLabel(tr("New tab"))
    , m_portEndpoint(new PortEndpoint(this))
    , m_session(session)
    , m_logFile(nullptr)
    , m_contextMenu(nullptr)
    , m_lastTabIndex(0)
{
    qDebug() << "CConsoleTab::CConsoleTab()";

    m_ui->setupUi(this);

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

    connect(m_portEndpoint, &PortEndpoint::readyRead, this, &ConsoleTab::onEndpointData);
    connect(m_portEndpoint, &PortEndpoint::connected, this, &ConsoleTab::onEndpointConnected);
    connect(m_portEndpoint, &PortEndpoint::disconnected, this, &ConsoleTab::onEndpointDisconnected);
    connect(m_portEndpoint, &PortEndpoint::readyRead, this, &ConsoleTab::onReconnectionSignal);
    connect(m_ui->statusBar, &StatusBarFrame::cancelReconnection, this, &ConsoleTab::onReconnectionCancel);
    connect(m_ui->renameTabFrame, &RenameTabFrame::applyPressed, this, &ConsoleTab::onRenameTab);

    m_ui->connectionBar->setPortEnumerator(pe);

    if (session)
    {
        m_portEndpoint->connectEndpoint(session);

        m_ui->connectionBar->loadFromSession(session);

        QList<Highlighting> highlightings;

        for (const QVariant& h : session->getHighlights())
        {
            Highlighting hi = h.value<Highlighting>();
            QPixmap                        pixmap(10, 10);
            pixmap.fill(hi.color);
            QIcon            icon(pixmap);
            QListWidgetItem* item = new QListWidgetItem(icon, hi.pattern);
            item->setData(Qt::UserRole, QVariant(hi.color));
            m_ui->highlightingsFrame->addHighlighting(item);
            highlightings.append(hi);
        }

        lineBuffer_->setHighlightings(highlightings);

        if (session->getUseTimeStamps())
        {
            m_ui->actionToggleTimeStamps->activate(QAction::Trigger);
        }
    }

    m_ui->consoleView->setModel(lineBuffer_);

    m_ui->consoleView->installEventFilter(this);
}

ConsoleTab::~ConsoleTab()
{
    qDebug() << "CConsoleTab::~CConsoleTab()";
    delete m_ui;
    delete m_contextMenu;
    delete lineBuffer_;
}

QString ConsoleTab::getLabel() const
{
    return mTabLabel;
}

void ConsoleTab::setLabel(const QString& label)
{
    qDebug() << "setLabel(" << label << ")";
    mTabLabel = label;

    emit labelChanged(label);
}

void ConsoleTab::toggleFullScreen()
{
    if (!QWidget::isFullScreen())
    {
        mMainWindow = static_cast<MainWindow*>(QApplication::activeWindow());
        mMainWindow->hide();
        QWidget::setParent(nullptr);

        int screenNumber = QApplication::desktop()->screenNumber(mMainWindow);
        QWidget::setGeometry(QApplication::desktop()->screenGeometry(screenNumber));

        QWidget::showFullScreen();
        m_ui->actionFullscreen->setChecked(true);
    }
    else
    {
        if (mMainWindow)
        {
            mMainWindow->attachTab(this);
            QWidget::showNormal();
            m_ui->consoleView->setFocus();
            m_ui->actionFullscreen->setChecked(false);
            mMainWindow->show();
        }
    }
}

void ConsoleTab::clearTab()
{
    lineBuffer_->clear();
}

void ConsoleTab::createContextMenu()
{
    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction(m_ui->actionConnection);
    m_contextMenu->addAction(m_ui->actionLogging);
    m_contextMenu->addAction(m_ui->actionToggleTimeStamps);
    m_contextMenu->addAction(m_ui->actionHighlight);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_ui->actionToggleAutoscroll);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_ui->actionChangeFont);
    m_contextMenu->addAction(m_ui->actionChangeFontColor);
    m_contextMenu->addAction(m_ui->actionChangeColor);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_ui->actionClear);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_ui->actionFullscreen);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_ui->actionAbout);
    m_ui->actionToggleAutoscroll->setChecked(true);
}

void ConsoleTab::showContextMenu(const QPoint& pt)
{
    m_contextMenu->exec(mapToGlobal(pt));
}

void ConsoleTab::updateHighlightings()
{
    QList<Highlighting> highlightings = m_ui->highlightingsFrame->getItems();
    lineBuffer_->setHighlightings(highlightings);

    if (m_session)
    {
        m_session->setHighlights(SerializableObject::convertToQVariantList(m_ui->highlightingsFrame->getItems()));

        if (m_session->isPortConnected())
        {
            m_session->saveToFile();
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
    m_ui->highlightingsFrame->clear();

    while (!xml.atEnd())
    {
        xml.readNext();

        if (xml.isStartElement())
        {
            QString token = xml.name().toString();

            if (token == "port")
            {
                m_ui->connectionBar->setDeviceName(xml.readElementText());
            }
            else if (token == "speed")
            {
                m_ui->connectionBar->setBaudRate(xml.readElementText());
            }
            else if (token == "databits")
            {
                m_ui->connectionBar->setDataBits(xml.readElementText());
            }
            else if (token == "parity")
            {
                m_ui->connectionBar->setParity(xml.readElementText());
            }
            else if (token == "stopbits")
            {
                m_ui->connectionBar->setStopBits(xml.readElementText());
            }
            else if (token == "flowcontrol")
            {
                m_ui->connectionBar->setFlowControl(xml.readElementText());
            }
            else if (token == "pattern")
            {
                QXmlStreamAttributes           attr = xml.attributes();
                Highlighting hi;
                hi.pattern = xml.readElementText();
                hi.color = QColor(attr.value("color").toString());
                highlightings.append(hi);

                QPixmap pixmap(10, 10);
                pixmap.fill(hi.color );
                QIcon            icon(pixmap);
                QListWidgetItem* item = new QListWidgetItem(icon, hi.pattern);
                item->setData(Qt::UserRole, QVariant(hi.color));
                m_ui->highlightingsFrame->addHighlighting(item);
            }
        }
    }

    lineBuffer_->setHighlightings(highlightings);
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
    xmlWriter.writeTextElement("port", m_ui->connectionBar->getDeviceName());
    xmlWriter.writeTextElement("speed", m_ui->connectionBar->getBaudRate());
    xmlWriter.writeTextElement("databits", m_ui->connectionBar->getDataBits());
    xmlWriter.writeTextElement("parity", m_ui->connectionBar->getParity());
    xmlWriter.writeTextElement("stopbits", m_ui->connectionBar->getStopBits());
    xmlWriter.writeTextElement("flowcontrol", m_ui->connectionBar->getFlowControl());
    xmlWriter.writeEndElement();

    QList<Highlighting> h = m_ui->highlightingsFrame->getItems();

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

    QString logFileName = m_ui->logPanel->getLogFileName();

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
    QFont font = QFontDialog::getFont(&ok, m_ui->consoleView->font(), this, QString(), QFontDialog::MonospacedFonts);
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
    const bool bAutoScrollToBottom = !m_ui->consoleView->autoScrollToBottom();

    m_ui->consoleView->setAutoScrollToBottom(bAutoScrollToBottom);
}

void ConsoleTab::setConsoleFont(const QFont& font)
{
    m_ui->consoleView->setFont(font);
}

void ConsoleTab::setColor(const QColor& textColor, const QColor& backgroundColor)
{
    m_ui->consoleView->setTextColor(textColor);
    m_ui->consoleView->setBackgroundColor(backgroundColor);
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
    const bool bTimestampsEnabled = !m_ui->consoleView->timestampsEnabled();

    m_ui->consoleView->setTimestampsEnabled(bTimestampsEnabled);

    if (m_session)
    {
        m_session->setUseTimeStamps(bTimestampsEnabled);
        m_session->saveToFile();
    }
}

void ConsoleTab::onEndpointData(const Message& message)
{
    if (message.isCmd(Message::DataCmd))
    {
        QByteArray data = message.getPayload();

        if (m_logFile)
        {
            m_logFile->write(data);
            m_logFile->flush();
        }

        escapeSpecialChars(data);
        lineBuffer_->append(data);
    }
}

void ConsoleTab::onConnectClicked()
{
    if (!m_portEndpoint->isConnected())
    {
        if (!m_session)
        {
            m_session = new Session();
        }

        m_session->setDeviceName(m_ui->connectionBar->getDeviceName());
        m_session->setTabLabel(m_ui->connectionBar->getDeviceName());
        m_session->setDeviceDesc(m_ui->connectionBar->getDeviceDesc());
        m_session->setBaudRate(m_ui->connectionBar->getBaudRate().toUInt());
        m_session->setDataBits(m_ui->connectionBar->getDataBits().toInt());
        m_session->setParity(Globals::ParityNameMap.key(m_ui->connectionBar->getParity()));
        m_session->setStopBits(Globals::StopBitsNameMap.key(m_ui->connectionBar->getStopBits()));
        m_session->setFlowControl(Globals::FlowControlNameMap.key(m_ui->connectionBar->getFlowControl()));

        m_portEndpoint->connectEndpoint(m_session);
    }
    else
    {
        disconnectEndpoint();
    }
}

void ConsoleTab::disconnectEndpoint()
{
    if (m_portEndpoint->isConnected())
    {
        m_portEndpoint->disconnectEndpoint();
        destroySession();
    }
}

void ConsoleTab::destroySession()
{
    if (m_session)
    {
        delete m_session;
        m_session = nullptr;
    }
}

void ConsoleTab::onEndpointDisconnected(int returnCode)
{
    qDebug() << "[slot] onEndpointDisconnected" << returnCode;

    m_ui->connectionBar->onDisconnected();
    m_ui->consoleView->setFocus();

    switch (returnCode)
    {
        case 1:
            {
                m_ui->statusBar->showMessage(tr("Error connecting to port %1.").arg(m_session->getDeviceName()));
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
    m_ui->statusBar->showMessage("ERROR: " + QString::number(error) + " opening port " );
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
    QString text = m_ui->renameTabFrame->getText();

    setLabel(text);

    if (m_session)
    {
        m_session->setTabLabel(text);

        if (m_session->isPortConnected())
        {
            m_session->saveToFile();
        }
    }
}

void ConsoleTab::showRenameTabDialog()
{
    m_ui->renameTabFrame->setText(getLabel());
    m_ui->renameTabFrame->show();
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

    if (m_portEndpoint->isConnected())
    {
        m_portEndpoint->writeData(b);
    }

    lineBuffer_->append(b); // FIXME: remove
}

void ConsoleTab::startLogging()
{
    qDebug() << "LOGGING STARTED";
    const QString sFileName = m_ui->logPanel->getLogFileName();
    m_logFile = new QFile(sFileName);
    if (!m_logFile->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        m_ui->statusBar->showMessage(tr("Logging to %1 failed.").arg(sFileName));
    }

    m_ui->statusBar->showMessage(tr("Logging to %1 started.").arg(sFileName), 3000);
}

void ConsoleTab::stopLogging()
{
    m_logFile->close();
    delete m_logFile;
    m_logFile = nullptr;
}

void ConsoleTab::onEndpointConnected()
{
    qDebug() << "[slot] onEndpointConnected";

    const QString sDeviceName = m_session->getDeviceName();

    m_ui->connectionBar->onConnected();
    m_ui->consoleView->setFocus();

    setLabel(m_session->getTabLabel());

    m_ui->statusBar->showMessage(tr("Successfully connected to %1.").arg(sDeviceName), 3000);

    if (m_session)
    {
        m_session->saveToFile();
    }
}

void ConsoleTab::onReconnectionSignal(const Message& message)
{
    const QString sDeviceName = m_session->getDeviceName();

    if (message.isCmd(Message::SigCmd))
    {
        Message::Signal sig = message.getSignal();
        qDebug() << "[slot] onReconnectionSignal(" << sig << ")";

        if (sig == Message::IsConSig)
        {
            if (m_session)
            {
                m_session->setPortConnected(false);
            }

            m_ui->statusBar->showProgressMessage(tr("Trying to reconnect to %1...").arg(sDeviceName));
        }
        else if (sig == Message::DoneConSig)
        {
            if (m_session)
            {
                m_session->setPortConnected(true);
            }

            m_ui->statusBar->hideProgressMessage();
            m_ui->statusBar->showMessage(tr("Successfully reconnected to %1.").arg(sDeviceName), 3000);
        }
    }
}

void ConsoleTab::onReconnectionCancel()
{
    m_portEndpoint->writeSignal(Message::CancelConSig);
}

QSize ConsoleTab::getViewPortSize() const
{
    return m_ui->consoleView->viewport()->size();
}

QSize ConsoleTab::getCharWidth() const
{
    return m_ui->consoleView->getCharWidth();
}

// EOF <stefan@scheler.com>
