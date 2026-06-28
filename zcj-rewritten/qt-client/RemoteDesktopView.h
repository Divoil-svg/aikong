#ifndef REMOTEDESKTOPVIEW_H
#define REMOTEDESKTOPVIEW_H

#include <QWidget>
#include <QImage>
#include <QPainter>

class RemoteDesktopView : public QWidget
{
    Q_OBJECT
public:
    explicit RemoteDesktopView(QWidget *parent = nullptr);

    void updateFrame(const QImage &image = QImage());

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage currentFrame;
};

#endif // REMOTEDESKTOPVIEW_H