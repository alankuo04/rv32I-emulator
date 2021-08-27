#include "filereader.h"

FileReader::FileReader()
{
    filePath = "";
    text = "";
    interval = 1000;
    stop = false;
}

void FileReader::setFilePath(QString filePath)
{
    this->filePath = filePath;
}

QString FileReader::getFilePath()
{
    return filePath;
}

void FileReader::setText(QString text)
{
    this->text= text;
}

QString FileReader::getText()
{
    return text;
}

int FileReader::getPC()
{
    return pc;
}

void FileReader::setPC(int pc)
{
    this->pc = pc;
}

int FileReader::getInterval()
{
    return interval;
}

void FileReader::setInterval(int interval)
{
    this->interval = interval;
}

bool FileReader::isStop()
{
    return stop;
}

void FileReader::setStop(bool stop)
{
    this->stop = stop;
}
