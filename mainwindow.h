#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Information_triggered();

    void on_Send_butt_clicked();

    void on_Clear_butt_clicked();

    void UDP_init(); // создание объекта и инициализация udp-сокета

    void onDataReceived();

    void addTableRow(const QString &time, const QString &fromIP, const QString &fromPort,
                     const QString &toAddress, const QString &toPort,
                     const QString &ascii, const QString &hex);
private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    int messageCounter;
};
#endif // MAINWINDOW_H
