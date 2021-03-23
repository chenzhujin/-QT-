#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <student.h>
#include <QSqlError>
#include <QMessageBox>

student *w;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    w = new student(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QFile  a("admin.txt");

    if(a.open(QIODevice::ReadWrite|QIODevice::NewOnly))
    {
        qDebug() << "ok";

        QString  code=ui->lineEdit->text();
        QString  pwd =ui->lineEdit_2->text();

        QString  all = QString("%1 %2").arg(code).arg(pwd);

        a.write(all.toUtf8());
        a.close();
        qDebug() << "登陆成功";
        w->show();
        this->hide();
    }
    else {
      qDebug() << "fail";

       if(a.open(QIODevice::ReadWrite))
       {
           QString all = a.readAll();

           QString  code=ui->lineEdit->text();
           QString  pwd =ui->lineEdit_2->text();

           QString  all1 = QString("%1 %2").arg(code).arg(pwd);

           if(all1 == all)
           {
               qDebug() << "登陆成功2";
               w->show();
               this->hide();
           }
           else
           {
               qDebug() << "账号密码错误";
               QMessageBox::warning(this, tr("登录信息"),
                                              tr("账号或密码错误！"),
                                              QMessageBox::Ok);
           }

       }else {

           qDebug() << "fail2";
        }
    }
}
