#ifndef Connect_HPP
#define Connect_HPP

#include "ui_ipconnect.h"
#include "ui_comconnect.h"
#include <QWidget>
#include <QtWidgets>
#include <QTcpSocket>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>
#include <QTimer>
#include <queue>

QT_BEGIN_NAMESPACE
namespace Ui { class IP; }
namespace Ui { class COM; }
QT_END_NAMESPACE

class Connect : public QWidget
{
    Q_OBJECT
protected:
    bool Cn;
    QString Answer;

    virtual void showEvent(QShowEvent* event) = 0;

public:
    explicit Connect(QWidget *parent = nullptr);
    virtual void open() = 0;
    virtual void close(bool ui = false) = 0;
    virtual void send(QString C) = 0;
    virtual QString enter() = 0;

    bool isConnect();
signals:
    void Connected();
    void Disconnected();

};

class IPConnect: public Connect
{
    Q_OBJECT
private:
    QString Host_IP;
    int Host_port;
    QTcpSocket *S;
    Ui::IP *ui;

    void showEvent(QShowEvent* event);

public:
    IPConnect(QString IP, int port);
    ~IPConnect();
    void host(QString IP, int port);
    void open();
    void close(bool ui = false);
    void send(QString C);
    QString enter();

private slots:
    void read();
    void swap_Connect();
};

class COMConnect: public Connect
{
    Q_OBJECT
private:
    QString port_Name;
    int port_baud;
    QSerialPort* P;
    Ui::COM *ui;

    void showEvent(QShowEvent* event);

public:
    const QByteArray QStrToQByte(QString A);
    COMConnect(int baud);
    ~COMConnect();
    void open();
    void close(bool ui = false);
    void send(QString send);
    QString enter();



private slots:
    void read();
    void swap_Connect();
    void Update_COM();
    void setPort();
    void setBaud();

};

#endif // Connect_HPP
