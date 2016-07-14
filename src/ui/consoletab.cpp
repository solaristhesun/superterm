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
#if defined(Q_OS_WIN)
#include <windows.h>
#endif

#include "ui_consoletab.h"
#include "misc/globals.h"
#include "ui/consoletab.h"
#include "ui/mainwindow.h"
#include "ui/consoletabwidget.h"
#include "ui/highlightsframe.h"
#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"
#include "serial/portendpoint.h"
#include "session/session.h"
#include "ipc/message.h"

quint32 CConsoleTab::m_u32counter = 1;

Q_DECLARE_METATYPE(QSerialPort::DataBits)
Q_DECLARE_METATYPE(QSerialPort::StopBits)
Q_DECLARE_METATYPE(QSerialPort::Parity)
Q_DECLARE_METATYPE(QSerialPort::FlowControl)

CConsoleTab::CConsoleTab(CPortEnumerator* pe, CSession* session)
    : QWidget(Q_NULLPTR)
    , m_ui(new Ui::CConsoleTab)
    , mMainWindow(Q_NULLPTR)
    , mTabLabel(tr("New tab"))
    , m_portEndpoint(new CPortEndpoint(this))
    , m_session(session)
    , m_logFile(NULL)
    , m_menu(NULL)
    , m_lastTabIndex(0)
{
    qDebug() << "CConsoleTab::CConsoleTab()";

    m_ui->setupUi(this);

    // load font from settings
    QSettings settings;
    QFont     consoleFont;
    QColor    color = QColor(settings.value("background").toString());
    consoleFont.fromString(settings.value("font").toString());
    setConsoleFont(consoleFont);
    setBackgroundColor(color);

    createContextMenu();

    connect(m_portEndpoint, &CPortEndpoint::readyRead, this, &CConsoleTab::onEndpointData);
    connect(m_portEndpoint, &CPortEndpoint::connected, this, &CConsoleTab::onEndpointConnected);
    connect(m_portEndpoint, &CPortEndpoint::disconnected, this, &CConsoleTab::onEndpointDisconnected);
    connect(m_portEndpoint, &CPortEndpoint::readyRead, this, &CConsoleTab::onReconnectionSignal);
    connect(m_ui->statusBar, &CStatusBarFrame::cancelReconnection, this, &CConsoleTab::onReconnectionCancel);

    m_ui->connectionBar->setPortEnumerator(pe);

    if (session)
    {
        m_portEndpoint->connectEndpoint(session);

        m_ui->connectionBar->loadFromSession(session);

        QList<CHighlightsFrame::Highlighting> highlights;

        for (const QVariant& h : session->getHighlights())
        {
            CHighlightsFrame::Highlighting hi = h.value<CHighlightsFrame::Highlighting>();
            QPixmap                        pixmap(10, 10);
            pixmap.fill(hi.color);
            QIcon            icon(pixmap);
            QListWidgetItem* item = new QListWidgetItem(icon, hi.pattern);
            item->setData(Qt::UserRole, QVariant(hi.color));
            m_ui->highlightsFrame->addHighlighting(item);
            highlights.append(hi);
        }

        m_ui->consoleView->setHighlighting(highlights);
    }
}

CConsoleTab::~CConsoleTab()
{
    qDebug() << "CConsoleTab::~CConsoleTab()";
    delete m_ui;
    delete m_menu;
}

QString CConsoleTab::getLabel() const
{
    return mTabLabel;
}

void CConsoleTab::setLabel(const QString& label)
{
    mTabLabel = label;

    emit labelChanged(label);
}

void CConsoleTab::toggleFullScreen()
{
    if (!QWidget::isFullScreen())
    {
        mMainWindow = static_cast<CMainWindow*>(QApplication::activeWindow());
        mMainWindow->hide();
        QWidget::setParent(0);
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

void CConsoleTab::createContextMenu()
{
    m_menu = new QMenu(this);
    m_menu->addAction(m_ui->actionConnection );
    m_menu->addAction(m_ui->actionLogging);
    m_menu->addAction(m_ui->actionHighlight);
    m_menu->addSeparator();
    m_menu->addAction(m_ui->actionToggleAutoscroll);
    m_menu->addSeparator();
    m_menu->addAction(m_ui->actionChangeColor);
    m_menu->addAction(m_ui->actionChangeFont);
    m_menu->addSeparator();
    m_menu->addAction(m_ui->actionClear);
    m_menu->addSeparator();
    m_menu->addAction(m_ui->actionFullscreen);
    m_menu->addSeparator();
    m_menu->addAction(m_ui->actionAbout);
    m_ui->actionToggleAutoscroll->setChecked(true);
}

void CConsoleTab::showContextMenu(const QPoint& pt)
{
    m_menu->exec(mapToGlobal(pt));
}

void CConsoleTab::updateHighlighting()
{
    QList<CHighlightsFrame::Highlighting> h = m_ui->highlightsFrame->getItems();
    m_ui->consoleView->setHighlighting(h);
}

void CConsoleTab::onConfigurationChanged(const QString& config)
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

    QList<CHighlightsFrame::Highlighting> h;
    m_ui->highlightsFrame->clear();

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
                CHighlightsFrame::Highlighting hi;
                hi.pattern = xml.readElementText();
                hi.color = QColor(attr.value("color").toString());
                h.append(hi);

                QPixmap pixmap(10, 10);
                pixmap.fill(hi.color );
                QIcon            icon(pixmap);
                QListWidgetItem* item = new QListWidgetItem(icon, hi.pattern);
                item->setData(Qt::UserRole, QVariant(hi.color));
                m_ui->highlightsFrame->addHighlighting(item);
            }
        }
    }

    m_ui->consoleView->setHighlighting(h);
}

void CConsoleTab::showSaveDialog()
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

    QList<CHighlightsFrame::Highlighting> h = m_ui->highlightsFrame->getItems();

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

void CConsoleTab::showColorDialog()
{
    QSettings settings;
    QColor    initial(settings.value("background").toString());
    QColor    rgb = QColorDialog::getColor(initial, this);
    if (rgb.isValid())
    {
        setBackgroundColor(rgb);
        settings.setValue("background", rgb.name());
    }
}

void CConsoleTab::showFontDialog()
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

void CConsoleTab::toggleAutoScroll()
{
    static bool bEnabled = true;
    bEnabled = !bEnabled;
    m_ui->consoleView->setAutoScroll(bEnabled);
}

void CConsoleTab::setConsoleFont(const QFont& font)
{
    m_ui->consoleView->setFont(font);
    m_ui->consoleView->refreshCursor();
}

void CConsoleTab::setBackgroundColor(const QColor& color)
{
    setStyleSheet(QString("QPlainTextEdit { background-color: %1; }").arg(color.name()));
}

void CConsoleTab::onEndpointData(const CMessage& message)
{
    if (message.isCmd(CMessage::DataCmd))
    {
        QByteArray data = message.getPayload();

        if (m_logFile)
        {
            m_logFile->write(data);
            m_logFile->flush();
        }

        if (data.at(0) == 0x08)
        {
            m_ui->consoleView->insertBackspace();
        }
        else
        {
            for (int p = 0; p < data.size(); p++)
            {
                if (data.at(p) < 30 && data.at(p) != '\n' && data.at(p) != '\r')
                {
                    data[p] = '.';
                }
            }

            QString str = data;
            str = str.replace("\r", "");
            m_ui->consoleView->insertPlainText(str);
        }
    }
}

void CConsoleTab::onConnectClicked()
{
    if (!m_portEndpoint->isConnected())
    {
        if (!m_session)
        {
            m_session = new CSession();
        }

        m_session->setDeviceName(m_ui->connectionBar->getDeviceName());
        m_session->setDeviceDesc(m_ui->connectionBar->getDeviceDesc());
        m_session->setBaudRate(m_ui->connectionBar->getBaudRate().toUInt());
        m_session->setDataBits(m_ui->connectionBar->getDataBits().toInt());
        m_session->setParity(g_ParityNameMap.key(m_ui->connectionBar->getParity()));
        m_session->setStopBits(g_StopBitsNameMap.key(m_ui->connectionBar->getStopBits()));
        m_session->setFlowControl(g_FlowControlNameMap.key(m_ui->connectionBar->getFlowControl()));

        m_portEndpoint->connectEndpoint(m_session);
    }
    else
    {
        m_portEndpoint->disconnectEndpoint();
    }
}

void CConsoleTab::onEndpointDisconnected(int returnCode)
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
}

void CConsoleTab::showError(QSerialPort::SerialPortError error)
{
    qDebug() << "ERROR: " << error;
    m_ui->statusBar->showMessage("ERROR: " + QString::number(error) + " opening port " );
}

void CConsoleTab::showAboutDialog()
{
    const QString contents = QString(
        "<p><font color=#000080><font size=6><b>%1</b></font> <font size=4>(revision %2)</font></font></p>"
        "<p align=left>Copyright &copy; 2015-2016 Stefan Scheler. %3</p>"
        "<p><a href=\"%4\">%5</a></p>"
        "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</p>")
                             .arg(g_sAppFullName, QString::number(g_u32revision), tr("All rights reserved."), g_sAppHomepage, tr("Visit superterm website"));

    QMessageBox::about(this, tr("About superterm"), contents);
}

void CConsoleTab::onKeyPressed(QKeyEvent* e)
{
    QString key;

    if (e->key() == Qt::Key_Return)
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
}

void CConsoleTab::startLogging()
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

void CConsoleTab::stopLogging()
{
    m_logFile->close();
    delete m_logFile;
    m_logFile = NULL;
}

void CConsoleTab::onAppQuit()
{
    qDebug() << "[slot] onAppQuit";

    if (m_session && m_session->isPortConnected())
    {
        m_session->setHighlights(CSerializableObject::convertToQVariantList(m_ui->highlightsFrame->getItems()));
        m_session->saveToFile();
    }
}

void CConsoleTab::onEndpointConnected()
{
    qDebug() << "[slot] onEndpointConnected";

    const QString sDeviceName = m_session->getDeviceName();

    m_ui->connectionBar->onConnected();
    m_ui->consoleView->setFocus();

    setLabel(sDeviceName);

    m_ui->statusBar->showMessage(tr("Successfully connected to %1.").arg(sDeviceName), 3000);
}

void CConsoleTab::onReconnectionSignal(const CMessage& message)
{
    const QString sDeviceName = m_session->getDeviceName();

    if (message.isCmd(CMessage::SigCmd))
    {
        CMessage::Signal sig = message.getSignal();
        qDebug() << "[slot] onReconnectionSignal(" << sig << ")";

        if (sig == CMessage::IsConSig)
        {
            if (m_session)
            {
                m_session->setPortConnected(false);
            }

            m_ui->statusBar->showProgressMessage(tr("Trying to reconnect to %1...").arg(sDeviceName));
        }
        else if (sig == CMessage::DoneConSig)
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

void CConsoleTab::onReconnectionCancel()
{
    m_portEndpoint->writeSignal(CMessage::CancelConSig);
}

// EOF <stefan@scheler.com>
