#ifndef MAINSERVERWIDGET_H
#define MAINSERVERWIDGET_H

#include <QWidget>
#include <QFileDialog>

#include <iostream>
#include <fstream>
#include <thread>

#ifdef WIN64
#include <WinSock2.h>
#include <WS2tcpip.h>
#elif linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpainet/inet.h>
#else
#...
#endif

#define MAX_FILENAME_LEN 512

namespace Ui {
class MainServerWidget;
}

class MainServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainServerWidget(QWidget *parent = nullptr);
    ~MainServerWidget();

private slots:
    void on_toolButton_ChoseDir_clicked();

protected:
    void initServer();
    void startRecv();

private:
    Ui::MainServerWidget *ui;

#ifdef _WIN64
    SOCKET m_ClientSocket;
    SOCKET m_ServerSocket;
    WSADATA m_WSADATA;
#endif
#ifdef linux
    int m_ClientSocket;
#endif
};

#endif // MAINSERVERWIDGET_H
