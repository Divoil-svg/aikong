#include "MainWindow.h"
#include "RemoteDesktopView.h"
#include "ConnectionDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDateTime>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>

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
    captureBtn = new QPushButton("捕获屏幕 (Demo)");
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

    // 状态栏
    statusBar()->showMessage("就绪 - 请点击“连接远程”");
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
    ConnectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString host = dialog.getHost();
        quint16 port = dialog.getPort();
        QString token = dialog.getToken();

        // TODO: 使用 host:port + token 调用 Rust 网络层建立真实连接
        appendLog(QString("正在连接 %1:%2 ...").arg(host).arg(port));
        if (!token.isEmpty()) {
            appendLog("已提供认证 Token");
        }

        isConnected = true;
        connectBtn->setEnabled(false);
        disconnectBtn->setEnabled(true);
        statusBar()->showMessage(QString("已连接到 %1:%2").arg(host).arg(port));
        appendLog("连接成功（模拟模式 - Rust 后端待实现）");
    }
}

void MainWindow::onDisconnectClicked()
{
    isConnected = false;
    connectBtn->setEnabled(true);
    disconnectBtn->setEnabled(false);
    statusBar()->showMessage("已断开");
    appendLog("已断开连接");
}

void MainWindow::onCaptureScreenClicked()
{
    if (!isConnected) {
        QMessageBox::warning(this, "警告", "请先连接远程");
        return;
    }

    appendLog("正在捕获屏幕...");

    // Demo: 捕获当前主机屏幕（实际应从远程获取）
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QPixmap pixmap = screen->grabWindow(0);
        QImage image = pixmap.toImage();
        remoteView->updateFrame(image);
        appendLog(QString("屏幕捕获成功 - 尺寸: %1x%2").arg(image.width()).arg(image.height()));
    } else {
        appendLog("无法获取屏幕");
    }

    // TODO: 通过 Rust capture 模块从真正的远程目标获取
}

void MainWindow::onExecuteCommandClicked()
{
    QString cmd = commandLineEdit->text().trimmed();
    if (cmd.isEmpty()) return;

    appendLog("执行命令: " + cmd);
    // TODO: 发送到 Rust 后端执行并显示结果
    commandLineEdit->clear();
}

void MainWindow::appendLog(const QString &msg)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    logTextEdit->append(QString("[%1] %2").arg(time, msg));
}