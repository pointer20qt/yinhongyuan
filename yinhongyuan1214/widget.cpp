#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QCheckBox>
#include <QDateTime>
#include <QString>
#include <QMessageBox>
#include "person.h"

Widget::Widget(QWidget *parent,QString sname) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->sname=sname;

    dataToTable("select * from student");

    displayClass();

    displaySno();

    connect(ui->tableWidget,QTableWidget::cellDoubleClicked,[=](int r,int c){
        Person* p=new Person();
        p->init(ui->tableWidget->item(r,0)->text(),ui->tableWidget->item(r,1)->text());
        p->show();
    });




}

//将数据显示到表格
void Widget::dataToTable(QString sql)
{
    qDebug()<<this->sname;
    QSqlQuery q;
    q.exec(QString("select sno from student where sname='%1'").arg(this->sname));
    q.next();
    QString curSno=q.value(0).toString();

    if(q.exec(sql))
    {
        ui->tableWidget->setRowCount(q.size());
        int i=0;
        while(q.next())
        {
            QCheckBox *c=new QCheckBox("未签到");
            QString sno=q.value(0).toString();

            QSqlQuery q3;
            q3.exec(QString("select signTime from signinfo "
                            "where sno=%1 and date(signTime)=curdate() "
                            "order by signTime desc").arg(sno));
            q3.next();

            if(sno!=curSno)
            {
                c->setDisabled(true);
            }

            if(q3.size())
            {
                c->setText("已签到");
                c->setDisabled(true);
                c->setChecked(true);
                ui->tableWidget->setItem(i,4,new QTableWidgetItem(q3.value(0).toString()));
            }

            ui->tableWidget->setItem(i,0,new QTableWidgetItem(sno));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(q.value(1).toString()));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(q.value(2).toString()));
            ui->tableWidget->setCellWidget(i,3,c);


            connect(c,QCheckBox::clicked,[=](){
                //1.修改控件本身的状态
                c->setText("已签到");
                c->setDisabled(true);
                //2.向表格中添加本人签到时间
                ui->tableWidget->setItem(i,4,new QTableWidgetItem(QDateTime::currentDateTime().toString()));

                //3.向数据库中插入一条数据
                QSqlQuery q1;
                qDebug()<<q1.exec(QString("insert into signinfo values(%1,now())").arg(sno));
            });
            i++;
        }
    }

}
//按照班级显示
void Widget::displayClass()
{
    QSqlQuery q4;
    q4.exec("select distinct class from student");
    ui->comboBox->addItem("全部班级");
    while(q4.next())
    {
        ui->comboBox->addItem(q4.value(0).toString());
    }
    connect(ui->comboBox,QComboBox::currentTextChanged,[=](QString text){
        if(text=="全部班级")
        {
            dataToTable("select * from student");
        }
        else
        {
            dataToTable(QString("select * from student where class='%1'").arg(text));
        }
    });
}
//按照学号显示
void Widget::displaySno()
{
    connect(ui->pushButton,QPushButton::clicked,[=](){
        if(ui->lineEdit->text().isEmpty())
        {
            QMessageBox::warning(this,"warn","您输入为空");
        }
        else
        {
            dataToTable(QString("select * from student where sno like'%1'").arg(ui->lineEdit->text()));
        }
    });


}

Widget::~Widget()
{
    delete ui;
}
