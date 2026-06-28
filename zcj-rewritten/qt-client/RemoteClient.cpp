#include "RemoteClient.h"
#include <QDataStream>
#include <QDebug>
#include <QDateTime>

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

    // 简单处理：假设每次接到完整的 JSON 响应
    QJsonDocument doc = QJsonDocument::fromJson(buffer);
    if (doc.isNull() || !doc.isObject()) {
        // 数据不完整，等待更多数据
        return;
    }

    buffer.clear();
    QJsonObject obj = doc.object();

    QString status = obj["status"].toString();
    QString message = obj["message"].toString();

    if (message.contains("屏幕") || obj.contains("data")) {
        // 处理捕获帧
        // 当前 Rust 返回的是 RGB24 数据 (width=800, height=600)
        int width = 800;
        int height = 600;

        // TODO: 从 obj["data"] 真正解析字节数据
        // 当前 Demo: 生成渐变图像以模拟效果
        QImage img(width, height, QImage::Format_RGB888);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int r = (x * 255) / width;
                int g = (y * 255) / height;
                int b = 128;
                img.setPixel(x, y, qRgb(r, g, b));
            }
        }
        emit captureReceived(img);
    } else {
        emit commandResult(message);
    }
}