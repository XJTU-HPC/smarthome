#ifndef LYRICS_H
#define LYRICS_H

#include <QWidget>
#include <QString>
#include <QMap>
#include <iostream>
#include <QFile>
#include <QDebug>
#include <QRegExp>
#include <QList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
using namespace std;
class Lyrics
{
private:
    QString lyricsPath;

public:
    QList<QString> listLyricsText;
    QList<int> listLyricsTime;
public:
    Lyrics(QString lyricsPath);
    Lyrics();
    bool readLyricsFile(QString lyricsPath,bool Isequal);
    bool analysisLyricsFile(QString line);
    QList<QString> getListLyricsText() const;
    QList<int> getListLyricsTime() const;
    int FindThelyrics(qint64 Position) const;
};

#endif // LYRICS_H
