#ifndef STUDENT_H
#define STUDENT_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class student;
}

class student : public QMainWindow
{
    Q_OBJECT

public:
    explicit student(QWidget *parent = nullptr);
    ~student();

    void set_table_it(int r,QString id,QString name,QString clas,QString chinese,QString mat,QString english);

    void update_table();

    void  show_table();

    int get_sql_row();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);
    void sortByColumn(int);

    void on_pushButton_5_clicked();

private:
    Ui::student *ui;
};

#endif // STUDENT_H
