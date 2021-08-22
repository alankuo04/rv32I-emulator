#include "elfreader.h"

ElfReader::ElfReader()
{
    filePath = "";
    text = "";
}

void ElfReader::setFilePath(QString filePath)
{
    this->filePath = filePath;
}

QString ElfReader::getFilePath()
{
    return filePath;
}

bool ElfReader::isElfFile()
{
    return text.at(0)==(char)127 && text.at(1)=='E' && text.at(2)=='L' && text.at(3)=='F';
}

void ElfReader::setText(QString text)
{
    this->text= text;
}

QString ElfReader::getText()
{
    return text;
}
