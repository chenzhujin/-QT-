#include "student.h"
#include "ui_student.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QTableWidgetItem>

QSqlDatabase db;
QSqlQuery *sql;
int row=0; //数据库中的行数

student::student(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::student)
{
    ui->setupUi(this);

   db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("./stu.db");
   if(db.open()){
        qDebug() << "打开数据库成功";

     sql  = new QSqlQuery(db);

     QString cmd = "create table stu(id primary key,name,class,chinese,math,english);";
    if(sql->exec(cmd)){
            qDebug() << "创建表格成功";
    }else{
            qDebug() << "创建表格失败" << sql->lastError().text();
    }
   }
   else{
       qDebug() << "打开数据库失败";
   }

    this->get_sql_row();

   //设置表格中的行数
   ui->tableWidget->setRowCount(row);


    //显示表格中的数据
     this->show_table();


    //隐藏表头
    ui->tableWidget->verticalHeader()->setVisible(false);

    //不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //自适应
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }");

    qDebug() << "数据项数" <<row;

    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int )),this, SLOT(sortByColumn(int)));
}

student::~student()
{
    delete ui;
}

void student::set_table_it(int r,QString id, QString name, QString clas, QString chinese, QString mat, QString english)
{
    //插入表格中
    ui->tableWidget->setItem(r,0,new QTableWidgetItem(id));
    ui->tableWidget->setItem(r,1,new QTableWidgetItem(name));
    ui->tableWidget->setItem(r,2,new QTableWidgetItem(clas));
    ui->tableWidget->setItem(r,3,new QTableWidgetItem(chinese));
    ui->tableWidget->setItem(r,4,new QTableWidgetItem(mat));
    ui->tableWidget->setItem(r,5,new QTableWidgetItem(english));
}

//更新表格
void student::update_table()
{
      QString cmd;

      this->get_sql_row();

      //清空 数据库
        cmd = "delete from stu;";
       if(sql->exec(cmd)){
            qDebug() << "清空数据库成功";
       }else{
           qDebug() << "清空数据库失败";
       }



#if 0
       //重写插入数据
    cmd = "select count(*) from  stu;";
    if(sql->exec(cmd))
    {

         while(sql->next())
         {
               int row  = sql->value(0).toInt();
               qDebug() << row;
         }
    }

#endif
        //重新插入数据
        for(int i=0;i<row;i++)
        {
            QString id     = ui->tableWidget->item(i,0)->text();
            QString   name = ui->tableWidget->item(i,1)->text();
            QString   clas = ui->tableWidget->item(i,2)->text();
            QString   chinese = ui->tableWidget->item(i,3)->text();
            QString   mat     = ui->tableWidget->item(i,4)->text();
            QString   english = ui->tableWidget->item(i,5)->text();
            QString  sql_inser = QString("insert  into  stu values('%1','%2','%3',%4,%5,%6);").arg(id).arg(name).arg(clas).arg(chinese).arg(mat).arg(english);
            qDebug() << sql_inser;

            if(!sql->exec(sql_inser))
            {
               qDebug() << sql->lastError().text();
            }
        }

        //清空表格
        ui->tableWidget->clear();

          //设置表头
          QStringList head;
          head << "学号" << "姓名"  << "班级" << "语文" << "数学" << "英语" << "平均";
          ui->tableWidget->setHorizontalHeaderLabels(head);


         //显示表格中的数据
         this->show_table();


      ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
      qDebug() << "数据更新成功";

      ui->label->setText("温馨提示：正处于浏览模式");
      ui->label->setStyleSheet("color: rgb(0, 100, 0);");

}

void student::show_table()
{
    //重新设置行数
    int r=this->get_sql_row();

    ui->tableWidget->setRowCount(r);

    //查询数据库
    //获取表格内容
   QString cmd="select * from stu;";
    if(sql->exec(cmd))
    {
        int col=0;
         while(sql->next())
         {
              QString id     = sql->value(0).toString();
              QString   name = sql->value(1).toString();
              QString   clas = sql->value(2).toString();
              QString   chinese = sql->value(3).toString();
              QString   mat     = sql->value(4).toString();
              QString   english = sql->value(5).toString();

              float pingjun =  (chinese.toInt() + mat.toInt() + english.toInt())/3;

             ui->tableWidget->setItem(col,6,new QTableWidgetItem(QString::number(pingjun)));

             this->set_table_it(col++,id,name,clas,chinese,mat,english);



         }
    }
    else{
        qDebug() <<  sql->lastError().text();
    }
}

int student::get_sql_row()
{
    //获取数据库中的行数
  QString  cmd = "select count(*) from  stu;";
    if(sql->exec(cmd))
    {
         while(sql->next())
         {
               row  = sql->value(0).toInt();
               qDebug() << row;
         }
    }

    return row;
}

void student::on_pushButton_clicked()
{
        ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
        ui->label->setText("温馨提示：正处于编辑模式！");
        ui->label->setStyleSheet("color: rgb(255, 0, 0);");
}

void student::on_pushButton_2_clicked()
{
        this->update_table();
}
//插入数据
void student::on_pushButton_3_clicked()
{

    int r=this->get_sql_row();


    if(ui->pushButton_3->text() == "插入数据")
    {
        //ui->tableWidget->insertRow(1);
           ui->tableWidget->setRowCount(r+1);

           this->on_pushButton_clicked();

           ui->pushButton_3->setText("确定插入");
           return ;
    }
    else if(ui->pushButton_3->text() == "确定插入")
    {

             qDebug() <<  "确定插入";

             if(ui->tableWidget->item(row,0) == nullptr||
                ui->tableWidget->item(row,1) == nullptr||
                ui->tableWidget->item(row,2) == nullptr||
                ui->tableWidget->item(row,3) == nullptr||
                ui->tableWidget->item(row,4) == nullptr||ui->tableWidget->item(row,5)==nullptr)
             {
                qDebug() << "请输入数据";
                return ;
             }


            QString id     = ui->tableWidget->item(row,0)->text();
            QString   name = ui->tableWidget->item(row,1)->text();
            QString   clas = ui->tableWidget->item(row,2)->text();
            QString   chinese = ui->tableWidget->item(row,3)->text();
            QString   mat     = ui->tableWidget->item(row,4)->text();
            QString   english = ui->tableWidget->item(row,5)->text();



            QString  sql_inser = QString("insert  into  stu values('%1','%2','%3',%4,%5,%6);").arg(id).arg(name).arg(clas).arg(chinese).arg(mat).arg(english);
            qDebug() << sql_inser;
            if(sql->exec(sql_inser))
            {
                     qDebug() << "插入数据成功";

                     ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
                     ui->label->setText("温馨提示：正处于浏览模式");
                     ui->label->setStyleSheet("color: rgb(0, 100, 0);");

            }
            else {
                qDebug() << sql->lastError().text();
                qDebug() << "插入数据失败,请修改数据";
                return ;
            }

            ui->pushButton_3->setText("插入数据");
    }




        //更新表格
       this->show_table();

}

void student::on_pushButton_4_clicked()
{
   QString cmd  = QString("delete from stu where id='%1';").arg(ui->lineEdit->text());
    if(cmd.length() <= 0)
    {
        qDebug()  << "请输入数据";
    }

     if(!sql->exec(cmd))
     {
         qDebug() << sql->lastError().text();
     }

     this->show_table();

}

void student::on_tableWidget_cellDoubleClicked(int row, int column)
{
     qDebug() <<  row;
     qDebug() <<  column;
}

bool SortUporDown=true;

void student::sortByColumn(int arg)
{
         qDebug() <<  arg;

            if(SortUporDown)
            {
                ui->tableWidget->sortItems(arg, Qt::AscendingOrder);
                SortUporDown=false;
            }
            else
            {
                ui->tableWidget->sortItems(arg, Qt::DescendingOrder);
                SortUporDown=true;
            }

}

void student::on_pushButton_5_clicked()
{
    this->parentWidget()->show();

    this->hide();
}
