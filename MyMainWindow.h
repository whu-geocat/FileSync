#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>

#include "MainWidget.h"
#include "MainServerWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyMainWindow; }
QT_END_NAMESPACE

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyMainWindow(QWidget *parent = nullptr);
    ~MyMainWindow();

private slots:
    void on_pushButton_AsServer_clicked();
    void on_pushButton_AsClient_clicked();

    void on_action_SetBackGround_triggered();

private:
    Ui::MyMainWindow *ui;

    MainWidget* m_pClient = nullptr;
    MainServerWidget* m_pServer = nullptr;
};
#endif // MYMAINWINDOW_H
