#include "MyMainWindow.h"
#include "ui_MyMainWindow.h"

MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}


void MyMainWindow::on_pushButton_AsServer_clicked()
{
    m_pServer = new MainServerWidget(this);
    this->setCentralWidget(m_pServer);
}

void MyMainWindow::on_pushButton_AsClient_clicked()
{
    m_pClient = new MainWidget(this);
    this->setCentralWidget(m_pClient);
}

void MyMainWindow::on_action_SetBackGround_triggered()
{
    QString sBgPic = QFileDialog::getOpenFileName(this, "选择背景图片", "");

    QPixmap pixmap(sBgPic);
    // 设置主窗口背景
    this->setAutoFillBackground(true);
    QPalette pale;
    pale.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(pale);
}
