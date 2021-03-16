#include "MainServerWidget.h"
#include "ui_MainServerWidget.h"

MainServerWidget::MainServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainServerWidget)
{
    ui->setupUi(this);

    // 构造的时候加载提示信息
    ui->lineEdit_ClientIP->setText(QString::fromLocal8Bit("暂无连接"));
    ui->lineEdit_Dir->setText(QString::fromLocal8Bit("请选择文件夹"));
    ui->textEdit->append(QString::fromLocal8Bit("***注意！！！***\n在选择了同步文件夹之后，程序会自动开启服务器监听，等待客户端连接。\n***注意！！！***\n"));
}

MainServerWidget::~MainServerWidget()
{
    delete ui;
}

void MainServerWidget::on_toolButton_ChoseDir_clicked()
{
    QString sDir = QFileDialog::getExistingDirectory(0, QString::fromLocal8Bit("选择同步文件夹"), "./");
    ui->lineEdit_Dir->setText(sDir);

    // 开启服务器监听
    ui->textEdit->append(QString::fromLocal8Bit("===== 正在开启服务器..."));

    std::thread th(& MainServerWidget::initServer, this);
    th.detach();
}

void MainServerWidget::initServer()
{
    WSAStartup(MAKEWORD(2, 2), &m_WSADATA);

    // 创建服务器套接字
    m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

    // 服务器地址
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
#define DEFAULT_PORT 1314
    servAddr.sin_port = htons(DEFAULT_PORT);
    servAddr.sin_addr.S_un.S_addr = INADDR_ANY;

    // 绑定套接字
    int iBindRes = bind(m_ServerSocket, (sockaddr*) &servAddr, sizeof (sockaddr));
    if(iBindRes < 0)
    {
        ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: 绑定套接字失败"));
        return;
    }
    else
        ui->textEdit->append(QString::fromLocal8Bit("===== 绑定套接字成功"));

    int iListenRes = listen(m_ServerSocket, 2);
    if(iListenRes < 0)
    {
        ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: 监听套接字失败"));
        return;
    }
    else
        ui->textEdit->append(QString::fromLocal8Bit("===== 监听套接字成功"));

    sockaddr_in clientAddr;
    int iLen = sizeof (clientAddr);
    m_ClientSocket = accept(m_ServerSocket, (sockaddr*) &clientAddr, &iLen);

    char cIP[16] = {'\0'};
    ui->textEdit->append(QString::fromLocal8Bit("客户端IP为：") + inet_ntop(AF_INET, &clientAddr.sin_addr, cIP, 16));
    ui->lineEdit_ClientIP->setText(cIP);

    startRecv();
}

void MainServerWidget::startRecv()
{
    while(true)
    {
        // 接收文件头
        char cFilename[MAX_FILENAME_LEN] = {'\0'};
        int iRecvLen = recv(m_ClientSocket, cFilename, MAX_FILENAME_LEN, 0);
        if(iRecvLen != MAX_FILENAME_LEN)
        {
            ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: 数据流出错"));
            return;
        }
        int iPackSize = 0;
        iRecvLen = recv(m_ClientSocket, (char*)&iPackSize, sizeof (int), 0);
        if(iRecvLen != sizeof (int))
        {
            ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: 数据流出错"));
            return;
        }
        int iFileSize = iPackSize - MAX_FILENAME_LEN - sizeof (int);
        unsigned char * pDataBuf = new unsigned char[iFileSize];
        int iWriteLen = 0;
        while (iFileSize > 0) {
            iRecvLen = recv(m_ClientSocket, (char*)(pDataBuf + iWriteLen), iFileSize, 0);

            iFileSize -= iRecvLen;
            iWriteLen += iRecvLen;
        }
        // 判断路径是否存在
        QString sDir = ui->lineEdit_Dir->text();
        QString sFileName = sDir + "/" + cFilename;

        QFileInfo info(sFileName);
        QDir dir(info.path());
        if(! dir.exists())
            dir.mkpath(info.path());    // 自动创建全路径
        // 写入文件
        std::ofstream outFile(sFileName.toStdString(), std::ios::out | std::ios::binary);
        outFile.write((char*) pDataBuf, iWriteLen);
        outFile.close();
        delete [] pDataBuf;

        ui->textEdit->append(QString::fromLocal8Bit("文件：") +
                             QString::fromLocal8Bit(cFilename) +
                             QString::fromLocal8Bit("> 接收成功。"));
    }
}
