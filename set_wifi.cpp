#include "set_wifi.h"
#include "ui_set_wifi.h"
#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QSettings>
//int Set_Wifi::i = 0;
Set_Wifi::Set_Wifi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Set_Wifi)
{ 
    ui->setupUi(this);
    password_edit = new My_lineEdit(this);
    password_edit->setObjectName("password");
    password_edit->setGeometry(QRect(150, 110, 161, 31));
    syszuxpinyin =new SyszuxPinyin();
    connect(password_edit,SIGNAL(send_show(QString)),this,SLOT(keyboardshow(QString)));
    connect(syszuxpinyin,SIGNAL(sendPinyin(QString)),this,SLOT(confirmString(QString)));
}

Set_Wifi::~Set_Wifi()
{
    delete ui;
}
void Set_Wifi::keyboardshow(QString data)
{

    syszuxpinyin->lineEdit_window->setText(data);
    syszuxpinyin->resize(800,310);
    syszuxpinyin->move(120,300);
    syszuxpinyin->show();

}
void  Set_Wifi::confirmString(QString gemfield)
{
   password_edit->setText(gemfield);
}
void Set_Wifi::on_pushButton_clicked()
{
    bool passwordvaild = false;
    int i =get_i();
    while (!passwordvaild) {
        QString passwordtext = password_edit->text();

        if(password_edit->text().isEmpty() || passwordtext.length() < 8){
            QMessageBox::warning(this,tr("Connect information"),tr("密码不能为空且长度必须大于等于8位"));
            password_edit->clear();
            continue;
        }else{
            QProcess process;
            QString command = "wpa_cli";
            QStringList arguments;
            arguments << "-i" << "wlan0" << QString("set_network %1 ssid '%2'").arg(i).arg(wifiName);
            process.start(command,arguments);
            process.waitForFinished();

            arguments.clear();
            arguments << "-i" << "wlan0" << QString("set_network %1 psk '%2' > TorF.ini").arg(i).arg(wifiName);
            process.start(command,arguments);
            process.waitForFinished();
            qDebug()<<"i"<<i;
            arguments.clear();
            arguments << "-i" << "wlan0" << QString("select_network %1").arg(i);
            process.start(command,arguments);
            process.waitForFinished();

            QString torfOutput = process.readAllStandardOutput();
            QString torf = torfOutput.trimmed().toUpper();
            qDebug()<<"Torf" << torf;
            if(torf == 'F'){
                QMessageBox::warning(this, tr("Connect information"), tr("密码错误"));
                password_edit->clear();
                continue;
            }
            passwordvaild = true;
            qDebug() << "success";
        }
    }

        system("udhcpc -i wlan0");
        system("echo \"nameserver 114.114.114.114\" > /etc/resolv.conf");
        system("echo \"nameserver 8.8.8.8\" > /etc/resolv.conf");
        QProcess pingProcess;
        QString command = "ping";
        QStringList arguments;
        arguments << "-c" << "4" << "www.baidu.com";
        pingProcess.start(command, arguments);
        pingProcess.waitForFinished();
        QString pingOutput = pingProcess.readAllStandardOutput();
        qDebug() << pingOutput;

        close();
        QMessageBox::information(this, tr("Connect information"), tr("连接成功"));

}
//读取i的值
int Set_Wifi::get_i()
{
    char buf[32] = {0};
    FILE* fp0 = fopen("i.ini", "r+");//读写方式打开
    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf), fp0);
    QString str = buf;
    return str.toInt();
}
//验证是否登陆成功
QString Set_Wifi::get_TorF()
{
    char buf[32] = {0};
    FILE* fp = fopen("TorF.ini", "r+");//读写方式打开
    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf), fp);
    QString str = buf;
    for (int i = 0;i<str.length();i++) {
        QString work = str.at(i);
        qDebug() <<"work :" << work;
    }
    return str;
}
void Set_Wifi::on_pushButton_2_clicked()
{
    close();
}

