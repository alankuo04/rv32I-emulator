#ifndef ELFREADER_H
#define ELFREADER_H

#include <QString>
#include "elfheader.h"

class ElfReader
{
public:
    ElfReader(QString text);
    QString getTextSection();

private:
    ELF_Header *elf_header = nullptr;
    Program_Header *program_header = nullptr;
    Section_Header *section_header = nullptr;
    char *string_table = nullptr;
    Text_section *text = nullptr;
};

#endif // ELFREADER_H
