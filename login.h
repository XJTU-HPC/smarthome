#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QKeyEvent>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include "syszuxpinyin.h"
#include "my_lineedit.h"
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    SyszuxPinyin *syszuxpinyin_usrname;
    SyszuxPinyin *syszuxpinyin_passwd;
private slots:
    void  pushbutton_loginSlot();
    void  pushbutton_exitSlot();
    void  keyboardshow_usrnameSlot(QString data);
    void  keyboardshow_passwdSlot(QString data);
    void confirmString_usrnameSlot(QString gemfield);   //接收键盘发过来的数据
    void confirmString_passwdSlot(QString gemfield);   //接收键盘发过来的数据
private:
    Ui::Login *ui;
    My_lineEdit *password_edit;
    My_lineEdit *usr_edit;
    bool openDb();
    void createTable();
    void insertInfo();
signals:
    void loginsuccess();
    void loginfailed();
    void loginclose();
};

#endif // LOGIN_H
