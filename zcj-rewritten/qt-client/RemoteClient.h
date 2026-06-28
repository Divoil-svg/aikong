#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QImage>

class RemoteClient : public QObject
{
    Q_OBJECT
public:
    explicit RemoteClient(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    void requestCapture();
    void sendCommand(const QString &cmd);

signals:
    void connected();
    void disconnected();
    void captureReceived(const QImage &image);
    void commandResult(const QString &result);
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    QTcpSocket *socket;
    QByteArray buffer;
};

#endif // REMOTECLIENT_H