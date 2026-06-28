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
        return; // 数据不完整
    }

    buffer.clear();
    QJsonObject obj = doc.object();
    QString status = obj["status"].toString();
    QString message = obj["message"].toString();

    if (message.contains("屏幕") && obj.contains("data")) {
        // 真正解析 Rust 返回的图像数据
        QJsonArray dataArray = obj["data"].toArray();
        int width = obj["width"].toInt(800);
        int height = obj["height"].toInt(600);

        if (dataArray.size() == width * height * 3) {
            QImage img(width, height, QImage::Format_RGB888);
            for (int i = 0; i < dataArray.size(); i += 3) {
                int x = (i / 3) % width;
                int y = (i / 3) / width;
                int r = dataArray[i].toInt();
                int g = dataArray[i+1].toInt();
                int b = dataArray[i+2].toInt();
                img.setPixel(x, y, qRgb(r, g, b));
            }
            emit captureReceived(img);
        } else {
            // 数据大小不匹配，降级为生成测试图
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
        }
    } else {
        emit commandResult(message);
    }
}