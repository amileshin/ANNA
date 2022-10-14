#include "connect.hpp"

Connect::Connect(QWidget *parent) : QWidget(parent)
{
    this->Cn = false;
}

IPConnect::IPConnect(QString IP, int port) : Host_IP(IP), Host_port(port)
{
    this->ui = new Ui::IP();
    this->ui->setupUi(this);
    this->Cn = false;
    this->S = new QTcpSocket(this);
    this->S->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    this->ui->IPE->setText(this->Host_IP);
    this->ui->PE->setText(QString::number(this->Host_port));
    connect(this->S, &QTcpSocket::readyRead, this, &IPConnect::read);
    connect(this->ui->CB, &QPushButton::clicked, this, &IPConnect::swap_Connect);
}

IPConnect::~IPConnect()
{
    delete this->ui;
    this->close();
    delete this->S;
}

bool Connect::isConnect()
{
    return this->Cn;
}

void IPConnect::showEvent(QShowEvent* event)
{
    event->accept();
}

void IPConnect::open()
{
    if (!this->Cn)
    {
        this->S->connectToHost(this->Host_IP, this->Host_port);
        if (S->waitForConnected(1000))
        {
            this->Cn = true;
            this->ui->CHB->append("Connect sucsses. Host [" + Host_IP + ", " + QString::number(Host_port) + "]\n");
            this->Connected();
        }
        else
            this->ui->CHB->append("Connect failed. Host [" + Host_IP + ", " + QString::number(Host_port) + "]\n");
    }
}

void IPConnect::close(bool ui)
{
    if (this->Cn)
    {
        this->S->disconnectFromHost();
        this->Disconnected();
        this->Cn = false;
        if (ui)
            this->ui->CHB->append("Dissconnect from Host");
    }
}

void IPConnect::host(QString IP, int port)
{
    this->Host_IP = IP;
    this->Host_port = port;
}

void IPConnect::send(QString C)
{
    this->S->write(C.toLocal8Bit());
    this->S->waitForBytesWritten(1000);
    this->ui->CHB->append("<-::" + QTime::currentTime().toString("hh:mm:ss") + ' '+ C);
    QThread::msleep(10);
}

QString IPConnect::enter()
{
    if (this->Answer == "")
        S->waitForReadyRead(1000);
    QString A = this->Answer;
    this->Answer = "";
    return A;
}

void IPConnect::read()
{
    Answer = QString::fromLocal8Bit(S->readAll());
    ui->CHB->append("->::" + QTime::currentTime().toString("hh:mm:ss") + ' ' + Answer);
}

void IPConnect::swap_Connect()
{
    ui->CB->setEnabled(false);
    if (Cn)
        close();
    else
    {
        host(ui->IPE->text(), ui->PE->text().toInt());
        open();
    }
    ui->CB->setEnabled(true);
}

COMConnect::COMConnect(int baud) : port_baud(baud)
{
    port_Name = "";
    this->ui = new Ui::COM();
    this->ui->setupUi(this);
    this->Cn = false;
    this->P = new QSerialPort(this);
    this->Update_COM();
    switch (port_baud)
    {
    default:
        ui->BBC->setCurrentIndex(-1);
        port_baud = 0;
        break;
    case (1200):
        ui->BBC->setCurrentIndex(0);
        break;
    case (2400):
        ui->BBC->setCurrentIndex(1);
        break;
    case (4800):
        ui->BBC->setCurrentIndex(2);
        break;
    case (9600):
        ui->BBC->setCurrentIndex(3);
        break;
    case (19200):
        ui->BBC->setCurrentIndex(4);
        break;
    case (38400):
        ui->BBC->setCurrentIndex(5);
        break;
    case (115200):
        ui->BBC->setCurrentIndex(6);
        break;
    }
    connect(this->P, &QSerialPort::readyRead, this, &COMConnect::read);
    connect(this->ui->PBC, &QComboBox::currentTextChanged, this, &COMConnect::setPort);
    connect(this->ui->BBC, &QComboBox::currentTextChanged, this, &COMConnect::setBaud);
    connect(this->ui->CB, &QPushButton::clicked, this, &COMConnect::swap_Connect);
    //connect(this, QWidget::showEvent, this, &COMConnect::Update_COM);
}

COMConnect::~COMConnect()
{
    delete this->ui;
    this->close();
    delete this->P;
}

void COMConnect::showEvent(QShowEvent* event)
{
    this->Update_COM();
    event->accept();
}


void COMConnect::open()
{
    if (!this->Cn)
    {
        P->setBaudRate(port_baud);
        P->setPortName(port_Name);
        P->setDataBits(P->Data8);
        if (P->open(QIODevice::ReadWrite))
        {
            this->Cn = true;
            this->ui->CHB->append("Connect sucsses. Port " + port_Name + ", " + QString::number(port_baud) + "\n");
            this->Connected();
        }
        else
            this->ui->CHB->append("Connect failed. Port " + port_Name + ", " + QString::number(port_baud) + "\n");
    }
}

void COMConnect::close(bool ui)
{
    if (this->Cn)
    {
        P->close();
        this->ui->CHB->append("Dissconnect from Host");
        this->Disconnected();
        this->Cn = false;
        if (ui)
            this->ui->CHB->append("Dissconnect from Host");
    }
}

void COMConnect::send(QString send)
{
    if (Cn)
    {
        QString Wr;
        QByteArray A = COMConnect::QStrToQByte(send);
        P->write(A);
        P->waitForBytesWritten();
        for (uchar i : A)
        {
            Wr += QString::number(i, 16) + ' ';
        }
        this->ui->CHB->append("<-::" + QTime::currentTime().toString("hh:mm:ss") + ' ' + Wr);
    }
}

QString COMConnect::enter()
{
    if (Cn)
    {
        if (this->Answer == "")
            P->waitForReadyRead(1000);
        QString A = this->Answer;
        this->Answer = "";
        return A;
    }
    return "";
}

void COMConnect::read()
{
    char A[1];
    P->read(A, 1);
    Answer = QString::number(A[0]);
    ui->CHB->append("->::" + QTime::currentTime().toString("hh:mm:ss") + ' ' + Answer);

}

void COMConnect::Update_COM()
{
    ui->PBC->clear();
    const auto COMs = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &COM: COMs)
        ui->PBC->addItem(COM.portName());
    ui->PBC->setCurrentIndex(0);
}

void COMConnect::swap_Connect()
{
    ui->CB->setEnabled(false);
    if (Cn)
        close();
    else
    {
        port_Name = ui->PBC->currentText();
        port_baud = ui->BBC->currentText().toInt();
        open();
    }
    ui->CB->setEnabled(true);
}

void COMConnect::setPort()
{
    port_Name = ui->PBC->currentText();
}


void COMConnect::setBaud()
{
    port_baud = ui->PBC->currentText().toInt();
}

const QByteArray COMConnect::QStrToQByte(QString A)
{
    QByteArray Ans;
    for (QChar i : A)
        Ans.push_back(i.cell());
    return Ans;
}