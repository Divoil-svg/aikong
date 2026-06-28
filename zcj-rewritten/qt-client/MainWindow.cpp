#include "MainWindow.h"
#include "RemoteDesktopView.h"
#include "ConnectionDialog.h"
#include "RemoteClient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , remoteClient(new RemoteClient(this))
{
    setupUi();
    setupConnections();
    setupRemoteClient();

    appendLog("AIKong Modern Remote Admin 已启动");
    appendLog("警告：请确保您已获得目标系统的明确授权");
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    setWindowTitle("AIKong - Qt + Rust 现代化远程管理");
    resize(1400, 900);

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // 远程桌面标签页
    QWidget *remoteTab = new QWidget();
    QVBoxLayout *remoteLayout = new QVBoxLayout(remoteTab);
    remoteView = new RemoteDesktopView();
    remoteLayout->addWidget(remoteView);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    captureBtn = new QPushButton("捕获远程屏幕");
    btnLayout->addWidget(captureBtn);
    remoteLayout->addLayout(btnLayout);
    tabWidget->addTab(remoteTab, "远程桌面");

    // 命令控制
    QWidget *cmdTab = new QWidget();
    QVBoxLayout *cmdLayout = new QVBoxLayout(cmdTab);
    commandLineEdit = new QLineEdit();
    commandLineEdit->setPlaceholderText("输入命令...");
    execBtn = new QPushButton("执行命令");
    QHBoxLayout *cmdBtnLayout = new QHBoxLayout();
    cmdBtnLayout->addWidget(commandLineEdit);
    cmdBtnLayout->addWidget(execBtn);
    cmdLayout->addLayout(cmdBtnLayout);
    cmdLayout->addStretch();
    tabWidget->addTab(cmdTab, "命令控制");

    // 日志
    logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    tabWidget->addTab(logTextEdit, "日志");

    // 工具栏
    connectBtn = new QPushButton("连接远程");
    disconnectBtn = new QPushButton("断开连接");
    disconnectBtn->setEnabled(false);

    QToolBar *toolBar = addToolBar("Main");
    toolBar->addWidget(connectBtn);
    toolBar->addWidget(disconnectBtn);

    statusBar()->showMessage("就绪");
}

void MainWindow::setupConnections()
{
    connect(connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(disconnectBtn, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    connect(captureBtn, &QPushButton::clicked, this, &MainWindow::onCaptureScreenClicked);
    connect(execBtn, &QPushButton::clicked, this, &MainWindow::onExecuteCommandClicked);
}

void MainWindow::setupRemoteClient()
{
    connect(remoteClient, &RemoteClient::connected, this, [this]() {
        appendLog("已连接到 Rust 服务器");
        isConnected = true;
        connectBtn->setEnabled(false);
        disconnectBtn->setEnabled(true);
    });

    connect(remoteClient, &RemoteClient::disconnected, this, [this]() {
        appendLog("已断开 Rust 服务器");
        isConnected = false;
        connectBtn->setEnabled(true);
        disconnectBtn->setEnabled(false);
    });

    connect(remoteClient, &RemoteClient::captureReceived, this, [this](const QImage &img) {
        remoteView->updateFrame(img);
        appendLog("已接收到远程屏幕图像");
    });

    connect(remoteClient, &RemoteClient::commandResult, this, [this](const QString &result) {
        appendLog("命令结果: " + result);
    });

    connect(remoteClient, &RemoteClient::errorOccurred, this, [this](const QString &err) {
        appendLog("错误: " + err);
    });
}

void MainWindow::onConnectClicked()
{
    ConnectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString host = dialog.getHost();
        quint16 port = dialog.getPort();

        appendLog(QString("正在连接 %1:%2 ...").arg(host).arg(port));
        remoteClient->connectToServer(host, port);
    }
}

void MainWindow::onDisconnectClicked()
{
    remoteClient->disconnectFromServer();
}

void MainWindow::onCaptureScreenClicked()
{
    if (!isConnected) {
        QMessageBox::warning(this, "警告", "请先连接到 Rust 服务器");
        return;
    }
    appendLog("正在请求远程屏幕...");
    remoteClient->requestCapture();
}

void MainWindow::onExecuteCommandClicked()
{
    QString cmd = commandLineEdit->text().trimmed();
    if (cmd.isEmpty() || !isConnected) return;

    appendLog("执行命令: " + cmd);
    remoteClient->sendCommand(cmd);
    commandLineEdit->clear();
}

void MainWindow::appendLog(const QString &msg)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    logTextEdit->append(QString("[%1] %2").arg(time, msg));
}