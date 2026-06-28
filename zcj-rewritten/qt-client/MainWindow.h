#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>

class RemoteDesktopView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectClicked();
    void onDisconnectClicked();
    void onCaptureScreenClicked();
    void onExecuteCommandClicked();
    void appendLog(const QString &msg);

private:
    void setupUi();
    void setupConnections();

    QTabWidget *tabWidget;
    RemoteDesktopView *remoteView;
    QTextEdit *logTextEdit;
    QLineEdit *commandLineEdit;
    QPushButton *connectBtn;
    QPushButton *disconnectBtn;
    QPushButton *captureBtn;
    QPushButton *execBtn;

    bool isConnected = false;
};

#endif // MAINWINDOW_H