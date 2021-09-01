#ifndef ELFREADER_H
#define ELFREADER_H

#include <QString>
#include "elfheader.h"

class ElfReader
{
public:
    ElfReader(QString filePath);
    QString getTextSection();
    Program_Header* getProgramHeader();
    QMap<QString, int>* getSectionMap();
    int getEntry();
    bool isElf();

private:
    ELF_Header *elf_header = nullptr;
    Program_Header *program_header = nullptr;
    Section_Header *section_header = nullptr;
    char *string_table = nullptr;
    QMap<QString, int> *sectionMap = nullptr;
    Text_section *text = nullptr;
    bool isElfFile;
};

#endif // ELFREADER_H
