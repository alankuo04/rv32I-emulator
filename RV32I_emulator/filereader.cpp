#include "filereader.h"

FileReader::FileReader()
{
    filePath = "";
    text = "";
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

