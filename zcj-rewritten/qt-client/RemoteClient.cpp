#include "RemoteClient.h"
#include <QDataStream>
#include <QDebug>

RemoteClient::RemoteClient(QObject *parent)
    : QObject(parent)
    , socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::connected, this, &RemoteClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &RemoteClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &RemoteClient::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, [this](QAbstractSocket::SocketError) {
                emit errorOccurred(socket->errorString());
            });
}

void RemoteClient::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void RemoteClient::disconnectFromServer()
{
    socket->disconnectFromHost();
}

void RemoteClient::requestCapture()
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit errorOccurred("Not connected");
        return;
    }

    QJsonObject cmd;
    cmd["cmd_type"] = "capture_screen";
    cmd["timestamp"] = QDateTime::currentSecsSinceEpoch();

    QJsonDocument doc(cmd);
    socket->write(doc.toJson(QJsonDocument::Compact));
}

void RemoteClient::sendCommand(const QString &cmdText)
{
    if (socket->state() != QAbstractSocket::ConnectedState) return;

    QJsonObject cmd;
    cmd["cmd_type"] = "execute_cmd";
    cmd["payload"] = cmdText.toUtf8();
    cmd["timestamp"] = QDateTime::currentSecsSinceEpoch();

    QJsonDocument doc(cmd);
    socket->write(doc.toJson(QJsonDocument::Compact));
}

void RemoteClient::onConnected()
{
    emit connected();
}

void RemoteClient::onDisconnected()
{
    emit disconnected();
}

void RemoteClient::onReadyRead()
{
    buffer.append(socket->readAll());

    // 简单处理：假设每次接到完整 JSON
    QJsonDocument doc = QJsonDocument::fromJson(buffer);
    if (doc.isNull()) return;

    buffer.clear();

    QJsonObject obj = doc.object();
    QString status = obj["status"].toString();

    if (obj.contains("data") && obj["message"].toString().contains("屏幕")) {
        // 处理捕获帧 (Demo: 假设收到 RGB 数据后生成 QImage)
        // 实际应解析宽高并转换
        QImage img(800, 600, QImage::Format_RGB888);
        img.fill(Qt::blue); // Demo 效果
        emit captureReceived(img);
    } else {
        emit commandResult(obj["message"].toString());
    }
}