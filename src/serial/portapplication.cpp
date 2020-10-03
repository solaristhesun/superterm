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

#include <QCoreApplication>
#include <QLocalSocket>
#include <QTimer>
#include <QSerialPort>

#include "ipc/message.h"
#include "ipc/messagecodec.h"
#include "serial/portobserver.h"
#include "serial/portapplication.h"

#if 0
void dumpDCB(const char* szFileName)
{
    printf("using port %s\n", szFileName);

#if defined(Q_OS_WIN)
    HANDLE h = CreateFileA(szFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, 0);

    DCB dcb;

    GetCommState(h, &dcb);

    printf("dcb.DCBlength:         0x%08x\n", dcb.DCBlength);
    printf("dcb.BaudRate:          0x%08x (%lu)\n", dcb.BaudRate, dcb.BaudRate);
    printf("dcb.fBinary:           0x%08x\n", dcb.fBinary);
    printf("dcb.fParity:           0x%08x\n", dcb.fParity);
    printf("dcb.fOutxCtsFlow:      0x%08x\n", dcb.fOutxCtsFlow);
    printf("dcb.fOutxDsrFlow:      0x%08x\n", dcb.fOutxDsrFlow);
    printf("dcb.fDtrControl:       0x%08x\n", dcb.fDtrControl);
    printf("dcb.fDsrSensitivity:   0x%08x\n", dcb.fDsrSensitivity);
    printf("dcb.fTXContinueOnXoff: 0x%08x\n", dcb.fTXContinueOnXoff);
    printf("dcb.fOutX:             0x%08x\n", dcb.fOutX);
    printf("dcb.fInX:              0x%08x\n", dcb.fInX);
    printf("dcb.fErrorChar:        0x%08x\n", dcb.fErrorChar);
    printf("dcb.fNull:             0x%08x\n", dcb.fNull);
    printf("dcb.fRtsControl:       0x%08x\n", dcb.fRtsControl);
    printf("dcb.fAbortOnError:     0x%08x\n", dcb.fAbortOnError);
    printf("dcb.XonLim:            0x%04x\n", dcb.XonLim);
    printf("dcb.XoffLim:           0x%04x\n", dcb.XoffLim);
    printf("dcb.ByteSize:          0x%02x\n", dcb.ByteSize);
    printf("dcb.Parity:            0x%02x\n", dcb.Parity);
    printf("dcb.StopBits:          0x%02x\n", dcb.StopBits);

    printf("dcb.XonChar:           0x%02x\n", dcb.XonChar);
    printf("dcb.XoffChar:          0x%02x\n", dcb.XoffChar);
    printf("dcb.ErrorChar:         0x%02x\n", dcb.ErrorChar);
    printf("dcb.EofChar:           0x%02x\n", dcb.EofChar);
    printf("dcb.EvtChar:           0x%02x\n", dcb.EvtChar);

    CloseHandle(h);
#endif
}
#endif

PortApplication::PortApplication(int& argc, char** argv)
    : QCoreApplication(argc, argv)
    , socket_(new QLocalSocket(this))
    , port_(nullptr)
    , observer_(new PortObserver())
    , bHasSendReconSignal_(false)
{
    // currently empty
}

int PortApplication::exec()
{
    QString                  portName = arguments().at(1);
    quint32                  baudRate = arguments().at(2).toUInt();
    QSerialPort::DataBits    dataBits = static_cast<QSerialPort::DataBits>(arguments().at(3).toInt());
    QSerialPort::Parity      parity = static_cast<QSerialPort::Parity>(arguments().at(4).toInt());
    QSerialPort::StopBits    stopBits = static_cast<QSerialPort::StopBits>(arguments().at(5).toInt());
    QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(arguments().at(6).toInt());

    connect(socket_, &QLocalSocket::connected, this, &PortApplication::onSocketConnected);
    connect(socket_, &QLocalSocket::disconnected, this, &PortApplication::onSocketDisconnected);
    connect(socket_, &QLocalSocket::readyRead, this, &PortApplication::onSocketData);
    connect(socket_, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::errorOccurred), this, &PortApplication::onSocketError);
    connect(observer_, &PortObserver::disconnected, this, &PortApplication::onPortDisconnected);

    port_ = new QSerialPort(portName);

    if (port_->open(QIODevice::ReadWrite))
    {
        QTextStream(stdout) << "open:" << portName << Qt::endl;
        port_->setBaudRate(baudRate);
        port_->setFlowControl(flowControl);
        port_->setParity(parity);
        port_->setStopBits(stopBits);
        port_->setDataBits(dataBits);
    }
    else
    {
        QTextStream(stdout) << "error:" << port_->errorString();
        return 1;
    }

    connect(port_, &QSerialPort::readyRead, this, &PortApplication::onSerialDataAvailable);

    socket_->abort();
    socket_->connectToServer("serial:" + portName.replace("/", "_"));

    QTextStream(stdout) << "started:" << portName << Qt::endl;

    return QCoreApplication::exec();
}

void PortApplication::onSerialDataAvailable()
{
    socket_->write(MessageCodec::encodeData(port_->readAll()));
}

void PortApplication::onSocketData()
{
    QByteArray array = socket_->readAll();

    while (array.size() > 0)
    {
        const Message& message = MessageCodec::decode(array);

        if (message.isCmd(Message::DataCmd))
        {
            port_->write(message.getPayload());
        }
        else if (message.isCmd(Message::SigCmd))
        {
            if (message.getSignal() == Message::CancelConSig)
            {
                observer_->setActive(false);
            }
        }
    }
}

void PortApplication::onSocketDisconnected()
{
    QTextStream(stdout) << "disconnected" << Qt::endl;
    observer_->setActive(false);
    QCoreApplication::quit();
}

void PortApplication::onSocketConnected()
{
    QTextStream(stdout) << "connected" << Qt::endl;
    observer_->setPort(port_);
    observer_->setActive(true);
}

void PortApplication::onSocketError(QLocalSocket::LocalSocketError error)
{
    QTextStream(stdout) << error << Qt::endl;
    QCoreApplication::quit();
}

void PortApplication::onPortDisconnected()
{
    QString                  portName = port_->portName();
    quint32                  baudRate = port_->baudRate();
    QSerialPort::DataBits    dataBits = port_->dataBits();
    QSerialPort::Parity      parity = port_->parity();
    QSerialPort::StopBits    stopBits = port_->stopBits();
    QSerialPort::FlowControl flowControl = port_->flowControl();

    port_->close();

    if (!bHasSendReconSignal_)
    {
        socket_->write(MessageCodec::encodeSignal(Message::IsConSig));
        bHasSendReconSignal_ = true;
    }

    if (port_->open(QIODevice::ReadWrite))
    {
        port_->setPortName(portName);
        port_->setBaudRate(baudRate);
        port_->setFlowControl(flowControl);
        port_->setParity(parity);
        port_->setStopBits(stopBits);
        port_->setDataBits(dataBits);

        socket_->write(MessageCodec::encodeSignal(Message::DoneConSig));
        bHasSendReconSignal_ = false;
    }
}

// EOF <stefan@scheler.com>
