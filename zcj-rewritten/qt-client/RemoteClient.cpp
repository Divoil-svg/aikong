#include "RemoteClient.h"
#include <QDataStream>
#include <QDebug>
#include <QDateTime>
#include <QJsonArray>

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
        emit errorOccurred("Not connected to server");
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

    QJsonDocument doc = QJsonDocument::fromJson(buffer);
    if (doc.isNull() || !doc.isObject()) {
        return;
    }

    buffer.clear();
    QJsonObject obj = doc.object();
    QString status = obj["status"].toString();
    QString message = obj["message"].toString();

    if (message.contains("屏幕") && obj.contains("data")) {
        QByteArray imageData;
        if (obj["data"].isArray()) {
            QJsonArray arr = obj["data"].toArray();
            imageData.reserve(arr.size());
            for (const auto &v : arr) {
                imageData.append(v.toInt());
            }
        } else if (obj["data"].isString()) {
            // 如果是 base64 或直接字节数组
            imageData = QByteArray::fromBase64(obj["data"].toString().toUtf8());
        }

        QImage img;
        QString format = obj["format"].toString();

        if (format == "jpeg" && !imageData.isEmpty()) {
            img.loadFromData(imageData, "JPEG");
        } else if (!imageData.isEmpty()) {
            // RGB24 fallback
            int w = obj["width"].toInt(800);
            int h = obj["height"].toInt(600);
            img = QImage((const uchar*)imageData.constData(), w, h, QImage::Format_RGB888).copy();
        }

        if (!img.isNull()) {
            emit captureReceived(img);
        } else {
            // Demo fallback
            QImage demo(800, 600, QImage::Format_RGB888);
            demo.fill(Qt::darkCyan);
            emit captureReceived(demo);
        }
    } else {
        emit commandResult(message);
    }
}