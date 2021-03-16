#include "MainWidget.h"
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 申请内存
    m_pDataBuf = new char[MAX_STREAM_SIZE];
}

MainWidget::~MainWidget()
{
    delete ui;

#ifdef _WIN64
    closesocket(m_ClientSocket);
    closesocket(m_ServerSocket);
#endif
#ifdef linux
#endif
}

void MainWidget::traverseAllFiles(QString &sDir)
{
    QFileInfoList infoList = QDir(sDir).entryInfoList();
    for(auto info: infoList)
    {
        if(info.isFile())
        {
            m_qslFileNames.push_back(info.filePath());
        }
        else if(info.isDir() && info.fileName() != "." && info.fileName() != "..")
        {
            QString sSubDir = info.filePath();
            traverseAllFiles(sSubDir);
        }
    }
}

void MainWidget::on_pushButton_Connect_clicked()
{// 连接服务器
    std::string sIpAddr = ui->lineEdit_IP->text().toStdString();
    if(sIpAddr.empty())
    {
        ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: IP地址为空，请输入正确的IP地址."));
        return;
    }
    if(! Util::ipIllegal(sIpAddr))
    {
        ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: IP地址不合法."));
        return;
    }
    int iPort = 0;
    iPort = ui->lineEdit_Port->text().toInt();
    if(0 == iPort)
    {
        ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: 端口号不能为0."));
        return;
    }

    // IP 和 端口号都设置正确之后，可以点击按钮连接服务器
    // 连接成功，则输出信息，按钮背景色变为绿色

#ifdef _WIN64
    WSAStartup(MAKEWORD(2, 2), &m_WSADATA);
    m_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);   // 创建套接字

    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(iPort);
    inet_pton(AF_INET, sIpAddr.c_str(), &servAddr.sin_addr.S_un.S_addr);

    int iConnRes = ::connect(m_ClientSocket, (struct sockaddr*) &servAddr, sizeof (struct sockaddr));
    if(iConnRes < 0)
    {
        ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: 连接服务器失败."));
        ui->pushButton_Connect->setText(QString::fromLocal8Bit("连接失败"));
        ui->pushButton_Connect->setStyleSheet("background: rgba(255, 0, 0, 125);");
        m_bConnected = false;
        return;
    }

    ui->textEdit->append(QString::fromLocal8Bit("============: 连接成功."));
    ui->pushButton_Connect->setText(QString::fromLocal8Bit("已连接"));
    ui->pushButton_Connect->setStyleSheet("background: rgba(0, 255, 0, 255);");
    m_bConnected = true;
    return;

#endif
#ifdef linux
    .....
#endif
}

void MainWidget::on_toolButton_ChooseDir_clicked()
{// 选择文件夹
    m_qsDir = QFileDialog::getExistingDirectory(0, QString::fromLocal8Bit("选择同步文件夹"), "./");
    m_qslFileNames.clear();
    traverseAllFiles(m_qsDir);

    // ===Debug===
    // 打印所有的文件
    for(auto file: m_qslFileNames)
    {
        ui->textEdit->append(file);
    }
    // ===Debug===

    ui->lineEdit_Dir->setText(m_qsDir);
}

void MainWidget::on_pushButton_Start_clicked()
{// 开始同步
    std::thread th(& MainWidget::sendFile, this);
    th.join();
}

void MainWidget::sendFile()
{
    if(m_bConnected == false)
    {
        ui->textEdit->append(QString::fromLocal8Bit("***ERROR***: 无法连接到服务器."));
        return;
    }

    // 读文件，然后字节发送到服务器
    for(auto file: m_qslFileNames)
    {
        // 设置进度条
        ui->textEdit->append(QString::fromLocal8Bit("正在发送:<") + file + ">...");
//        ui->label_ProgressFileName->setText(QString::fromLocal8Bit("正在发送:<") + file + ">...");
//        m_pLabel->show();

        std::string sFilePath = file.toStdString();
        std::ifstream inFile(sFilePath, std::ios::in | std::ios::binary);
        inFile.seekg(0, std::ios::end); // 跳到文件尾
        int iFileSize = inFile.tellg(); // 文件大小
        inFile.seekg(0, std::ios::beg); // 跳到文件头

        int iPackSize = MAX_FILENAME_LEN + sizeof (int) + iFileSize;
        if(iPackSize > MAX_STREAM_SIZE)
        {
            delete [] m_pDataBuf;
            m_pDataBuf = new char[iPackSize];
        }
        inFile.read((char*)(m_pDataBuf + MAX_FILENAME_LEN + sizeof (int)), iFileSize);
        inFile.close();

        // 发送文件设置
            // 设置文件名
        std::string sDirName = m_qsDir.toStdString();
        sDirName = sDirName.substr(sDirName.find_last_of("/") + 1);
        std::string sFileName = file.toStdString().substr(m_qsDir.length() - sDirName.length());
        int iMinLen = MAX_FILENAME_LEN < sFileName.length() ? MAX_FILENAME_LEN : sFileName.length();
        for(int i = 0; i < iMinLen; i++)
            m_pDataBuf[i] = sFileName[i];
            // 设置文件大小
        *((int*)(m_pDataBuf + MAX_FILENAME_LEN)) = iPackSize;

        int iHaveSent = 0;
        while (iPackSize > 0) {
            int iSendLen = ::send(m_ClientSocket, (char*)(m_pDataBuf + iHaveSent), iPackSize, 0);

            iPackSize -= iSendLen;
            iHaveSent += iSendLen;
        }
        ui->textEdit->append(QString::fromLocal8Bit("文件：<") + file + QString::fromLocal8Bit("> 发送成功。"));
    }
}

bool Util::ipIllegal(const char *pIP)
{
    return true;
}

bool Util::ipIllegal(std::string sIP)
{
    return true;
}
