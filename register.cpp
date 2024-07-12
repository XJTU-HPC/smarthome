#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    password_edit = new My_lineEdit(ui->mydlg);
    password_edit->setObjectName("password");
    password_edit->setGeometry(QRect(670, 340, 181, 31));
    password_edit->setAutoFillBackground(false);
    password_edit->setStyleSheet(QString::fromUtf8(""));
    password_edit->setMaxLength(6);
    password_edit->setEchoMode(QLineEdit::Password);
    password_edit->setClearButtonEnabled(false);

    usr_edit = new My_lineEdit(ui->mydlg);
    usr_edit->setObjectName("username");
    usr_edit->setGeometry(QRect(670, 240, 181, 31));
    usr_edit->setStyleSheet(QString::fromUtf8(""));
    usr_edit->setText(QString::fromUtf8(""));
    usr_edit->setMaxLength(4);
    usr_edit->setClearButtonEnabled(true);
    syszuxpinyin_passwd =new SyszuxPinyin();
    syszuxpinyin_usrname=new SyszuxPinyin();
    connect(usr_edit,SIGNAL(send_show(QString)),this,SLOT(keyboardshow_usrnameSlot(QString)));
    connect(syszuxpinyin_usrname,SIGNAL(sendPinyin(QString)),this,SLOT(confirmString_usrnameSlot(QString)));
    connect(password_edit,SIGNAL(send_show(QString)),this,SLOT(keyboardshow_passwdSlot(QString)));
    connect(syszuxpinyin_passwd,SIGNAL(sendPinyin(QString)),this,SLOT(confirmString_passwdSlot(QString)));
    connect(ui->pushButton_register,SIGNAL(clicked()),this,SLOT(pushbutton_registerSlot()));
    connect(ui->pushButton_exit_2,SIGNAL(clicked()),this,SLOT(pushbutton_exitSlot()));
}

Register::~Register()
{
    delete ui;
}
void Register::keyboardshow_passwdSlot(QString data)
{

    syszuxpinyin_passwd->lineEdit_window->setText(data);
    syszuxpinyin_passwd->resize(800,310);
    syszuxpinyin_passwd->move(120,300);
    syszuxpinyin_passwd->show();

}
void  Register::confirmString_passwdSlot(QString gemfield)
{
   password_edit->setText(gemfield);
}
void Register::keyboardshow_usrnameSlot(QString data)
{

    syszuxpinyin_usrname->lineEdit_window->setText(data);
    syszuxpinyin_usrname->resize(800,310);
    syszuxpinyin_usrname->move(120,300);
    syszuxpinyin_usrname->show();

}
void Register::confirmString_usrnameSlot(QString gemfield)
{
   usr_edit->setText(gemfield);
}
void Register::pushbutton_registerSlot()
{
    insertusr();
}
void Register::pushbutton_exitSlot()
{
    close();
}
bool Register::insertusr()
{
    QString name = usr_edit->text();
    QString password = password_edit->text();
    if (name == ""||password == "")
    {
        QMessageBox::warning(this,"警告","用户名密码不能为空");
        return false;
    }
    QSqlQuery sq;
    //防止sql注入问题
    QString sql = "INSERT INTO usr VALUES(?,?)";
    //预处理
    sq.prepare(sql);
    //占位符数据替换
    sq.addBindValue(name);
    sq.addBindValue(password);
    //执行sql语句
    if(sq.exec())
    {
        QMessageBox::information(this,"通知","注册成功");
    }
    else
    {
        QString errorMsg = sq.lastError().text();
        QMessageBox::critical(this,"错误",errorMsg);
    }
    return true;
}
