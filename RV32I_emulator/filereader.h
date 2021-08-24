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

private:
    QString filePath;
    QString text;
    int pc;

};

#endif // FILEREADER_H
