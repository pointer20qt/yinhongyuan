#include "person.h"
#include "ui_person.h"
#include <QSqlQuery>
#include <QDebug>
#include <QTextCharFormat>

Person::Person(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Person)
{
    ui->setupUi(this);
}

void Person::init(QString sno,QString sname)
{
    ui->label->setText("学号: "+sno);
    ui->label_2->setText("姓名: "+sname);

    QSqlQuery q;
    q.exec(QString("select signTime from signInfo "
           "where sno=%1 and date_format(signTime,'%Y-%m')="
                  "date_format(curdate(),'%Y-%m')").arg(sno));
    ui->label_3->setText("签到次数: "+QString::number(q.size()));

    QTextCharFormat f;
    f.setForeground(Qt::red);
    f.setBackground(Qt::green);

    while(q.next())
    {
        ui->calendarWidget->setDateTextFormat(q.value(0).toDate(),f);
    }


}

Person::~Person()
{
    delete ui;
}
