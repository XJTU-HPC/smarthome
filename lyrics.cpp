#include "lyrics.h"
extern bool Isequal;
QList<QString> Lyrics::getListLyricsText() const
{
    return listLyricsText;
}

QList<int> Lyrics::getListLyricsTime() const
{
    return listLyricsTime;
}

Lyrics::Lyrics(QString lyricsPath)
{
    this->lyricsPath = lyricsPath;
}
Lyrics::Lyrics()
{
}

bool Lyrics::readLyricsFile(QString lyricsPath,bool Isequal)
{

      QFile file(lyricsPath);
      qDebug()<<"lyricsPath = "<<lyricsPath;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        listLyricsText.clear();
        listLyricsTime.clear();
        return false;
    }
    if(Isequal == false){
        listLyricsText.clear();
        listLyricsTime.clear();
    }
    qDebug()<<"list"<<listLyricsTime.size();
    QString line="";
    while((line=file.readLine())>nullptr){
        analysisLyricsFile(line);
    }

    return true;
}

bool Lyrics::analysisLyricsFile(QString line)
{

    if(line == nullptr || line.isEmpty()){
        qDebug()<<"this line is empty!";
        return false;
    }
    QRegularExpression regularExpression("\\[(\\d+)?:(\\d+)?(\\.\\d+)?\\](.*)?");
    int index = 0;
    QRegularExpressionMatch match;
    match = regularExpression.match(line, index);
    if(match.hasMatch()) {
        int totalTime;
        totalTime = match.captured(1).toInt() * 60000 + match.captured(2).toInt() * 1000;                    /*  计算该时间点毫秒数            */
        QString currentText =QString::fromStdString(match.captured(4).toStdString());      /*   获取歌词文本*/
        listLyricsText.push_back(currentText);
        listLyricsTime.push_back(totalTime);
        return true;
    }
    return false;
}
int Lyrics::FindThelyrics(qint64 Position) const{

    int pos = -1;
    for(int i = 0; i< listLyricsTime.size()-1;++i){
        if(Position >= listLyricsTime.at(i) && Position < listLyricsTime.at(i+1)){
            pos = i;
        }
    }

    return pos;
}
