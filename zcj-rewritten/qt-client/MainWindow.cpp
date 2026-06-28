#include "MainWindow.h"
#include "RemoteDesktopView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();
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

    // === 远程桌面标签页 ===
    QWidget *remoteTab = new QWidget();
    QVBoxLayout *remoteLayout = new QVBoxLayout(remoteTab);

    remoteView = new RemoteDesktopView();
    remoteLayout->addWidget(remoteView);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    captureBtn = new QPushButton("捕获屏幕");
    btnLayout->addWidget(captureBtn);
    remoteLayout->addLayout(btnLayout);

    tabWidget->addTab(remoteTab, "远程桌面");

    // === 命令控制标签页 ===
    QWidget *cmdTab = new QWidget();
    QVBoxLayout *cmdLayout = new QVBoxLayout(cmdTab);

    commandLineEdit = new QLineEdit();
    commandLineEdit->setPlaceholderText("输入命令并回车执行...");
    execBtn = new QPushButton("执行命令");

    QHBoxLayout *cmdBtnLayout = new QHBoxLayout();
    cmdBtnLayout->addWidget(commandLineEdit);
    cmdBtnLayout->addWidget(execBtn);

    cmdLayout->addLayout(cmdBtnLayout);
    cmdLayout->addStretch();

    tabWidget->addTab(cmdTab, "命令控制");

    // === 日志标签页 ===
    logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    tabWidget->addTab(logTextEdit, "日志");

    // === 工具栏 ===
    connectBtn = new QPushButton("连接远程");
    disconnectBtn = new QPushButton("断开连接");
    disconnectBtn->setEnabled(false);

    QToolBar *toolBar = addToolBar("Main");
    toolBar->addWidget(connectBtn);
    toolBar->addWidget(disconnectBtn);
}

void MainWindow::setupConnections()
{
    connect(connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(disconnectBtn, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    connect(captureBtn, &QPushButton::clicked, this, &MainWindow::onCaptureScreenClicked);
    connect(execBtn, &QPushButton::clicked, this, &MainWindow::onExecuteCommandClicked);
}

void MainWindow::onConnectClicked()
{
    isConnected = true;
    connectBtn->setEnabled(false);
    disconnectBtn->setEnabled(true);
    appendLog("已连接到远程目标 (Rust 后端通信已建立)");
    // TODO: 调用 Rust FFI 建立连接
}

void MainWindow::onDisconnectClicked()
{
    isConnected = false;
    connectBtn->setEnabled(true);
    disconnectBtn->setEnabled(false);
    appendLog("已断开连接");
}

void MainWindow::onCaptureScreenClicked()
{
    if (!isConnected) {
        QMessageBox::warning(this, "警告", "请先连接远程");
        return;
    }
    appendLog("正在请求屏幕捕获...");
    // TODO: 通过 Rust capture 模块获取并显示在 remoteView
    remoteView->updateFrame(); // 示例
}

void MainWindow::onExecuteCommandClicked()
{
    QString cmd = commandLineEdit->text().trimmed();
    if (cmd.isEmpty()) return;

    appendLog("执行命令: " + cmd);
    // TODO: 发送到 Rust 后端执行
    commandLineEdit->clear();
}

void MainWindow::appendLog(const QString &msg)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    logTextEdit->append(QString("[%1] %2").arg(time, msg));
}