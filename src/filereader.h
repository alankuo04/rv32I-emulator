#ifndef FILEREADER_H
#define FILEREADER_H

#include<QString>

class FileReader
{
public:
    FileReader();
    void loadFile(QString filePath);
    void setFilePath(QString filePath);
    QString getFilePath();
    void setText(QString text);
    QString getText();
    int getInterval();
    void setInterval(int interval);
    bool isStop();
    void setStop(bool stop);

private:
    QString filePath;
    QString text;
    int interval;
    bool stop;
};

#endif // FILEREADER_H
