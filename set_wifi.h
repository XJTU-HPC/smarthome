#ifndef SET_WIFI_H
#define SET_WIFI_H

#include <QDialog>
#include <QKeyEvent>
#include "syszuxpinyin.h"
#include "my_lineedit.h"
extern QString wifiName;
//extern int i;
namespace Ui {
class Set_Wifi;
}

class Set_Wifi : public QDialog
{
    Q_OBJECT

public:
    explicit Set_Wifi(QWidget *parent = nullptr);
    int get_i();
    QString get_TorF();
    SyszuxPinyin *syszuxpinyin;
    ~Set_Wifi();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void  keyboardshow(QString data);
    void confirmString(QString gemfield);   //接收键盘发过来的数据
private:
    Ui::Set_Wifi *ui;
    My_lineEdit *password_edit;
    //static int i;
    //static QString TorF;
};

#endif // SET_WIFI_H
