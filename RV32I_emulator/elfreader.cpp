#include <QFile>
#include <QDebug>
#include "elfreader.h"
#include "disassembler.h"
#include <string>
#include <sstream>

ElfReader::ElfReader(QString filePath)
{
    QFile file(filePath);
    Section_Header *temp = new Section_Header;
    isElfFile = false;
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            char buf[1024];
            elf_header = new ELF_Header;
            file.read(buf, sizeof(ELF_Header));
            memcpy(elf_header, buf, sizeof(ELF_Header));
            if(elf_header->e_ident[0]==0x7f && elf_header->e_ident[1]=='E' && elf_header->e_ident[2]=='L' && elf_header->e_ident[3]=='F')
            {
                isElfFile = true;
                program_header = new Program_Header[elf_header->e_phnum];
                file.seek(elf_header->e_phoff);
                for(int i=0;i<elf_header->e_phnum;i++)
                {
                    file.read(buf, sizeof(Program_Header));
                    memcpy((program_header+i), buf, sizeof(Program_Header));
                }
                section_header = new Section_Header[elf_header->e_shnum];
                file.seek(elf_header->e_shoff);
                for(int i=0;i<elf_header->e_shnum;i++)
                {
                    file.read(buf, sizeof(Section_Header));
                    memcpy((section_header+i), buf, sizeof(Section_Header));
                }

                file.seek(elf_header->e_shoff+elf_header->e_shstrndx*elf_header->e_shentsize);
                file.read(buf, sizeof(Section_Header));
                memcpy(temp, buf, sizeof(Section_Header));
                file.seek(temp->sh_offset);
                string_table = new char[temp->sh_size];
                file.read(string_table, temp->sh_size);
                text = new Text_section;
                sectionMap = new QMap<QString, int>;
                for(int i=0;i<elf_header->e_shnum;i++)
                {
                    std::stringstream ss(string_table+(section_header+i)->sh_name);
                    std::string name;
                    ss>>name;
                    sectionMap->insert(QString::fromStdString(name), (int)(section_header+i)->sh_addr);
                    if(name == ".text")
                    {
                        file.seek((section_header+i)->sh_offset);
                        text->text_section = new uint32_t[(section_header+i)->sh_size/sizeof(uint32_t)];
                        text->size = (section_header+i)->sh_size/sizeof(uint32_t);
                        file.read((char*)text->text_section, (section_header+i)->sh_size);
                        file.flush();
                    }
                }
                file.close();
            }
        }
    }
    delete temp;
}

QString ElfReader::getTextSection()
{
    QString temp = "";
    //qDebug()<<text->size;
    for(int i=0;i<text->size;i++)
    {
        Disassembler instruction(text->text_section[i]);
        temp.append(QString::number(elf_header->e_entry+i*4, 16).toUpper()+": "+instruction.getInstruction());
    }
    return temp;
}

Program_Header* ElfReader::getProgramHeader()
{
    return program_header;
}

QMap<QString, int>* ElfReader::getSectionMap()
{
    return sectionMap;
}

int ElfReader::getEntry()
{
    return elf_header->e_entry;
}

bool ElfReader::isElf()
{
    return isElfFile;
}
