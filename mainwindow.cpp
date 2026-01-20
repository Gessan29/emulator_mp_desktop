#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QUdpSocket>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , messageCounter(0)
{
    ui->setupUi(this);

    UDP_init(); // инициализация UDP сокета
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UDP_init(){
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 0);
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::onDataReceived);
}

void MainWindow::on_Information_triggered()
{
    QMessageBox::information(this, "О программе", "Режим работы: UDP, IPv4");
}

void MainWindow::on_Send_butt_clicked()
{
    QString ipAddress = ui->IP_line->text();
    QString port = ui->Port_line->text();
    QString message = ui->ASCII_line->text();

    if(ipAddress.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Введите IP-адрес!");
        return;
    }
    if(port.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Введите порт!");
        return;
    }
    if(message.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Введите сообщение!");
        return;
    }

    bool ok;
    quint16 portNum = port.toUShort(&ok);
    if (!ok || portNum == 0){
        QMessageBox::warning(this, "Ошибка!", "Недопустимый порт!");
        return;
    }

    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString hexData;
    for (char c : message.toUtf8()){
        hexData += QString("%1 ").arg((unsigned char)c, 2, 16, QChar('0')).toUpper();
    }
    hexData = hexData.trimmed();

    QByteArray datagram = message.toUtf8();
    udpSocket->writeDatagram(datagram, QHostAddress(ipAddress), portNum);

    QString localIP = "You";
    QString localPost = QString::number(udpSocket->localPort());

    addTableRow(currentTime, localIP, localPost,
                ipAddress, port, message, hexData);
}

void MainWindow::onDataReceived(){
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress senderAddress;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        QString message = QString::fromUtf8(datagram);
        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

        QString hexData;
        for (unsigned char c : datagram){
            hexData += QString("%1 ").arg(c, 2, 16, QChar('0')).toUpper();
        }
        hexData = hexData.trimmed();

        QString localIP = "You";
        QString ourPort = QString::number(udpSocket->localPort());

        addTableRow(currentTime, senderAddress.toString(), QString::number(senderPort),
                    localIP, ourPort, message, hexData);
    }
}

void MainWindow::addTableRow(const QString &time, const QString &fromIP, const QString &fromPort,
                             const QString &toAddress, const QString &toPort,
                             const QString &ascii, const QString &hex){

    int row = ui->Log->rowCount();
    ui->Log->insertRow(row);

    ui->Log->setItem(row, 0, new QTableWidgetItem(time));
    ui->Log->setItem(row, 1, new QTableWidgetItem(fromIP));
    ui->Log->setItem(row, 2, new QTableWidgetItem(fromPort));
    ui->Log->setItem(row, 3, new QTableWidgetItem(toAddress));
    ui->Log->setItem(row, 4, new QTableWidgetItem(toPort));
    ui->Log->setItem(row, 5, new QTableWidgetItem(ascii));
    ui->Log->setItem(row, 6, new QTableWidgetItem(hex));

}

void MainWindow::on_Clear_butt_clicked()
{
    ui->Log->setRowCount(0);
    messageCounter = 0;
}

