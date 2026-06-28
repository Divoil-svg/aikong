#include "RemoteDesktopView.h"
#include <QPainter>

RemoteDesktopView::RemoteDesktopView(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800, 600);
    setStyleSheet("background-color: #1e1e1e;");
}

void RemoteDesktopView::updateFrame(const QImage &image)
{
    if (!image.isNull()) {
        currentFrame = image;
    } else {
        // 示例：绘制一个占位图
        currentFrame = QImage(size(), QImage::Format_RGB32);
        currentFrame.fill(Qt::darkGray);
    }
    update(); // 触发重绘
}

void RemoteDesktopView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!currentFrame.isNull()) {
        QImage scaled = currentFrame.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int x = (width() - scaled.width()) / 2;
        int y = (height() - scaled.height()) / 2;
        painter.drawImage(x, y, scaled);
    } else {
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "远程桌面视图\n\n请点击“捕获屏幕”获取画面");
    }
}