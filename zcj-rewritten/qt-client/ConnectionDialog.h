#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QDialogButtonBox>

class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionDialog(QWidget *parent = nullptr);

    QString getHost() const;
    quint16 getPort() const;
    QString getToken() const;

private:
    QLineEdit *hostEdit;
    QLineEdit *portEdit;
    QLineEdit *tokenEdit;
};

#endif // CONNECTIONDIALOG_H