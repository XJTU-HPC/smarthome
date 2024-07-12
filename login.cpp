#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
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
    connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(pushbutton_loginSlot()));
    connect(ui->pushButton_exit,SIGNAL(clicked()),this,SLOT(pushbutton_exitSlot()));
}

Login::~Login()
{
    delete ui;
}
void Login::keyboardshow_passwdSlot(QString data)
{

    syszuxpinyin_passwd->lineEdit_window->setText(data);
    syszuxpinyin_passwd->resize(800,310);
    syszuxpinyin_passwd->move(120,300);
    syszuxpinyin_passwd->show();

}
void  Login::confirmString_passwdSlot(QString gemfield)
{
   password_edit->setText(gemfield);
}
void Login::keyboardshow_usrnameSlot(QString data)
{

    syszuxpinyin_usrname->lineEdit_window->setText(data);
    syszuxpinyin_usrname->resize(800,310);
    syszuxpinyin_usrname->move(120,300);
    syszuxpinyin_usrname->show();

}
void Login::confirmString_usrnameSlot(QString gemfield)
{
   usr_edit->setText(gemfield);
}
void Login::pushbutton_loginSlot()
{
    bool ret =openDb();
    if(ret == true)
        emit loginsuccess();
    else
        emit loginfailed();
}
void Login::createTable()
{
    //创建一个数据库操作对象
    QSqlQuery sq;
    QString sql = "CREATE TABLE IF NOT EXISTS usr(usrname TEXT PRIMARY KEY,password TEXT NOT NULL);";
    if(sq.exec(sql))
        qDebug() << "CREATE TABLE success";
    else
        qDebug() << sq.lastError().text();
}
void Login::insertInfo()
{
    QSqlQuery sq;
    QString sql = "INSERT INTO usr VALUES(\"usr\",\"123\");";
    if(sq.exec(sql))
        qDebug() << "INSERT INTO success";
    else
        qDebug() << sq.lastError().text();
}
bool Login::openDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("usr.db");
    if(!db.open())
    {
        QMessageBox::warning(0, tr("Warning"), db.lastError().text());
        return false;
    }
//    建表
    createTable();
//    插入
    insertInfo();
    QSqlQuery query(db);
    if(!query.exec("select usrname,password from usr"))
    {
        db.close();
        return false;
    }
    while(query.next())
    {
        QString UserName =   query.value(0).toString();
        QString Password =   query.value(1).toString();
//        qDebug()<< UserName;
//        qDebug()<< Password;
        if(UserName == usr_edit->text() &&Password ==password_edit->text())
            return true;
    }
    db.close();
    return false;
}
void Login::pushbutton_exitSlot()
{
    emit loginclose();
    close();
}
