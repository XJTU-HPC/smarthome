#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <QTextStream>
#include <QFile>
#include <QRadioButton>
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>
#include <QProcess>
#include <gpiod.h>
#include <QDebug>
#include "QTextCodec"
#include <QFont>
#include "QFontDatabase"
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <qcryptographichash.h>
#include <QMovie>
#include <iostream>
#define BOARD_TYPE_FSMP1A            1
#define BOARD_TYPE_FSMP1C            2
#define EXIT_FAN (_IO('f', 7))
#define FAN_UP (_IO('f', 5))
#define FAN_DOWN (_IO('f', 6))
#define INIT_FAN (_IO('f', 4))

//static int get_board_type(void)
//{
//    char searchText[]="-fsmp1a";
//    int len_searchText;
//    FILE *file;
//    char string[128];
//    int len_string;
//    int i = 0;

//    memset(string, 0x0, sizeof(string));

//    file = fopen("/proc/device-tree/compatible", "r");
//    if (file == NULL) {
//        qDebug("fails to open /proc/device-tree/compatible\n");
//        return -1;
//    }

//    len_searchText = strlen(searchText);

//    while(fgets(string, sizeof(string), file) != 0)
//    {
//        len_string = strlen(string);
//        for(i = 0 ; i < len_string ; i++) {
//            if(strncmp(searchText, (string + i), len_searchText) == 0) {
//                fclose(file);
//                return BOARD_TYPE_FSMP1A;
//            }
//        }
//    }
//    fclose(file);
//    return BOARD_TYPE_FSMP1C;
//}
//异常检测开关
bool abnormalSwitch=false;
//输入密码剩余次数
int Numberoferrors =3;
QString wifiName;
//static int j = 0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), facerec(nullptr)
{
    ui->setupUi(this);
    ui->tabWidget->setIconSize(QSize(40, 40));
    ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabWidget->widget(0)), QIcon(":/newpics/icons8-主页-40.png"));
    ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabWidget->widget(1)), QIcon(":/newpics/icons8-夜晚晴间多云-40.png"));
    ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabWidget->widget(2)), QIcon(":/newpics/icons8-湿度-40.png"));
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabWidget->widget(3)), QIcon(":/newpics/icons8-无线上网-40.png"));
    ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabWidget->widget(4)), QIcon(":/newpics/icons8-电力-40.png"));
    ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabWidget->widget(5)), QIcon(":/newpics/icons8-仪表板-40.png"));
    ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabWidget->widget(6)), QIcon(":/newpics/icons8-壁挂式摄像机-40.png"));
    ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(ui->tabWidget->widget(7)), QIcon(":/newpics/icons8-上传到云-40.png"));
    ui->tabWidget->setTabPosition(QTabWidget::West);
    ui->quitappwifi->setStyleSheet("QPushButton{border:none;outline:none;background-color:transparent;}");
    ui->quitappwifi->setFixedSize(50,50);
    ui->quitappwifi->setIcon(QIcon(":/icon/exit2.png"));
    ui->quitappwifi->setIconSize(QSize(50,50));

    ui->quitappweather->setStyleSheet("QPushButton{border:none;outline:none;background-color:transparent;}");
    ui->quitappweather->setFixedSize(50,50);
    ui->quitappweather->setIcon(QIcon(":/icon/exit2.png"));
    ui->quitappweather->setIconSize(QSize(50,50));

    ui->quitapphumiture->setStyleSheet("QPushButton{border:none;outline:none;background-color:transparent;}");
    ui->quitapphumiture->setFixedSize(50,50);
    ui->quitapphumiture->setIcon(QIcon(":/icon/exit2.png"));
    ui->quitapphumiture->setIconSize(QSize(50,50));

    ui->quitappcotrol->setStyleSheet("QPushButton{border:none;outline:none;background-color:transparent;}");
    ui->quitappcotrol->setFixedSize(50,50);
    ui->quitappcotrol->setIcon(QIcon(":/icon/exit2.png"));
    ui->quitappcotrol->setIconSize(QSize(50,50));

    ui->quitappabnormal->setStyleSheet("QPushButton{border:none;outline:none;background-color:transparent;}");
    ui->quitappabnormal->setFixedSize(50,50);
    ui->quitappabnormal->setIcon(QIcon(":/icon/exit2.png"));
    ui->quitappabnormal->setIconSize(QSize(50,50));

    ui->quitappioT->setStyleSheet("QPushButton{border:none;outline:none;background-color:transparent;}");
    ui->quitappioT->setFixedSize(50,50);
    ui->quitappioT->setIcon(QIcon(":/icon/exit2.png"));
    ui->quitappioT->setIconSize(QSize(50,50));
    QMovie *movie = new QMovie(":/newpics/随机姿势.gif");
    ui->labelgifhome->setMovie(movie);
    movie->start();
    QMovie *movie_wifi = new QMovie(":/newpics/头左右.gif");
    ui->labelgif_wifi->setMovie(movie_wifi);
    movie_wifi->start();
    QMovie *movie_weather = new QMovie(":/newpics/进来2.gif");
    ui->labelgif_weather->setMovie(movie_weather);
    movie_weather->start();
    QMovie *movie_env = new QMovie(":/newpics/speak2_start.gif");
    ui->labelgif_env->setMovie(movie_env);
    movie_env->start();
    QMovie *movie_ctrl = new QMovie(":/newpics/头左右.gif");
    ui->labelgif_ctrl->setMovie(movie_ctrl);
    movie_ctrl->start();
    QMovie *movie_auto = new QMovie(":/newpics/speak2_start.gif");
    ui->labelgif_auto->setMovie(movie_auto);
    movie_auto->start();
//    ui->tabWidget->setStyleSheet("QTabWidget#tabWidget{background-color:rgb(255,0,0);}\
//                                    QTabBar::tab{background-color:rgb(220,200,180);color:rgb(0,0,0);font:10pt '新宋体'}\
//                                    QTabBar::tab::selected{background-color:rgb(0,100,200);color:rgb(255,0,0);font:10pt '新宋体'}");
//                                开发板类型
//    board_type = get_board_type();
//    led1_off_btnSlot();
//    led2_off_btnSlot();
//    led3_off_btnSlot();
    connect(ui->button_home1, &QPushButton::clicked, this, [this](){ui->tabWidget->setCurrentIndex(1);});
    connect(ui->button_home2, &QPushButton::clicked, this, [this](){ui->tabWidget->setCurrentIndex(2);});
    connect(ui->button_startface, &QPushButton::clicked, this, &MainWindow::startFaceRec);
    connect(ui->button_stopface, &QPushButton::clicked, this, &MainWindow::stopFaceRec);
    /***************************** 连接wlan模块构造 *********************************/
    WifiDlg = new Set_Wifi(ui->tab_WiFi);
    Get_Wifi_Name();
    /***************************** weather模块构造  ********************************/
    QStringList cityList = QString("西安,富平,宝鸡,鹤壁,上海,北京").split(",");
    ui->comboBox->addItems(cityList);
    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
    connect(ui->pushButton_weather,SIGNAL(clicked()),this,SLOT(weather_cilcked_Slot()));

    /****************************** 启动数据采集线程  ****************************/
    connect(&thread_collentdata,SIGNAL(send(QString,QString,QString)),this,SLOT(set_humAdtemAdill(QString,QString,QString)));
    connect(&thread_collentdata,SIGNAL(turn_warn_on(const char*, int)),this,SLOT(beep_on_btnSlot()));
    thread_collentdata.start();
//    /****************************** 启动光电开关线程  ****************************/
//    connect(&pe15thread,SIGNAL(pesig()),this,SLOT(loginSlot()));
//    pe15thread.start();
    /******************************    控制模块  *********************************/
    connect(ui->pushButton_led1_on,SIGNAL(clicked()),this,SLOT(led1_on_btnSlot()));
    connect(ui->pushButton_led1_off,SIGNAL(clicked()),this,SLOT(led1_off_btnSlot()));
    connect(ui->pushButton_led2_on,SIGNAL(clicked()),this,SLOT(led2_on_btnSlot()));
    connect(ui->pushButton_led2_off,SIGNAL(clicked()),this,SLOT(led2_off_btnSlot()));
    connect(ui->pushButton_led3_on,SIGNAL(clicked()),this,SLOT(led3_on_btnSlot()));
    connect(ui->pushButton_led3_off,SIGNAL(clicked()),this,SLOT(led3_off_btnSlot()));
//    connect(ui->pushButton_fan_on,SIGNAL(clicked()),this,SLOT(fan_on_btnSlot()));
//    connect(ui->pushButton_fan_off,SIGNAL(clicked()),this,SLOT(fan_off_btnSlot()));
//    connect(ui->pushButton_fan_2,SIGNAL(clicked()),this,SLOT(fan_2_btnSlot()));
//    connect(ui->pushButton_fan_3,SIGNAL(clicked()),this,SLOT(fan_3_btnSlot()));
    connect(ui->pushButton_beep_on,SIGNAL(clicked()),this,SLOT(beep_on_btnSlot()));
    connect(ui->pushButton_beep_off,SIGNAL(clicked()),this,SLOT(beep_off_btnSlot()));

    /******************************    设置异常阈值初始值  *******************************/
    /****************************    初始化软件盘 以及建立信号槽连接  ***********************/
    tem_max = "28";
    ill_lv1 = "3";
    ill_lv2 = "2";
    ill_lv3 = "1";
    lineEdit_led_lv1 =new My_lineEdit(ui->tab_abnormal);
    lineEdit_led_lv2 =new My_lineEdit(ui->tab_abnormal);
    lineEdit_led_lv3 =new My_lineEdit(ui->tab_abnormal);
    lineEdit_fan_tem =new My_lineEdit(ui->tab_abnormal);
    lineEdit_led_lv1->setObjectName("lineEdit_led_lv1");
    lineEdit_led_lv1->setGeometry(QRect(260, 290, 111, 31));
    lineEdit_led_lv1->setText("90");
    lineEdit_led_lv1->setStyleSheet("background:white;");
    lineEdit_led_lv2->setObjectName("lineEdit_led_lv2");
    lineEdit_led_lv2->setGeometry(QRect(260, 400, 111, 31));
    lineEdit_led_lv2->setText("60");
    lineEdit_led_lv2->setStyleSheet("background:white;");
    lineEdit_led_lv3->setObjectName("lineEdit_led_lv3");
    lineEdit_led_lv3->setGeometry(QRect(260, 510, 111, 31));
    lineEdit_led_lv3->setText("30");
    lineEdit_led_lv3->setStyleSheet("background:white;");
    lineEdit_fan_tem->setObjectName("lineEdit_fan_tem");
    lineEdit_fan_tem->setGeometry(QRect(770, 140, 111, 31));
    lineEdit_fan_tem->setText("");
    lineEdit_fan_tem->setStyleSheet("background:transparent;");
    syszuxpinyin_ledlv1 =new SyszuxPinyin();
    syszuxpinyin_ledlv2 =new SyszuxPinyin();
    syszuxpinyin_ledlv3 =new SyszuxPinyin();
    syszuxpinyin_fan    =new SyszuxPinyin();
    connect(lineEdit_led_lv1,SIGNAL(send_show(QString)),this,SLOT(abn_keyboardshow_ledlv1(QString)));
    connect(lineEdit_led_lv2,SIGNAL(send_show(QString)),this,SLOT(abn_keyboardshow_ledlv2(QString)));
    connect(lineEdit_led_lv3,SIGNAL(send_show(QString)),this,SLOT(abn_keyboardshow_ledlv3(QString)));
    connect(lineEdit_fan_tem,SIGNAL(send_show(QString)),this,SLOT(abn_keyboardshow_fan(QString)));
    connect(syszuxpinyin_ledlv1,SIGNAL(sendPinyin(QString)),this,SLOT(abn_confirmString_ledlv1(QString)));
    connect(syszuxpinyin_ledlv2,SIGNAL(sendPinyin(QString)),this,SLOT(abn_confirmString_ledlv2(QString)));
    connect(syszuxpinyin_ledlv3,SIGNAL(sendPinyin(QString)),this,SLOT(abn_confirmString_ledlv3(QString)));
    connect(syszuxpinyin_fan,SIGNAL(sendPinyin(QString)),this,SLOT(abn_confirmString_fan(QString)));
    connect(ui->pushButton_abn_led,SIGNAL(clicked()),this,SLOT(pushButton_abn_ledSlot()));

//    connect(ui->pushButton_abn_fan,SIGNAL(clicked()),this,SLOT(pushButton_abn_fanSlot()));
   /***************************   连接百度云模块  **************************************/
    ui->pushButton_connectmqtt->setEnabled(false);
//    lineEdit_coreid = new My_lineEdit(ui->tab_ioT);
//    lineEdit_devkey = new My_lineEdit(ui->tab_ioT);
//    lineEdit_devsecret = new My_lineEdit(ui->tab_ioT);
//    lineEdit_coreid->setObjectName(QString::fromUtf8("lineEdit_coreid"));
//    lineEdit_coreid->setGeometry(QRect(490, 50, 211, 41));
//    lineEdit_coreid->setText("akaveqc");
//    lineEdit_devkey->setObjectName(QString::fromUtf8("lineEdit_devkey"));
//    lineEdit_devkey->setGeometry(QRect(490, 110, 211, 41));
//    lineEdit_devkey->setText("device");
//    lineEdit_devsecret->setObjectName(QString::fromUtf8("lineEdit_devsecret"));
//    lineEdit_devsecret->setGeometry(QRect(490, 170, 211, 41));
//    lineEdit_devsecret->setText("mqdBApqXmUPApPtd");
    syszuxpinyin_iotCoreid=new SyszuxPinyin();;
    syszuxpinyin_iotDevKey=new SyszuxPinyin();;
    syszuxpinyin_iotDevSecret=new SyszuxPinyin();;
    m_client = new QMqttClient(this);
//    connect(lineEdit_coreid,SIGNAL(send_show(QString)),this,SLOT(ito_keyboardshow_coreldSlot(QString)));
//    connect(lineEdit_devkey,SIGNAL(send_show(QString)),this,SLOT(ito_keyboardshow_devKeySlot(QString)));
//    connect(lineEdit_devsecret,SIGNAL(send_show(QString)),this,SLOT(ito_keyboardshow_devSecretSlot(QString)));
//    connect(syszuxpinyin_iotCoreid,SIGNAL(sendPinyin(QString)),this,SLOT(ito_confirmString_coreldSlot(QString)));
//    connect(syszuxpinyin_iotDevKey,SIGNAL(sendPinyin(QString)),this,SLOT(ito_confirmString_devKeySlot(QString)));
//    connect(syszuxpinyin_iotDevSecret,SIGNAL(sendPinyin(QString)),this,SLOT(ito_confirmString_devSecretSlot(QString)));

//    connect(ui->pushButton_gettime,SIGNAL(clicked()),this,SLOT(pushButton_gettimeSlot()));
    connect(ui->pushButton_calculate,SIGNAL(clicked()),this,SLOT(pushButton_calculateSlot()));
    connect(ui->pushButton_connectmqtt,SIGNAL(clicked()),this,SLOT(pushButton_connectmqttSlot()));


//     /***************  门禁模块  *******************/
//     login = new Login();
//     regist = new Register();
    /**********************  异常开关 *******************/
     connect(ui->pushButton_abn_ON,SIGNAL(clicked()),this,SLOT(abn_pushbutton_ONSlot()));
     connect(ui->pushButton_abn_OFF,SIGNAL(clicked()),this,SLOT(abn_pushbutton_OFFSlot()));

//     currentTime=new QTimer(this);
//     currentTime->start(1000); // 每次发射timeout信号时间间隔为1秒

//     connect(currentTime,SIGNAL(timeout()),this,SLOT(timeUpdate()));

//     system("wpa_supplicant -D nl80211 -i wlan0 -c /etc/wpa_supplicant.conf -B");
}

MainWindow::~MainWindow()
{
    delete ui;
    if(facerec && facerec->isRunning()) {
        facerec->stop();
        facerec->wait();
    }
}

void MainWindow::updateImage(const QImage &image) {
    ui->label_facerec->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::startFaceRec() {
    std::cout<<"start facerec"<<std::endl;
    if (facerec) {
        std::cout<<"start facerec"<<std::endl;
        if (facerec->isRunning()) {
            facerec->stop();
            facerec->wait(); // 等待线程完成
        }
        delete facerec; // 删除旧的线程对象
    }
    std::cout<<"before new facerec"<<std::endl;
    facerec = new faceRecThread(this);
    connect(facerec, &faceRecThread::alarm, this, &MainWindow::alarmStranger);
    connect(facerec, &faceRecThread::imageReady, this, &MainWindow::updateImage);
//    connect(facerec, &faceRecThread::finished, facerec, &QObject::deleteLater);
    ui->button_startface->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/OpenMonitoring.png);\n"
    "border-style:outset;"));
    ui->button_stopface->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disCloseMonitoring.png);\n"
    "border-style:outset;"));
    facerec->start();
}

void MainWindow::stopFaceRec() {
    if(facerec && facerec->isRunning()) {
        ui->button_startface->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disOpenMonitoring.png);\n"
        "border-style:outset;"));
        ui->button_stopface->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/CloseMonitoring.png);\n"
        "border-style:outset;"));
        facerec->stop();
        facerec->wait();
    }

}

/***************************** 连接wlan模块 *********************************/
void MainWindow::on_connectBTN_clicked()
{
//    添加网络
    //system("wpa_supplicant -D nl80211 -i wlan0 -c /etc/wpa_supplicant.conf -B");
    system("wpa_cli -i wlan0 add_network > i.ini");
    qDebug()<<"connectBtn clicked";
    if(ui->Wlan1->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan1->text();
            WifiDlg->show();
        }else if(ui->Wlan2->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan2->text();
            WifiDlg->show();
        }else if(ui->Wlan3->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan3->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan3->text();
            WifiDlg->show();
        }else if(ui->Wlan4->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan4->text();
            WifiDlg->show();
        }else if(ui->Wlan5->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan5->text();
            WifiDlg->show();
        }else if(ui->Wlan6->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan6->text();
            WifiDlg->show();
        }else if(ui->Wlan7->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan7->text();
            WifiDlg->show();
        }else if(ui->Wlan8->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan8->text();
            WifiDlg->show();
        }else if(ui->Wlan9->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan9->text();
            WifiDlg->show();
        }else if(ui->Wlan10->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan10->text();
            WifiDlg->show();
        }else if(ui->Wlan11->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan11->text();
            WifiDlg->show();
        }else if(ui->Wlan12->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan12->text();
            WifiDlg->show();
        }else if(ui->Wlan13->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan13->text();
            WifiDlg->show();
        }else if(ui->Wlan14->isChecked())
        {
            ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan14->setStyleSheet("color:rgb(114, 159, 207);");
            ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
            ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan14->text();
            WifiDlg->show();
        }else if(ui->Wlan15->isChecked())
        {
        ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan15->setStyleSheet("color:rgb(114, 159, 207);");
        ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan16->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan15->text();
            WifiDlg->show();
        }else if(ui->Wlan16->isChecked())
        {
        ui->Wlan1->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan2->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan16->setStyleSheet("color:rgb(114, 159, 207);");
        ui->Wlan3->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan4->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan5->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan6->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan8->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan9->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan10->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan11->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan12->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan13->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan14->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan15->setStyleSheet("color:rgb(0, 0, 0);");
        ui->Wlan7->setStyleSheet("color:rgb(0, 0, 0);");
            wifiName = ui->Wlan16->text();
            WifiDlg->show();
        }else
        {
            QMessageBox::warning(this,"Warnning","请选择要连接的网络",QMessageBox::Yes);
        }
}
void MainWindow::Get_Wifi_Name()
{
    QStringList arguments;
    QProcess process;
    QRadioButton *wlangroup[16] = {ui->Wlan1,ui->Wlan2,ui->Wlan3,ui->Wlan4,ui->Wlan5,ui->Wlan6,ui->Wlan7,ui->Wlan8,ui->Wlan9,
                                   ui->Wlan10,ui->Wlan11,ui->Wlan12,ui->Wlan13,ui->Wlan14,ui->Wlan15,ui->Wlan16};
    //system("wpa_supplicant -D nl80211 -i wlan0 -c /etc/wpa_supplicant.conf -B");
    QThread::msleep(100);
    system("wpa_cli -i wlan0 scan");
    QThread::msleep(100);
    arguments << "-i" << "wlan0" << "scan_results";
    process.start("wpa_cli",arguments);
    if(!process.waitForFinished()){
        qDebug() << "Failed execute command";
        return;
    }
    QByteArray output = process.readAllStandardOutput();
    QString result = QString::fromLocal8Bit(output);
    QStringList wifiList = result.split("\n");
    QStringList wifiNames;
    for (int i =0;i < wifiList.size();i++) {
        QString wifiInfo = wifiList.at(i);
        QStringList infoList = wifiInfo.split("\t");
        if(infoList.size() >= 5){
            QString wifiNamelist = infoList.at(4);
            wifiNames.append(wifiNamelist);
        }
    }
    for (int i = 0;i < wifiNames.size() && i < 16;i++){
        QString wifiName = wifiNames.at(i);
        qDebug() << wifiName;
        wlangroup[i]->setVisible(true);
        wlangroup[i]->setText(wifiName);
    }
}
//刷新
void MainWindow::on_connectBTN_2_clicked()
{
    Get_Wifi_Name();
     QMessageBox::information(this,tr("Connect information"), tr("成功!"));
}
/******************************   天气模块 ***************************************/
//点击查询请求天气数据
void MainWindow::weather_cilcked_Slot()
{
    QString local_city = ui->comboBox->currentText().trimmed(); //获得需要查询天气的城市名称
    sendQuest(local_city);
}
//get方法获取信息
void MainWindow::sendQuest(QString cityStr)
{
    char quest_array[256] = "http://v0.yiketianqi.com/api?unescape=1&version=v61&appid=13717381&appsecret=S3yuPKG9&city=";
    QNetworkRequest quest;
    sprintf(quest_array, "%s%s", quest_array, cityStr.toUtf8().data());
    quest.setUrl(QUrl(quest_array));
    quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
    manager->get(quest);    /*发送get网络请求*/
}
//天气数据处理槽函数
void MainWindow::replyFinished(QNetworkReply *reply)
{
    QString all = reply->readAll();
    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(), &err);//解析json对象
    qDebug()<<"all:"<<all;

    if (!json_recv.isNull() && err.error == QJsonParseError::NoError)
    {
        QJsonObject object = json_recv.object();
        QStringList keys = object.keys();
        foreach (const QString& key,keys){
            qDebug() << "Key:" <<key;
        }
        QString weather_type = object.value("wea").toString();
        qDebug()<<"weather_type :"<<weather_type;
        QString tuijian = object.value("air_tips").toString();
        qDebug()<<"tuijian: "<<tuijian;
        QString low = object.value("tem2").toString();
        qDebug()<<"low :"<<low;
        QString high = object.value("tem1").toString();
        QString wendu = low.mid(low.length() - 4, 4) + "~" + high.mid(high.length() - 4, 4);
        qDebug()<<"wendu: "<<wendu;
        QString strength = object.value("win_speed").toString();
        qDebug()<<"strength: "<<strength;
        strength.remove(0, 8);
        strength.remove(strength.length() - 2, 2);
        QString fengli = object.value("win").toString() + strength;
        ui->label_weather_2->setText(weather_type);    //显示天气类型
        ui->label_temperature_2->setText(wendu);
        ui->label_wind_2->setText(fengli);
        ui->label_recommend_2->setText(tuijian);
    }
    else{
        ui->label_recommend_2->setText( "json_recv is NULL or is not a object !");
        qDebug() << "recv weather data! error:"<< err.error;
    }
    reply->deleteLater(); //销毁请求对象
}
/*****************************  数据采集模块  *************************************/
void MainWindow::set_humAdtemAdill(QString tem,QString hum,QString ill)
{
    //    将线程采集的数据赋值给成员变量
    this->tem =tem;
    this->hum =hum;
    this->ill =ill;
    /******************************    异常处理  *********************************/
    if(abnormalSwitch == true)
    {
//        if(this->tem.toFloat() >tem_max.toFloat())
//        {
//            fan_3_btnSlot();
//        }
//        else
//        {
//           fan_off_btnSlot();
//        }
        if(this->ill.toFloat() < ill_lv1.toFloat()&&this->ill.toFloat() > ill_lv2.toFloat())
        {
            led1_on_btnSlot();
        }
        else if(this->ill.toFloat() < ill_lv2.toFloat()&&this->ill.toFloat() > ill_lv3.toFloat())
        {
            led1_on_btnSlot();
            led2_on_btnSlot();
        }
        else if(this->ill.toFloat() < ill_lv3.toFloat())
        {
            led1_on_btnSlot();
            led2_on_btnSlot();
            led3_on_btnSlot();
        }
        else
        {
            led1_off_btnSlot();
            led2_off_btnSlot();
            led3_off_btnSlot();
        }
    }

    ui->illTextBrowser->setText(ill);
    ui->humTextBrowser_2->setText(hum);
    ui->temTextBrowser_2->setText(tem);
}
//开启/关闭智能检测
void MainWindow::abn_pushbutton_ONSlot()
{
    abnormalSwitch = true;
    QMessageBox::information(this, tr("information"),"开启智能检测成功");
    ui->pushButton_abn_ON->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/OpenMonitoring.png);\n"
"border-style:outset;"));
    ui->pushButton_abn_OFF->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disCloseMonitoring.png);\n"
"border-style:outset;"));
}
void MainWindow::abn_pushbutton_OFFSlot()
{
    abnormalSwitch =false;
    ui->pushButton_abn_ON->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disOpenMonitoring.png);\n"
"border-style:outset;"));
    ui->pushButton_abn_OFF->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/CloseMonitoring.png);\n"
"border-style:outset;"));
    QMessageBox::information(this, tr("information"),"关闭智能检测成功");
}
/*****************************  设置异常阈值  *************************************/
void MainWindow::pushButton_abn_ledSlot()
{
    if(lineEdit_led_lv1->text()!=""&&lineEdit_led_lv2->text()!=""&&lineEdit_led_lv3->text()!="")
    {
        ill_lv1 =lineEdit_led_lv1->text();
        ill_lv2 =lineEdit_led_lv2->text();
        ill_lv3 =lineEdit_led_lv3->text();
        QMessageBox::information(this,tr("information"), tr("修改成功"));
    }
    else
        QMessageBox::warning(this,tr("warning"), tr("阈值不能为空"));

}
void MainWindow::pushButton_abn_fanSlot()
{
    if(lineEdit_fan_tem->text()!="")
    {
        tem_max = lineEdit_fan_tem->text();
        QMessageBox::information(this,tr("information"), tr("修改成功"));
    }
    else
        QMessageBox::warning(this,tr("information"), tr("风扇开启温度阈值不能为空"));
}
void MainWindow::abn_keyboardshow_ledlv1(QString data)
{
    syszuxpinyin_ledlv1->lineEdit_window->setText(data);
    syszuxpinyin_ledlv1->resize(800,310);
    syszuxpinyin_ledlv1->move(120,300);
    syszuxpinyin_ledlv1->show();
}
void MainWindow::abn_keyboardshow_ledlv2(QString data)
{
    syszuxpinyin_ledlv2->lineEdit_window->setText(data);
    syszuxpinyin_ledlv2->resize(800,310);
    syszuxpinyin_ledlv2->move(120,300);
    syszuxpinyin_ledlv2->show();
}
void MainWindow::abn_keyboardshow_ledlv3(QString data)
{
    syszuxpinyin_ledlv3->lineEdit_window->setText(data);
    syszuxpinyin_ledlv3->resize(800,310);
    syszuxpinyin_ledlv3->move(120,300);
    syszuxpinyin_ledlv3->show();
}
void MainWindow::abn_keyboardshow_fan(QString data)
{
    syszuxpinyin_fan->lineEdit_window->setText(data);
    syszuxpinyin_fan->resize(800,310);
    syszuxpinyin_fan->move(120,300);
    syszuxpinyin_fan->show();
}
void MainWindow::abn_confirmString_ledlv1(QString gemfield)  //接收键盘发过来的数据
{
     lineEdit_led_lv1->setText(gemfield);
}
void MainWindow::abn_confirmString_ledlv2(QString gemfield)  //接收键盘发过来的数据
{
     lineEdit_led_lv2->setText(gemfield);
}
void MainWindow::abn_confirmString_ledlv3(QString gemfield)  //接收键盘发过来的数据
{
     lineEdit_led_lv3->setText(gemfield);
}
void MainWindow::abn_confirmString_fan(QString gemfield)     //接收键盘发过来的数据
{
     lineEdit_fan_tem->setText(gemfield);
}
// 函数: 打开LED
void turn_led_on(const char *chipname, int line_num) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;

    // 打开GPIO芯片
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("Failed to open GPIO chip");
        return;
    }

    // 获取GPIO引脚
    line = gpiod_chip_get_line(chip, line_num);
    if (!line) {
        perror("Failed to get GPIO line");
        gpiod_chip_close(chip);
        return;
    }

    // 配置GPIO引脚为输出模式，值为1（打开LED）
    if (gpiod_line_request_output(line, "led-on", 1)) {
        perror("Failed to request GPIO line as output");
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return;
    }

//    printf("LED is ON\n");
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}

// 函数: 关闭LED
void turn_led_off(const char *chipname, int line_num) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;

    // 打开GPIO芯片
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("Failed to open GPIO chip");
        return;
    }

    // 获取GPIO引脚
    line = gpiod_chip_get_line(chip, line_num);
    if (!line) {
        perror("Failed to get GPIO line");
        gpiod_chip_close(chip);
        return;
    }

    // 配置GPIO引脚为输出模式，值为0（关闭LED）
    if (gpiod_line_request_output(line, "led-off", 0)) {
        perror("Failed to request GPIO line as output");
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return;
    }

//    printf("LED is OFF\n");
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    // 注意：通常在程序结束或者不再需要控制LED时释放GPIO资源。
}

/*******************************  控制模块  ****************************************/
void MainWindow::led1_on_btnSlot()
{
    std::cout<<"led1 on\n";
    ui->pushButton_led1_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/open.png);\n"
"border-style:outset;"));
    ui->pushButton_led1_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disclose.png);\n"
"border-style:outset;"));
    turn_led_on("gpiochip4", 11);
}
void MainWindow::led1_off_btnSlot()
{
    std::cout<<"led1 off\n";
    ui->pushButton_led1_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disopen.png);\n"
"border-style:outset;"));
    ui->pushButton_led1_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/close.png);\n"
"border-style:outset;"));
    turn_led_off("gpiochip4", 11);
}
void MainWindow::led2_on_btnSlot()
{ 
    std::cout<<"led2 on\n";
    ui->pushButton_led2_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/open.png);\n"
"border-style:outset;"));
    ui->pushButton_led2_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disclose.png);\n"
"border-style:outset;"));
    turn_led_on("gpiochip1", 12);
}
void MainWindow::led2_off_btnSlot()
{
    std::cout<<"led2 off\n";
    ui->pushButton_led2_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disopen.png);\n"
"border-style:outset;"));
    ui->pushButton_led2_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/close.png);\n"
"border-style:outset;"));
    turn_led_off("gpiochip1", 12);
}
void MainWindow::led3_on_btnSlot()
{
    std::cout<<"led3 on\n";
    ui->pushButton_led3_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/open.png);\n"
"border-style:outset;"));
    ui->pushButton_led3_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disclose.png);\n"
"border-style:outset;"));
    turn_led_on("gpiochip1", 1);
}
void MainWindow::led3_off_btnSlot()
{
    std::cout<<"led3 off\n";
    ui->pushButton_led3_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disopen.png);\n"
"border-style:outset;"));
    ui->pushButton_led3_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/close.png);\n"
"border-style:outset;"));
    turn_led_off("gpiochip1", 1);
}
void MainWindow::fan_on_btnSlot()
{
//    ui->pushButton_fan_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/lv1.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disclose.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_2->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv2.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_3->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv3.png);\n"
//"border-style:outset;"));
//    if(board_type==BOARD_TYPE_FSMP1A)
//        system("echo 50 > /sys/class/hwmon/hwmon1/pwm1");
//    else if(board_type==BOARD_TYPE_FSMP1C)
//    {
//        unsigned char arg;
//        Ioctl(EXIT_FAN,&arg);
//        Ioctl(INIT_FAN,&arg);
//        Ioctl(FAN_UP,&arg);
//    }
}
void MainWindow::fan_off_btnSlot()
{
//    ui->pushButton_fan_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv1.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/close.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_2->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv2.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_3->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv3.png);\n"
//"border-style:outset;"));
//    if(board_type==BOARD_TYPE_FSMP1A)
//        system("echo 0 > /sys/class/hwmon/hwmon1/pwm1");
//    else if(board_type==BOARD_TYPE_FSMP1C)
//    {
//        unsigned char arg;
//        Ioctl(EXIT_FAN,&arg);
//    }
}
void MainWindow::fan_2_btnSlot()
{
//    ui->pushButton_fan_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv1.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disclose.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_2->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/lv2.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_3->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv3.png);\n"
//"border-style:outset;"));

//     if(board_type==BOARD_TYPE_FSMP1A)
//         system("echo 100 > /sys/class/hwmon/hwmon1/pwm1");
//     else if(board_type==BOARD_TYPE_FSMP1C)
//     {
//         unsigned char arg;
//         Ioctl(EXIT_FAN,&arg);
//         Ioctl(INIT_FAN,&arg);
//         for (int i=0;i<4;i++) {
//             Ioctl(FAN_UP,&arg);
//         }
//     }
}
void MainWindow::fan_3_btnSlot()
{
//    ui->pushButton_fan_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv1.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disclose.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_2->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/dislv2.png);\n"
//"border-style:outset;"));
//    ui->pushButton_fan_3->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/lv3.png);\n"
//"border-style:outset;"));
//    if(board_type==BOARD_TYPE_FSMP1A)
//        system("echo 255 > /sys/class/hwmon/hwmon1/pwm1");
//    else if(board_type==BOARD_TYPE_FSMP1C)\
//    {
//        unsigned char arg;
//        Ioctl(EXIT_FAN,&arg);
//        Ioctl(INIT_FAN,&arg);
//        for (int i=0;i<9;i++) {
//            Ioctl(FAN_UP,&arg);
//        }
//    }
}
void MainWindow::beep_on_btnSlot()
{
    ui->pushButton_beep_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/open.png);\n"
"border-style:outset;"));
    ui->pushButton_beep_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disclose.png);\n"
"border-style:outset;"));
    turn_led_off("gpiochip2",10);
//    beepring();
}
void MainWindow::beep_off_btnSlot()
{
    ui->pushButton_beep_on->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disopen.png);\n"
"border-style:outset;"));
    ui->pushButton_beep_off->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/close.png);\n"
"border-style:outset;"));
    turn_led_on("gpiochip2",10);
//    beepunring();
}
/***************************   连接百度云模块  **************************************/
//获取当前时间戳
void MainWindow::pushButton_gettimeSlot()
{
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    currentTimestamp = QString::number(timeT);
//    ui->lineEdit_time->setText(currentTimestamp);
}
//计算连接信息
void MainWindow::pushButton_calculateSlot()
{
    username.clear();
    password.clear();
    password_md5.clear();
    brokerAddr.clear();
//    ioTCoreld = lineEdit_coreid->text();
//    deviceKey = lineEdit_devkey->text();
//    deviceSecret = lineEdit_devsecret->text();
    ioTCoreld = "akaveqc";
    deviceKey = "device";
    deviceSecret = "mqdBApqXmUPApPtd";
    brokerPort = "1883";
    currentTimestamp="0";
    if(ui->comboBox_city->currentText() =="广州")
        brokerAddr = brokerAddr.append(ioTCoreld).append(".iot.").append("gz").append(".baidubce.com");
    else
        brokerAddr = brokerAddr.append(ioTCoreld).append(".iot.").append("bj").append(".baidubce.com");
    clientId = "zhjt123";
    username = username.append("thingidp").append("@").append(ioTCoreld).append("|").append(deviceKey)\
            .append("|").append(currentTimestamp).append("|").append("MD5");
    password = password.append(deviceKey).append("&").append(currentTimestamp).append("&").append("MD5")\
            .append(deviceSecret);
    QByteArray password_md5result;
    QCryptographicHash md(QCryptographicHash::Md5);
    md.addData(password.toUtf8());
    password_md5result = md.result();
    password_md5.append(password_md5result.toHex());
    ui->pushButton_connectmqtt->setEnabled(true);
    QMessageBox::information(this, tr("information"),"计算完成，请点击连接");
}
//连接
void MainWindow::pushButton_connectmqttSlot()
{
//    qDebug()<<"brokerAddr ="<< brokerAddr;
//    qDebug()<<"username ="<< username;
//    qDebug()<<"password ="<< password_md5;
//    qDebug()<<mqttTopic;
    connect(m_client,SIGNAL(connected()),this,SLOT(mqttconnectSlot()));
    //未连接服务器则连接
    if (m_client->state() == QMqttClient::Disconnected) {
        ui->pushButton_connectmqtt->setText(tr(""));
        m_client->setHostname(brokerAddr);
        m_client->setPort(brokerPort.toInt());
        m_client->setUsername(username);
        m_client->setPassword(password_md5);
        m_client->connectToHost();
        QMessageBox::information(this, tr("information"),"连接成功");
        //     定时器初始化
        InitTimer();
        connect(m_client, SIGNAL(messageReceived(const QByteArray, const QMqttTopicName)),
                   this, SLOT(messageReceived(const QByteArray, const QMqttTopicName)));

//        connect(facerec, &faceRecThread::alarm, this, &MainWindow::alarmStranger);

        ui->pushButton_connectmqtt->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/disconnect.png);\n"
"border-style:outset;"));

    } else {//断开连接
        ui->pushButton_connectmqtt->setStyleSheet(QString::fromUtf8("background-image:url(:/icon/4.png);\n"
"border-style:outset;"));
         QMessageBox::information(this, tr("information"),"断开连接");
        m_client->disconnectFromHost();
    }
}

void MainWindow::ito_keyboardshow_coreldSlot(QString data)
{
    syszuxpinyin_iotCoreid->lineEdit_window->setText(data);
    syszuxpinyin_iotCoreid->resize(800,310);
    syszuxpinyin_iotCoreid->move(120,300);
    syszuxpinyin_iotCoreid->show();
}
void MainWindow::ito_keyboardshow_devKeySlot(QString data)
{
    syszuxpinyin_iotDevKey->lineEdit_window->setText(data);
    syszuxpinyin_iotDevKey->resize(800,310);
    syszuxpinyin_iotDevKey->move(120,300);
    syszuxpinyin_iotDevKey->show();
}
void MainWindow::ito_keyboardshow_devSecretSlot(QString data)
{
    syszuxpinyin_iotDevSecret->lineEdit_window->setText(data);
    syszuxpinyin_iotDevSecret->resize(800,310);
    syszuxpinyin_iotDevSecret->move(120,300);
    syszuxpinyin_iotDevSecret->show();
}
void MainWindow::ito_confirmString_coreldSlot(QString gemfield)  //接收键盘发过来的数据
{
    lineEdit_coreid->setText(gemfield);
}
void MainWindow::ito_confirmString_devKeySlot(QString gemfield)   //接收键盘发过来的数据
{
    lineEdit_devkey->setText(gemfield);
}
void MainWindow::ito_confirmString_devSecretSlot(QString gemfield)  //接收键盘发过来的数据
{
   lineEdit_devsecret->setText(gemfield);
}

void MainWindow::InitTimer()
{
    m_timer = new QTimer;
    //设置定时器是否为单次触发。默认为 false 多次触发
    m_timer->setSingleShot(false);
    //启动或重启定时器, 并设置定时器时间：毫秒
    m_timer->start(5000);
    //定时器触发信号槽
    connect(m_timer,SIGNAL(timeout()),this,SLOT(TimertimeOut()));
}

void MainWindow::TimertimeOut()
{
    mqttTopic ="$iot/device/user/fortest";
    mqttMessage.clear();
    mqttMessage.append("{\"temperature").append("\"").append(":").append(tem)\
            .append(",").append("\"humidity").append("\"").append(":").append(hum)\
            .append(",").append("\"illumination").append("\"").append(":").append(ill)\
            .append("}");
    //执行定时器触发时需要处理的业务
    //        发布
    if(m_client->publish(mqttTopic,mqttMessage.toUtf8()) == -1)
    {
        QMessageBox::critical(this,"Error","连接断开或输入的topic有误，无法发布",QMessageBox::Yes);
        m_timer->stop();   //停止定时器
    }
//    if(true){
//        std::cout<<"stranger in!\n";
//        mqttTopic ="$iot/device/user/detection";
//        mqttMessage.clear();
//        mqttMessage.append("{\"stranger").append("\"").append(":").append("true")\
//                .append("}");
//        if(m_client->publish(mqttTopic,mqttMessage.toUtf8()) == -1)
//        {
//            QMessageBox::critical(this,"Error","连接断开或输入的topic有误，无法发布",QMessageBox::Yes);
//            m_timer->stop();   //停止定时器
//        }
//    }
}
void MainWindow::mqttconnectSlot()
{
    qDebug()<<"sub";
    QString subScribeTopic ="$iot/device/user/control";
    m_client->subscribe(subScribeTopic);
}
void MainWindow::alarmStranger()
{
    std::cout<<"stranger in!\n";
    mqttTopic ="$iot/device/user/detection";
    mqttMessage.clear();
    mqttMessage.append("{\"stranger").append("\"").append(":").append("true")\
            .append("}");
    if(m_client->publish(mqttTopic,mqttMessage.toUtf8()) == -1)
    {
        QMessageBox::critical(this,"Error","连接断开或输入的topic有误，无法发布",QMessageBox::Yes);
        m_timer->stop();   //停止定时器
    }
}
void MainWindow::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug()<<"messageReceived:"<<topic.name()<<QString(message);
    QJsonObject json_object = QJsonDocument::fromJson(message).object();
//    qDebug() <<json_object;
//    qDebug() <<json_object.value("led1").toInt();
    QStringList jsonlist = json_object.keys();
    QString key = jsonlist[jsonlist.size()-1];
    if(key == "led1")
    {
        if(json_object.value("led1").toInt() ==1)
            led1_on_btnSlot();
        else
            led1_off_btnSlot();
    }
    else if (key == "led2")
    {
        if(json_object.value("led2").toInt() ==1)
            led2_on_btnSlot();
        else
            led2_off_btnSlot();
    }
    else if (key == "led3")
    {
        if(json_object.value("led3").toInt() ==1)
            led3_on_btnSlot();
        else
            led3_off_btnSlot();
    }
    else if (key == "fan")
    {
        if(json_object.value("fan").toInt() ==0)
            fan_off_btnSlot();
        else
        {
           fan_3_btnSlot();
        }
    }
    else if (key == "beep")
    {
        if(json_object.value("beep").toInt() ==1)
            beep_on_btnSlot();
        else
            beep_off_btnSlot();
    }
}
/********************************  门禁登陆  *****************************************/
void MainWindow::loginSlot()
{
     disconnect(&pe15thread,SIGNAL(pesig()),this,SLOT(loginSlot()));
     login->show();
     connect(login,SIGNAL(loginsuccess()),this,SLOT(loginsuccessSlot()));
     connect(login,SIGNAL(loginfailed()),this,SLOT(loginfailedSlot()));
     connect(login,SIGNAL(loginclose()),this,SLOT(logincloseSlot()));
}
void MainWindow::loginsuccessSlot()
{
     QMessageBox::information(this, tr("information"),"密码正确，门锁已打开");
//     重置错误次数
     Numberoferrors =3;
     login->close();
     regist->show();
     beepunring();
     connect(&pe15thread,SIGNAL(pesig()),this,SLOT(loginSlot()));
}
void MainWindow::loginfailedSlot()
{
    QString info;
    Numberoferrors--;
    switch(Numberoferrors)
    {

    case 2:
        info ="密码错误,还有3次机会";
        break;
    case 1:
        info ="密码错误,还有2次机会";
        break;
    case 0:
        info ="密码错误,还有1次机会";
        break;
    default:
        info ="即将报警";
        break;
    }

    QMessageBox::warning(this, tr("warning"),info);
    if(Numberoferrors <0)
    {
        qDebug() << Numberoferrors;
        beepring();
    }
}
void MainWindow::logincloseSlot()
{
     connect(&pe15thread,SIGNAL(pesig()),this,SLOT(loginSlot()));
}
void MainWindow::beepring()
{
    int fd;
    struct input_event event;
    struct timeval time;
    fd = open("/dev/input/by-path/platform-beeper-event", O_RDWR);
    event.type = EV_SND;
    event.code = SND_TONE;
    event.value = 1000;
    time.tv_sec = 1;
    time.tv_usec = 0;
    event.time = time;
    write(fd, &event, sizeof(struct input_event));
}
void MainWindow::beepunring()
{
    int fd;
    struct input_event event;
    struct timeval time;
    fd = open("/dev/input/by-path/platform-beeper-event", O_RDWR);
    event.type = EV_SND;
    event.code = SND_TONE;
    event.value = 0;
    time.tv_sec = 0;
    time.tv_usec = 0;
    event.time = time;
    write(fd, &event, sizeof(struct input_event));
}

void MainWindow::timeUpdate()
{
   QDateTime CurrentTime=QDateTime::currentDateTime();
   QString Timestr=CurrentTime.toString(" yyyy年MM月dd日 hh:mm:ss "); //设置显示的格式
   ui->timeTextBrowser->setText(Timestr);
}
void MainWindow::Ioctl(unsigned long cmd, void* arg)
{
    int fd;
    fd = open("/dev/sensor-manage", O_RDWR);
    if(fd < 0) {
        perror("open");
    }
    if(ioctl(fd, cmd, arg))
        perror("ioctl");
}



void MainWindow::on_quitappioT_clicked()
{
    this->close();
    QApplication::quit();
}

void MainWindow::on_quitappabnormal_clicked()
{
    this->close();
    QApplication::quit();
}

void MainWindow::on_quitappcotrol_clicked()
{
    this->close();
    QApplication::quit();
}

void MainWindow::on_quitapphumiture_clicked()
{
    this->close();
    QApplication::quit();
}

void MainWindow::on_quitappweather_clicked()
{
    this->close();
    QApplication::quit();
}

void MainWindow::on_quitappwifi_clicked()
{
    this->close();
    QApplication::quit();
}
