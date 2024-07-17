#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMqtt/qmqttclient.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QTimer>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <QStringListModel>
#include "set_wifi.h"
#include "collentdatathread.h"
#include "readpe15thread.h"
#include "login.h"
#include "register.h"
#include "facerecthread.h"
#include "speechrecthread.h"
#include "erniellm.h"
#include "chatitemdelegate.h"
#include "agentspeak.h"
#include "speechplayerthread.h"
#include "screensaver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    /******************************   UI Control Variable  *********************************/
    QMovie *currentMovie;
    bool livingLED;
    bool bedLED;
    bool secbedLED;
    bool beep;
    bool remoteconnect;
    bool autocontrol;
    ernieLLM *llm;
    QString usertext;
    QString llmtext;
    QStandardItemModel *dialoglist;
    ChatItemDelegate *chatitem;
    bool usersaying; //whether now user saying
    bool agentspeaking;
    agentSpeak *agentspeak;
    bool speakruning;
    speechPlayerThread *speechplayer;
    screenSaver* screensaver;
    QTimer* idleTimer;

protected:
    void resetIdleTimer() {
        screensaver->hideSaver();
//        ui->tabWidget->setCurrentIndex(0);
//        if (screensaver->isVisible()) {
//            screensaver->hideSaver();
//        }
        idleTimer->start(30000); // 重新启动定时器
    }

    bool event(QEvent* event) override {
        // 监听用户所有活动事件
        switch (event->type()) {
            case QEvent::MouseMove:
            case QEvent::MouseButtonPress:
            case QEvent::KeyPress:
                resetIdleTimer();
                break;
            default:
                break;
        }
        return QWidget::event(event);
    }
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     Set_Wifi *WifiDlg;
     void Get_Wifi_Name();
     SyszuxPinyin *syszuxpinyin_ledlv1;
     SyszuxPinyin *syszuxpinyin_ledlv2;
     SyszuxPinyin *syszuxpinyin_ledlv3;
     SyszuxPinyin *syszuxpinyin_fan;
     SyszuxPinyin *syszuxpinyin_iotCoreid;
     SyszuxPinyin *syszuxpinyin_iotDevKey;
     SyszuxPinyin *syszuxpinyin_iotDevSecret;
     void updateImage(const QImage &image);
private slots:
    void on_connectBTN_clicked();
    void on_connectBTN_2_clicked();
    void replyFinished(QNetworkReply *reply); //天气数据处理槽函数
    void weather_cilcked_Slot();
    void set_humAdtemAdill(QString tem,QString hum,QString ill);
    /******************************   UI   *********************************/
    void on_llmButton_clicked();
    void onTabChanged(int index);
    void on_weatherButton_clicked();
    void on_WLANButton_clicked();
    void on_LEDButton_clicked();
    void on_cameraButton_clicked();
    void on_exitButton_clicked();
    void on_remoteButton_clicked();
    void on_faceReturnhome_clicked();
    void on_WLANReturnhome_clicked();
    void on_weatherReturnhome_clicked();
    void on_LEDReturnhome_clicked();
    void on_llmReturnhome_clicked();
    /******************************   LEDControl   *********************************/
    void on_livingLED_clicked();
    void on_bedLED_clicked();
    void on_secbedLED_clicked();
    void on_alarm_clicked();
    void on_autocontrolButton_clicked();
    void on_agentspeak_clicked();
    /******************************   LLMControl   *********************************/
    void on_speechButton_pressed();
    void on_speechButton_released();
    void handlespeechOutput(QString output);
    void on_cleardialog_clicked();
    void onStartedPlaying();
    void onFinishedPlaying();
    void showScreenSaver();
    /******************************    控制模块  *********************************/
    void led1_on_btnSlot();
    void led1_off_btnSlot();
    void led2_on_btnSlot();
    void led2_off_btnSlot();
    void led3_on_btnSlot();
    void led3_off_btnSlot();
    void fan_on_btnSlot();
    void fan_off_btnSlot();
    void fan_2_btnSlot();
    void fan_3_btnSlot();
    void beep_on_btnSlot();
    void beep_off_btnSlot();
    /***************************** 设置异常阈值 *****************************/
     void pushButton_abn_ledSlot();
     void pushButton_abn_fanSlot();
     void abn_keyboardshow_ledlv1(QString data);
     void abn_keyboardshow_ledlv2(QString data);
     void abn_keyboardshow_ledlv3(QString data);
     void abn_keyboardshow_fan(QString data);
     void abn_confirmString_ledlv1(QString gemfield);   //接收键盘发过来的数据
     void abn_confirmString_ledlv2(QString gemfield);   //接收键盘发过来的数据
     void abn_confirmString_ledlv3(QString gemfield);   //接收键盘发过来的数据
     void abn_confirmString_fan(QString gemfield);   //接收键盘发过来的数据
     void abn_pushbutton_ONSlot();
     void abn_pushbutton_OFFSlot();
     /***************************   连接百度云模块  **************************************/
     void pushButton_gettimeSlot();
     void pushButton_calculateSlot();
     void pushButton_connectmqttSlot();
     void ito_keyboardshow_coreldSlot(QString data);
     void ito_keyboardshow_devKeySlot(QString data);
     void ito_keyboardshow_devSecretSlot(QString data);
     void ito_confirmString_coreldSlot(QString gemfield);   //接收键盘发过来的数据
     void ito_confirmString_devKeySlot(QString gemfield);   //接收键盘发过来的数据
     void ito_confirmString_devSecretSlot(QString gemfield);   //接收键盘发过来的数据
     void messageReceived(const QByteArray &message, const QMqttTopicName &topic);
//     void alarmStranger();
     void TimertimeOut();
     void mqttconnectSlot();
     /********************************  门禁登陆  *****************************************/
     void loginSlot();
     void loginfailedSlot();
     void loginsuccessSlot();
     void logincloseSlot();
     /******************     time   **********/
     void timeUpdate();

     void on_quitappioT_clicked();

     void on_quitappabnormal_clicked();

     void on_quitapphumiture_clicked();

signals:
    void stopSpeechPlayerThread();

public slots:
     void startFaceRec();
     void stopFaceRec();
     void alarmStranger();

private:
    Ui::MainWindow *ui;
    faceRecThread *facerec;
    speechRecThread *speechThread;
    /****************************** 天气模块 ***************************************/
    //请求句柄
    QNetworkAccessManager *manager;
    void sendQuest(QString cityStr);
    /*****************************  数据采集模块  ***********************************/
    QString hum;
    QString tem;
    QString ill;
    //数据采集线程
    CollentdataThread thread_collentdata;
    /***************************** 异常阈值 *****************************/
    QString tem_max;
    QString ill_lv1;
    QString ill_lv2;
    QString ill_lv3;
    My_lineEdit *lineEdit_led_lv1;
    My_lineEdit *lineEdit_led_lv2;
    My_lineEdit *lineEdit_led_lv3;
    My_lineEdit *lineEdit_fan_tem;
    /***************************   连接百度云模块  **************************************/
    QMqttClient  *m_client;//mqtt client指针
    QString currentTimestamp;
    QString ioTCoreld;
    QString deviceKey;
    QString deviceSecret;
    QString brokerAddr;
    QString brokerPort;
    QString clientId;
    QString username;
    QString password;
    QString password_md5;
    QString mqttTopic;
    QString mqttMessage;
    My_lineEdit *lineEdit_coreid;
    My_lineEdit *lineEdit_devkey;
    My_lineEdit *lineEdit_devsecret;
    QTimer *m_timer;
    void InitTimer();
    /********************************  门禁登陆  *****************************************/
    ReadPE15Thread pe15thread;
    Login *login;
    void beepring();
    void beepunring();
    Register *regist;
      /******************     time   **********/
    QTimer *currentTime;


    void Ioctl(unsigned long cmd, void* arg);
    int board_type = 0;
};

#endif // MAINWINDOW_H
