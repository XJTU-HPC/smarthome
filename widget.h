#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QThread>
#include "lyrics.h"
#include <QDir>
#include <QDirIterator>
#include <QDebug>

#define SYSTEM_SET_PCM_VOLUME "amixer -c 0 cset numid=1"  //这个宏定义代表了设置 PCM（脉冲编码调制）音量的命令,用于调整音量设置
#define SYSTEM_SET_ANALOG_VOLUME "amixer -c 0 cset numid=3"  //这个宏定义代表了设置模拟音量的命令,用于调整音频输出的模拟信号级别
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void scanMusicDirectory(const QString &directoryPath);

private slots:
    void addSongs();
    void playPause();
    void playNext();
    void playPrevious();
    void posChanged(qint64 pos);
    void durChanged(qint64 dur);
    void setPos(int pos);
    void clearList();
    void delectSong();
    void volumeSilence();
    void setVol(int vol);
    void hideShow();
    void redVolume();
    void addVolume();
    void setPlayTime();
    void on_quitapp_clicked();

private:
    Ui::Widget *ui;
private:
    QMediaPlayer* player;
    QMediaPlaylist* playList;
    QAction* action_hideShow;
    QAction* action_addSongs;
    QAction* action_playPause;
    QAction* action_stop;
    QAction* action_previous;
    QAction* action_next;
    QAction* action_addVolume;
    QAction* action_redVolume;
    QAction* action_quit;
    Lyrics lyrics;
    int lyricsID = 0;
    QString playPath;
     QString presentTime;
     QString totalTime;
     qint64 Position;
     bool Isequal = false;
     bool Onlyone = false;
     int original = -1;
     int index = -1;
protected:
    void contextMenuEvent(QContextMenuEvent* );
};
#endif // WIDGET_H
