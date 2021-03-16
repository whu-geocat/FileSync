#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QLabel>

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

#define MAX_STREAM_SIZE 2097152     // 最大的流的字节大小：2 M
#define MAX_FILENAME_LEN 512

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    void traverseAllFiles(QString& sDir);

private slots:
    void on_pushButton_Connect_clicked();
    void on_toolButton_ChooseDir_clicked();
    void on_pushButton_Start_clicked();

protected:
    void sendFile();

private:
    Ui::MainWidget *ui;

    QStringList m_qslFileNames;
    bool m_bConnected = false;
    QString m_qsDir;

    // 数据流
    char* m_pDataBuf = nullptr;

#ifdef _WIN64
    SOCKET m_ClientSocket;
    SOCKET m_ServerSocket;
    WSADATA m_WSADATA;
#endif
#ifdef linux
    int m_ClientSocket;
#endif
};

namespace Util {
    bool ipIllegal(const char* pIP);
    bool ipIllegal(std::string sIP);
}

#endif // MAINWIDGET_H
