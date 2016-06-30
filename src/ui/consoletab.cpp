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

#include "globals.h"
#include "consoletab.h"
#include "mainwindow.h"
#include "ui_consoletab.h"
#include "consoletabwidget.h"
#include "highlightsframe.h"
#include "enumerator/portenumerator.h"
#include "enumerator/serialportinfo.h"
#include "obj/session.h"
#include "port/portendpoint.h"

quint32   CConsoleTab::m_u32counter = 1;

Q_DECLARE_METATYPE(QSerialPort::DataBits)
Q_DECLARE_METATYPE(QSerialPort::StopBits)
Q_DECLARE_METATYPE(QSerialPort::Parity)
Q_DECLARE_METATYPE(QSerialPort::FlowControl)

CConsoleTab::CConsoleTab(CPortEnumerator* pe, CConsoleTabWidget *parent, CSession* session)
    : QWidget(parent)
    , m_ui(new Ui::CConsoleTab)
    , m_portEndpoint(new CPortEndpoint(this))
    , m_pe(pe)
    , m_parent(parent)
    , m_session(session)
    , m_logFile(NULL)
    , m_menu(NULL)
    , m_lastTabIndex(0)
{
    qDebug() << "CConsoleTab::CConsoleTab()";

    m_ui->setupUi(this);

    fillComboBoxes();

    // load font from settings
    QSettings settings;
    QFont consoleFont;
    QColor color = QColor(settings.value("background").toString());
    consoleFont.fromString(settings.value("font").toString());
    setConsoleFont(consoleFont);
    setBackgroundColor(color);

    createContextMenu();

    m_ui->comboDataBits->hide();
    m_ui->comboFlowControl->hide();
    m_ui->comboParity->hide();
    m_ui->comboStopBits->hide();
    m_ui->comboConfigurations->hide();

    QDir dir(QCoreApplication::applicationDirPath());
    QStringList files = dir.entryList(QStringList()<<"*.xml", QDir::Files);
    m_ui->comboConfigurations->insertItems(0, files);

    if (!files.isEmpty())
    {
        m_ui->comboConfigurations->insertItem(0, "Select configuration");
        m_ui->comboConfigurations->setCurrentIndex(0);
        m_ui->comboConfigurations->show();
    }

    connect(m_portEndpoint, &CPortEndpoint::readyRead, this, &CConsoleTab::onEndpointData);
    connect(m_portEndpoint, &CPortEndpoint::connected, this, &CConsoleTab::onEndpointConnected);
    connect(m_portEndpoint, &CPortEndpoint::disconnected, this, &CConsoleTab::onEndpointDisconnected);

    if (m_session)
    {
        m_portEndpoint->setBaudRate(session->getBaudRate());
        m_portEndpoint->setDataBits(session->getDataBits());
        m_portEndpoint->setParity(session->getParity());
        m_portEndpoint->setStopBits(session->getStopBits());
        m_portEndpoint->setFlowControl(session->getFlowControl());
        m_portEndpoint->connectEndpoint(session->getDeviceName());

        m_ui->comboPorts->setCurrentText(session->getDeviceDesc());
        m_ui->comboBaudRates->setCurrentText(QString::number(session->getBaudRate()));
        m_ui->comboDataBits->setCurrentText(QString::number(session->getDataBits()));

        QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(session->getParity());
        m_ui->comboParity->setCurrentText(g_ParityNameMap.value(parity));

        QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>(session->getStopBits());
        m_ui->comboStopBits->setCurrentText(g_StopBitsNameMap.value(stopBits));

        QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(session->getFlowControl());
        m_ui->comboFlowControl->setCurrentText(g_FlowControlNameMap.value(flowControl));

        QList<CHighlightsFrame::Highlighting> highlights;

        foreach(const QVariant& h, m_session->getHighlights())
        {
            CHighlightsFrame::Highlighting hi = h.value<CHighlightsFrame::Highlighting>();
            QPixmap pixmap(10, 10);
            pixmap.fill(hi.color);
            QIcon icon(pixmap);
            QListWidgetItem *item = new QListWidgetItem(icon, hi.pattern);
            item->setData(Qt::UserRole, QVariant(hi.color));
            m_ui->highlightsFrame->addHighlighting(item);
            highlights.append(hi);
        }

        m_ui->consoleView->setHighlighting(highlights);

        m_ui->btnConnect->setEnabled(true); // FIXME: unsauber
    }
}

CConsoleTab::~CConsoleTab()
{
    qDebug() << "CConsoleTab::~CConsoleTab()";
    delete m_ui;
    delete m_menu;
}

void CConsoleTab::fillComboBoxes(void)
{
    QComboBox *combo = NULL;

    /** FIXME: DO THIS IN A MORE ELEGANT WAY! */
    //while(m_pe->getAvailablePorts().count() == 0) { QThread::msleep(20); }

    m_ui->comboPorts->setPortEnumerator(m_pe);

    // fill baud rates
    combo = m_ui->comboBaudRates;
#if 0
    foreach (const qint32 baudrate, QSerialPortInfo::standardBaudRates())
    {
        combo->addItem(QString::number(baudrate), QVariant(baudrate));
    }
#endif
    // just use more common baud rates for now
    combo->clear();
    combo->addItem(tr("Select baud rate"));
    combo->addItem("9600", QVariant(9600));
    combo->addItem("19200", QVariant(19200));
    combo->addItem("38400", QVariant(38400));
    combo->addItem("57600", QVariant(57600));
    combo->addItem("115200", QVariant(115200));
    combo->addItem("custom", QVariant(0));

    // fill data bits
    combo = m_ui->comboDataBits;
    combo->clear();
    combo->addItem("5", qVariantFromValue(QSerialPort::Data5));
    combo->addItem("6", qVariantFromValue(QSerialPort::Data6));
    combo->addItem("7", qVariantFromValue(QSerialPort::Data7));
    combo->addItem("8", qVariantFromValue(QSerialPort::Data8));
    combo->setCurrentIndex(3);

    // fill parity
    combo = m_ui->comboParity;
    combo->clear();
    combo->addItem(tr("None"), qVariantFromValue(QSerialPort::NoParity));
    combo->addItem(tr("Even"), qVariantFromValue(QSerialPort::OddParity));
    combo->addItem(tr("Space"), qVariantFromValue(QSerialPort::SpaceParity));
    combo->addItem(tr("Mark"), qVariantFromValue(QSerialPort::MarkParity));

    // fill stop bits
    combo = m_ui->comboStopBits;
    combo->clear();
    combo->addItem("1", qVariantFromValue(QSerialPort::OneStop));
    combo->addItem("1.5", qVariantFromValue(QSerialPort::OneAndHalfStop));
    combo->addItem("2", qVariantFromValue(QSerialPort::TwoStop));
    combo->setCurrentIndex(0);

    // fill flow control
    combo = m_ui->comboFlowControl;
    combo->clear();
    combo->addItem(tr("None"), qVariantFromValue(QSerialPort::NoFlowControl));
    combo->addItem(tr("Xon/Xoff"), qVariantFromValue(QSerialPort::SoftwareControl));
    combo->addItem(tr("Hardware"), qVariantFromValue(QSerialPort::HardwareControl));
    combo->setCurrentIndex(0);
}

void CConsoleTab::toggleFullScreen(void)
{
    static QString sLastTitle;
    if (!isFullScreen())
    {
        m_lastTabIndex = m_parent->currentIndex();
        qDebug() << m_lastTabIndex;
        sLastTitle = m_parent->tabText(m_lastTabIndex);
        m_parent->addNewTab(NULL); // add dummy tab
        setParent(0);
        showFullScreen();
        m_ui->actionFullscreen->setChecked(true);
    }
    else
    {
        setParent(m_parent);
        qDebug() << m_lastTabIndex;
        m_parent->insertTab(m_lastTabIndex, this, sLastTitle);
        m_parent->closeTab(m_parent->count()-1);
        m_parent->setCurrentIndex(m_lastTabIndex);
        m_ui->consoleView->setFocus();
        m_ui->actionFullscreen->setChecked(false);
    }
}

void CConsoleTab::createContextMenu()
{
    m_menu = new QMenu(this);
    m_menu->addAction(m_ui->actionConfiguration);
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

void CConsoleTab::showContextMenu(const QPoint &pt)
{
    m_menu->exec(mapToGlobal(pt));
}

void CConsoleTab::updateHighlighting()
{
    QList<CHighlightsFrame::Highlighting> h = m_ui->highlightsFrame->getItems();
    m_ui->consoleView->setHighlighting(h);
}

void CConsoleTab::showConnectBar(void)
{
    m_ui->btnBar->show();
}

void CConsoleTab::hideConnectBar(void)
{
    m_ui->btnBar->hide();
}

void CConsoleTab::onConfigurationChanged(const QString &text)
{
    qDebug() << "CURRENTINDEX: " << m_ui->comboConfigurations->currentIndex();

    if (m_ui->comboConfigurations->currentIndex() == 0)
        return;

    QString fileName = QDir(QCoreApplication::applicationDirPath()).filePath(text);

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
                QString text = xml.readElementText();
                m_ui->comboPorts->setCurrentText(text);
                qDebug() << "port: [" << text << "] " << m_ui->comboPorts->findText(text);
            }
            else if (token == "speed")
            {
                QString text = xml.readElementText();
                m_ui->comboBaudRates->setCurrentText(text);
            }
            else if (token == "databits")
            {
                QString text = xml.readElementText();
                m_ui->comboDataBits->setCurrentText(text);
            }
            else if (token == "parity")
            {
                QString text = xml.readElementText();
                m_ui->comboParity->setCurrentText(text);
            }
            else if (token == "stopbits")
            {
                QString text = xml.readElementText();
                m_ui->comboStopBits->setCurrentText(text);
            }
            else if (token == "flowcontrol")
            {
                QString text = xml.readElementText();
                m_ui->comboFlowControl->setCurrentText(text);
            }
            else if (token == "pattern")
            {
                QXmlStreamAttributes attr = xml.attributes();
                CHighlightsFrame::Highlighting hi;
                hi.pattern = xml.readElementText();
                hi.color = QColor(attr.value("color").toString());
                h.append(hi);

                QPixmap pixmap(10, 10);
                pixmap.fill(hi.color );
                QIcon icon(pixmap);
                QListWidgetItem *item = new QListWidgetItem(icon, hi.pattern);
                item->setData(Qt::UserRole, QVariant(hi.color));
                m_ui->highlightsFrame->addHighlighting(item);
            }
        }
    }

    m_ui->consoleView->setHighlighting(h);
}

void CConsoleTab::showSaveDialog(void)
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
    xmlWriter.writeTextElement("port", m_ui->comboPorts->currentText());
    xmlWriter.writeTextElement("speed", m_ui->comboBaudRates->currentText());
    xmlWriter.writeTextElement("databits", m_ui->comboDataBits->currentText());
    xmlWriter.writeTextElement("parity", m_ui->comboParity->currentText());
    xmlWriter.writeTextElement("stopbits", m_ui->comboStopBits->currentText());
    xmlWriter.writeTextElement("flowcontrol", m_ui->comboFlowControl->currentText());
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

void CConsoleTab::showColorDialog(void)
{
    QSettings settings;
    QColor initial(settings.value("background").toString());
    QColor rgb = QColorDialog::getColor(initial, this);
    if (rgb.isValid())
    {
        setBackgroundColor(rgb);
        settings.setValue("background", rgb.name());
    }
}

void CConsoleTab::showFontDialog(void)
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, m_ui->consoleView->font(), this);
    if (ok)
    {
        QSettings settings;
        settings.setValue("font", font.toString());
        setConsoleFont(font);
    }
}

void CConsoleTab::toggleAutoScroll(void)
{
    static bool bEnabled = true;
    bEnabled = !bEnabled;
    m_ui->consoleView->setAutoScroll(bEnabled);
}

void CConsoleTab::setConsoleFont(const QFont &font)
{
    m_ui->consoleView->setFont(font);
    m_ui->consoleView->refreshCursor();
}

void CConsoleTab::setBackgroundColor(const QColor &color)
{
    setStyleSheet(QString("QPlainTextEdit { background-color: %1; }").arg(color.name()));
}

void CConsoleTab::onMoreClicked()
{
    if (!m_ui->comboParity->isVisible())
    {
        m_ui->comboDataBits->show();
        m_ui->comboFlowControl->show();
        m_ui->comboParity->show();
        m_ui->comboStopBits->show();
        m_ui->btnMore->setText(tr("<< &Less"));
    }
    else
    {
        m_ui->comboDataBits->hide();
        m_ui->comboFlowControl->hide();
        m_ui->comboParity->hide();
        m_ui->comboStopBits->hide();
        m_ui->btnMore->setText(tr("&More >>"));
    }
}

void CConsoleTab::onEndpointData()
{
    QByteArray data = m_portEndpoint->readAll();

    if (m_logFile)
    {
        m_logFile->write(data);
        m_logFile->flush();
    }
#if 0
    for (int p = 0; p < data.size(); p++)
    {
        printf("0x%02x ", data.at(p));
    }
    printf("\n");
#endif

    if (data.at(0) == 0x08)
    {
        m_ui->consoleView->insertBackspace();
    }
    else
    {
#if 0
        for (int p = 0; p < data.size(); p++)
        {
            if (data.at(p) < 30)
            {
                data[p] = '.';
            }
        }
#endif

        QString str = data;
        str = str.replace("\r", "");
        m_ui->consoleView->insertPlainText(str);
    }
}

void CConsoleTab::onConnectClicked(void)
{
    const QString sDeviceName = m_ui->comboPorts->currentData().toString();

    if (!m_portEndpoint->isConnected())
    {
        if (!m_session)
        {
            m_session = new CSession();
        }

        m_session->setBaudRate(m_ui->comboBaudRates->currentText().toUInt());
        m_session->setDeviceName(sDeviceName);
        m_session->setDeviceDesc(m_ui->comboPorts->currentText());
        m_session->setDataBits(m_ui->comboDataBits->currentText().toInt());
        m_session->setParity(g_ParityNameMap.key(m_ui->comboParity->currentText()));
        m_session->setStopBits(g_StopBitsNameMap.key(m_ui->comboStopBits->currentText()));
        m_session->setFlowControl(g_FlowControlNameMap.key(m_ui->comboFlowControl->currentText()));

        m_portEndpoint->setBaudRate(m_ui->comboBaudRates->currentText().toUInt());
        m_portEndpoint->setDataBits(m_ui->comboDataBits->currentText().toInt());
        m_portEndpoint->setParity(m_ui->comboParity->currentText().toInt());
        m_portEndpoint->setStopBits(m_ui->comboStopBits->currentText().toInt());
        m_portEndpoint->setFlowControl(m_ui->comboFlowControl->currentText().toInt());
        m_portEndpoint->connectEndpoint(sDeviceName);
    }
    else
    {
        m_portEndpoint->disconnectEndpoint();
    }
}

void CConsoleTab::onEndpointDisconnected()
{
    qDebug() << "[slot] onEndpointDisconnected";

    m_ui->comboPorts->setEnabled(true);
    m_ui->btnConnect->setText("&Connect");

    showConnectBar();
    m_ui->consoleView->setFocus();
/*
    m_parent->setCurrentTabText(sDeviceName);

    m_ui->statusBar->showMessage(tr("Successfully connected to %1.").arg(sDeviceName), 3000);    */
}

void CConsoleTab::showError(QSerialPort::SerialPortError error)
{
    qDebug() << "ERROR: " << error;
    m_ui->statusBar->showMessage("ERROR: " + QString::number(error) + " opening port " );
}

void CConsoleTab::onComboChanged(void)
{
    if (m_ui->comboPorts->currentIndex() != 0 && m_ui->comboBaudRates->currentIndex() != 0)
    {
        m_ui->btnConnect->setEnabled(true);
        m_ui->btnSave->setEnabled(true);
    }
    else
    {
        m_ui->btnConnect->setEnabled(false);
        m_ui->btnSave->setEnabled(false);
    }
}

void CConsoleTab::showAboutDialog(void)
{
    const QString contents = QString(
        "<p><font color=#000080><font size=6><b>%1</b></font> <font size=4>(revision %2)</font></font></p>"
        "<p align=left>Copyright &copy; 2015 Stefan Scheler. %3</p>"
        "<p><a href=\"%4\">%5</a></p>"
        "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</p>")
        .arg(g_sAppFullName, QString::number(g_u32revision), tr("All rights reserved."), g_sAppHomepage, tr("Visit superterm website"));

    QMessageBox::about(this, tr("About superterm"), contents);
}

void CConsoleTab::onKeyPressed(QKeyEvent *e)
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
        m_portEndpoint->write(b);
    }
}

void CConsoleTab::startLogging(void)
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

void CConsoleTab::stopLogging(void)
{
    m_logFile->close();
    delete m_logFile;
    m_logFile = NULL;
}

void CConsoleTab::onAppQuit(void)
{
    qDebug() << "[slot] onAppQuit";

    if (m_session)
    {
        m_session->setHighlights(CSeriazableObject::convertToQVariantList(m_ui->highlightsFrame->getItems()));
        m_session->saveToFile();
    }
}

void CConsoleTab::onEndpointConnected()
{
    qDebug() << "[slot] onEndpointConnected";

    const QString sDeviceName = m_session->getDeviceName();

    hideConnectBar();
    m_ui->consoleView->setFocus();

    m_parent->setCurrentTabText(sDeviceName);

    m_ui->comboPorts->setEnabled(false);
    m_ui->btnConnect->setText(tr("&Disconnect"));

    m_ui->statusBar->showMessage(tr("Successfully connected to %1.").arg(sDeviceName), 3000);
#if 0
    }
    else
    {
        qDebug() << m_port->error();
        delete m_port;
        m_port = NULL;
        m_ui->statusBar->showMessage(tr("Error. Failed to open port."));
    }

    connect(m_port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(showError(QSerialPort::SerialPortError)));
#endif
}

// EOF <stefan@scheler.com>
