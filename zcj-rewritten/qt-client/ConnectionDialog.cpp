#include "ConnectionDialog.h"
#include <QLabel>
#include <QIntValidator>

ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("连接到远程目标");
    setModal(true);
    resize(400, 200);

    QFormLayout *form = new QFormLayout(this);

    hostEdit = new QLineEdit("127.0.0.1");
    portEdit = new QLineEdit("8443");
    portEdit->setValidator(new QIntValidator(1, 65535, this));
    tokenEdit = new QLineEdit();
    tokenEdit->setPlaceholderText("认证 Token / 密码 (可选)");
    tokenEdit->setEchoMode(QLineEdit::Password);

    form->addRow(new QLabel("主机/IP:"), hostEdit);
    form->addRow(new QLabel("端口:"), portEdit);
    form->addRow(new QLabel("认证 Token:"), tokenEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    form->addRow(buttons);
}

QString ConnectionDialog::getHost() const { return hostEdit->text().trimmed(); }
quint16 ConnectionDialog::getPort() const { return portEdit->text().toUShort(); }
QString ConnectionDialog::getToken() const { return tokenEdit->text(); }