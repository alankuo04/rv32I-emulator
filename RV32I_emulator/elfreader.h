#ifndef ELFREADER_H
#define ELFREADER_H

#include<QString>

class ElfReader
{
public:
    ElfReader();
    void loadFile(QString filePath);
    void setFilePath(QString filePath);
    QString getFilePath();
    bool isElfFile();
    void setText(QString text);
    QString getText();

private:
    QString filePath;
    QString text;
    int pc;

};

#endif // ELFREADER_H
