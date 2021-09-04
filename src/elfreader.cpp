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

QString ElfReader::showElfHeader()
{
    QString temp = "";
    temp += QString("%1").arg("Class:", -36);
    switch (elf_header->e_ident[4]) {
    case 1:
        temp += "ELF32";
        break;
    case 2:
        temp += "ELF64";
        break;
    default:
        temp += "Unknown";
        break;
    }
    temp += "\n";
    temp += QString("%1").arg("Data:", -36);
    switch (elf_header->e_ident[5]) {
    case 1:
        temp += "little endian";
        break;
    case 2:
        temp += "big endian";
        break;
    default:
        temp += "Unknown";
        break;
    }
    temp += "\n";
    temp += QString("%1").arg("Version:", -36);
    switch (elf_header->e_ident[6]) {
    case 1:
        temp += "1(current)";
        break;
    default:
        temp += "Unknown";
        break;
    }
    temp += "\n";
    temp += QString("%1").arg("OS/ABI:", -36);
    switch (elf_header->e_ident[7]) {
    case 0:
        temp += "No extensions or unspecified";
        break;
    case 1:
        temp += "Hewlett-Packard HP-UX";
        break;
    case 2:
        temp += "NetBSD";
        break;
    case 3:
        temp += "Linux";
        break;
    case 6:
        temp += "Sun Solaris";
        break;
    case 7:
        temp += "AIX";
        break;
    case 8:
        temp += "IRIX";
        break;
    case 9:
        temp += "FreeBSD";
        break;
    case 10:
        temp += "IRIX";
        break;
    case 11:
        temp += "Novell Modesto";
        break;
    case 12:
        temp += "Open BSD";
        break;
    case 13:
        temp += "Open VMS";
        break;
    case 14:
        temp += "Hewlett-Packard Non-Stop Kernel";
        break;
    default:
        temp += "Unknown";
        break;
    }
    temp += "\n";
    temp += QString("%1").arg("ABI Version:", -36);
    temp += QString::number(elf_header->e_ident[8]);
    temp += "\n";
    temp += QString("%1").arg("Type:", -36);
    switch (elf_header->e_type) {
    case 1:
        temp += "REL";
        break;
    case 2:
        temp += "EXEC";
        break;
    case 3:
        temp += "DYN";
        break;
    case 4:
        temp += "CORE";
        break;
    case 0xff00:
        temp += "LOPROC";
        break;
    case 0xffff:
        temp += "HIPROC";
        break;
    default:
        temp += "Unknown";
        break;
    }
    temp += "\n";
    temp += QString("%1").arg("Machine:", -36);
    temp += get_e_machine(elf_header->e_machine);
    temp += "\n";
    temp += QString("%1").arg("Version:", -36);
    switch (elf_header->e_version) {
    case 1:
        temp += "0x1";
        break;
    default:
        temp += "Unknown";
        break;
    }
    temp += "\n";
    temp += QString("%1").arg("Entry point address:", -36);
    temp += "0x"+QString::number(elf_header->e_entry, 16);
    temp += "\n";
    temp += QString("%1").arg("Start of program header:", -36);
    temp += QString::number(elf_header->e_phoff);
    temp += "\n";
    temp += QString("%1").arg("Start of section header:", -36);
    temp += QString::number(elf_header->e_shoff);
    temp += "\n";
    temp += QString("%1").arg("Flags:", -36);
    temp += "0x"+QString::number(elf_header->e_flags, 16);
    temp += "\n";
    temp += QString("%1").arg("Size of this header:", -36);
    temp += QString::number(elf_header->e_ehsize);
    temp += "\n";
    temp += QString("%1").arg("Size of program header:", -36);
    temp += QString::number(elf_header->e_phentsize);
    temp += "\n";
    temp += QString("%1").arg("Number of program header:", -36);
    temp += QString::number(elf_header->e_phnum);
    temp += "\n";
    temp += QString("%1").arg("Size of section header:", -36);
    temp += QString::number(elf_header->e_shentsize);
    temp += "\n";
    temp += QString("%1").arg("Number of section header:", -36);
    temp += QString::number(elf_header->e_shnum);
    temp += "\n";
    temp += QString("%1").arg("Section header string table index:", -36);
    temp += QString::number(elf_header->e_shstrndx);

    return temp;
}

QString ElfReader::showProgramHeader()
{
    QString temp = "";
    temp += QString("%1").arg("Type", -10);
    temp += QString("%1 ").arg("Offset", -8);
    temp += QString("%1 ").arg("VirtAddr", -10);
    temp += QString("%1 ").arg("PhysAddr", -10);
    temp += QString("%1 ").arg("FileSiz", -7);
    temp += QString("%1 ").arg("MemSiz", -7);
    temp += QString("%1 ").arg("Flg", -3);
    temp += QString("%1").arg("Align", -6);
    temp += "\n";

    for(int i=0;i<elf_header->e_phnum;i++)
    {
        switch (program_header[i].p_type) {
        case 1:
            temp += QString("%1").arg("LOAD", -10);
            break;
        case 2:
            temp += QString("%1").arg("DYNAMIC", -10);
            break;
        case 3:
            temp += QString("%1").arg("INTERP", -10);
            break;
        case 4:
            temp += QString("%1").arg("NOTE", -10);
            break;
        case 5:
            temp += QString("%1").arg("SHLIB", -10);
            break;
        case 6:
            temp += QString("%1").arg("PHDR", -10);
            break;
        case 0x70000000:
            temp += QString("%1").arg("LOPROC", -10);
            break;
        case 0x7fffffff:
            temp += QString("%1").arg("HIPROC", -10);
            break;
        default:
            temp += QString("%1").arg("Unknown", -10);
            break;
        }

        temp += QString("0x%1 ").arg(program_header[i].p_offset, 6, 16, QLatin1Char('0'));
        temp += QString("0x%1 ").arg(program_header[i].p_vaddr, 8, 16, QLatin1Char('0'));
        temp += QString("0x%1 ").arg(program_header[i].p_paddr, 8, 16, QLatin1Char('0'));
        temp += QString("0x%1 ").arg(program_header[i].p_filesz, 5, 16, QLatin1Char('0'));
        temp += QString("0x%1 ").arg(program_header[i].p_memsz, 5, 16, QLatin1Char('0'));
        switch (program_header[i].p_flags) {
        case 0x0ff00000:
            temp += "O   ";
            //temp += "MASKOS";
            break;
        case 0xf0000000:
            temp += "P   ";
            //temp += "MASKPROC";
            break;
        default:
            temp += (program_header[i].p_flags & 4)?"R":" ";
            temp += (program_header[i].p_flags & 2)?"W":" ";
            temp += (program_header[i].p_flags & 1)?"E":" ";
            temp += " ";
            break;
        }
        temp += QString("0x%1").arg(program_header[i].p_align, 4, 16, QLatin1Char('0'));
        temp += "\n";
    }
    return temp;
}

QString ElfReader::showSectionHeader()
{
    QString temp = "";
    temp += "Name\tType\tAddr\tOffset\tSize\tES\tFlg\tLk\tInf\tAl\n";
    for(int i=0;i<elf_header->e_shnum;i++)
    {
        std::stringstream ss(string_table+section_header[i].sh_name);
        std::string name;
        ss>>name;
        temp += QString::fromStdString(name);
        temp += "\t";
        switch (section_header[i].sh_type) {
        case 0:
            temp += "NULL";
            break;
        case 1:
            temp += "PROGBITS";
            break;
        case 2:
            temp += "SYMTAB";
            break;
        case 3:
            temp += "STRTAB";
            break;
        case 4:
            temp += "RELA";
            break;
        case 5:
            temp += "HASH";
            break;
        case 6:
            temp += "DYNAMIC";
            break;
        case 7:
            temp += "NOTE";
            break;
        case 8:
            temp += "NOBITS";
            break;
        case 9:
            temp += "REL";
            break;
        case 10:
            temp += "SHLIB";
            break;
        case 11:
            temp += "DYNSYM";
            break;
        case 14:
            temp += "INIT_ARRAY";
            break;
        case 15:
            temp += "FINI_ARRAY";
            break;
        case 16:
            temp += "PREINIT_ARRAY";
            break;
        case 17:
            temp += "GROUP";
            break;
        case 18:
            temp += "SYMTAB_SHNDX";
            break;
        case 19:
            temp += "NUM";
            break;
        case 0x60000000:
            temp += "LOOS";
            break;
        case 0x6ffffff5:
            temp += "GNU_ATTRIBUTES";
            break;
        case 0x6ffffff6:
            temp += "GNU_HASH";
            break;
        case 0x6ffffff7:
            temp += "GNU_LIBLIST";
            break;
        case 0x6ffffff8:
            temp += "CHECKSUM";
            break;
        case 0x6ffffffa:
            temp += "LOSUNW";
            break;
        case 0x6ffffffb:
            temp += "SUNW_COMDAT";
            break;
        case 0x6ffffffc:
            temp += "SUNW_syminfo";
            break;
        case 0x6ffffffd:
            temp += "GNU_verdef";
            break;
        case 0x6ffffffe:
            temp += "GNU_verneed";
            break;
        case 0x6fffffff:
            temp += "GNU_versym";
            break;
        case 0x70000000:
            temp += "LOPROC";
            break;
        case 0x7fffffff:
            temp += "HIPROC";
            break;
        case 0x80000000:
            temp += "LOUSER";
            break;
        case 0x8fffffff:
            temp += "HIUSER";
            break;
        }
        temp += "\t";
        QString flags = "";
        flags += (section_header[i].sh_flags & (1<<0))?"W":"";
        flags += (section_header[i].sh_flags & (1<<1))?"A":"";
        flags += (section_header[i].sh_flags & (1<<2))?"X":"";
        flags += (section_header[i].sh_flags & (1<<4))?"M":"";
        flags += (section_header[i].sh_flags & (1<<5))?"S":"";
        flags += (section_header[i].sh_flags & (1<<6))?"I":"";
        flags += (section_header[i].sh_flags & (1<<7))?"L":"";
        flags += (section_header[i].sh_flags & (1<<8))?"O":"";
        flags += (section_header[i].sh_flags & (1<<9))?"G":"";
        flags += (section_header[i].sh_flags & (1<<10))?"T":"";
        flags += (section_header[i].sh_flags & (1<<11))?"C":"";
        flags += (section_header[i].sh_flags & 0x0ff00000)?"o":"";
        flags += (section_header[i].sh_flags & 0xf0000000)?"p":"";
        flags += (section_header[i].sh_flags & (1<<30))?"":"";
        flags += (section_header[i].sh_flags & (1U<<31))?"E":"";
        temp += QString("%1").arg(flags, 3, QLatin1Char(' '));
        temp += "\t";
        temp += QString("%1").arg(section_header[i].sh_addr, 8, 16, QLatin1Char('0'));
        temp += "\t";
        temp += QString("%1").arg(section_header[i].sh_offset, 6, 16, QLatin1Char('0'));
        temp += "\t";
        temp += QString("%1").arg(section_header[i].sh_size, 6, 16, QLatin1Char('0'));
        temp += "\t";
        temp += QString("%1").arg(section_header[i].sh_link, 2, 16, QLatin1Char('0'));
        temp += "\t";
        temp += QString("%1").arg(section_header[i].sh_info, 2, 16, QLatin1Char('0'));
        temp += "\t";
        temp += QString("%1").arg(section_header[i].sh_addralign, 2, 16, QLatin1Char('0'));
        temp += "\t";
        temp += QString("%1").arg(section_header[i].sh_entsize  , 2, 16, QLatin1Char('0'));
    }
    return temp;
}

bool ElfReader::isElf()
{
    return isElfFile;
}

QString ElfReader::get_e_machine(int e_machine)
{
    switch (e_machine) {
    case 0:     return "No machine";
    case 1:     return "AT&T WE 32100";
    case 2:     return "SUN SPARC";
    case 3:     return "Intel 80386";
    case 4:     return "Motorola 68000";
    case 5:     return "Motorola 88000";
    case 6:     return "Intel MCU";
    case 7:     return "Intel 80860";
    case 8:     return "MIPS R3000 big-endian";
    case 9:     return "IBM System/370";
    case 10:    return "MIPS R3000 little-endian";
    case 15:    return "HPPA";
    case 17:    return "Fujitsu VPP500";
    case 18:    return "Sun's v8+";
    case 19:    return "Intel 80960";
    case 20:    return "PowerPC";
    case 21:    return "PowerPC 64-bit";
    case 22:    return "IBM S390";
    case 23:    return "IBM SPU/SPC";
    case 36:    return "NEC V800 series";
    case 37:    return "Fujitsu FR20";
    case 38:    return "TRW RH-32";
    case 39:    return "Motorola RCE";
    case 40:    return "ARM";
    case 41:    return "Digital Alpha";
    case 42:    return "Hitachi SH";
    case 43:    return "SPARC v9 64-bit";
    case 44:    return "Siemens Tricore";
    case 45:    return "Argonaut RISC Core";
    case 46:    return "Hitachi H8/300";
    case 47:    return "Hitachi H8/300H";
    case 48:    return "Hitachi H8S";
    case 49:    return "Hitachi H8/500";
    case 50:    return "Intel Merced";
    case 51:    return "Stanford MIPS-X";
    case 52:    return "Motorola Coldfire";
    case 53:    return "Motorola M68HC12";
    case 54:    return "Fujitsu MMA Multimedia Accelerator";
    case 55:    return "Siemens PCP";
    case 56:    return "Sony nCPU embeeded RISC";
    case 57:    return "Denso NDR1 microprocessor";
    case 58:    return "Motorola Start*Core processor";
    case 59:    return "Toyota ME16 processor";
    case 60:    return "STMicroelectronic ST100 processor";
    case 61:    return "Advanced Logic Corp. Tinyj emb.fam";
    case 62:    return "AMD x86-64 architecture";
    case 63:    return "Sony DSP Processor";
    case 64:    return "Digital PDP-10";
    case 65:    return "Digital PDP-11";
    case 66:    return "Siemens FX66 microcontroller";
    case 67:    return "STMicroelectronics ST9+ 8/16 mc";
    case 68:    return "STmicroelectronics ST7 8 bit mc";
    case 69:    return "Motorola MC68HC16 microcontroller";
    case 70:    return "Motorola MC68HC11 microcontroller";
    case 71:    return "Motorola MC68HC08 microcontroller";
    case 72:    return "Motorola MC68HC05 microcontroller";
    case 73:    return "Silicon Graphics SVx";
    case 74:    return "STMicroelectronics ST19 8 bit mc";
    case 75:    return "Digital VAX";
    case 76:    return "Axis Communications 32-bit emb.proc";
    case 77:    return "Infineon Technologies 32-bit emb.proc";
    case 78:    return "Element 14 64-bit DSP Processor";
    case 79:    return "LSI Logic 16-bit DSP Processor";
    case 80:    return "Donald Knuth's educational 64-bit proc";
    case 81:    return "Harvard University machine-independent object files";
    case 82:    return "SiTera Prism";
    case 83:    return "Atmel AVR 8-bit microcontroller";
    case 84:    return "Fujitsu FR30";
    case 85:    return "Mitsubishi D10V";
    case 86:    return "Mitsubishi D30V";
    case 87:    return "NEC v850";
    case 88:    return "Mitsubishi M32R";
    case 89:    return "Matsushita MN10300";
    case 90:    return "Matsushita MN10200";
    case 91:    return "picoJava";
    case 92:    return "OpenRISC 32-bit embedded processor";
    case 93:    return "ARC International ARCompact";
    case 94:    return "Tensilica Xtensa Architecture";
    case 95:    return "Alphamosaic VideoCore";
    case 96:    return "Thompson Multimedia General Purpose Proc";
    case 97:    return "National Semi. 32000";
    case 98:    return "Tenor Network TPC";
    case 99:    return "Trebia SNP 1000";
    case 100:   return "STMicroelectronics ST200";
    case 101:   return "Ubicom IP2xxx";
    case 102:   return "MAX processor";
    case 103:   return "National Semi. CompactRISC";
    case 104:   return "Fujitsu F2MC16";
    case 105:   return "Texas Instruments msp430";
    case 106:   return "Analog Devices Blackfin DSP";
    case 107:   return "Seiko Epson S1C33 family";
    case 108:   return "Sharp embedded microprocessor";
    case 109:   return "Arca RISC";
    case 110:   return "PKU-Unity & MPRC Peking Uni. mc series";
    case 111:   return "eXcess configurable cpu";
    case 112:   return "Icera Semi. Deep Execution Processor";
    case 113:   return "Altera Nios II";
    case 114:   return "National Semi. CompactRISC CRX";
    case 115:   return "Motorola XGATE";
    case 116:   return "Infineon C16x/XC16x";
    case 117:   return "Renesas M16C";
    case 118:   return "Microchip Technology dsPIC30F";
    case 119:   return "Freescale Communication Engine RISC";
    case 120:   return "Renesas M32C";
    case 131:   return "Altium TSK3000";
    case 132:   return "Freescale RS08";
    case 133:   return "Analog Devices SHARC family";
    case 134:   return "Cyan Technology eCOG2";
    case 135:   return "Sunplus S+core7 RISC";
    case 136:   return "New Japan Radio (NJR) 24-bit DSP";
    case 137:   return "Broadcom VideoCore III";
    case 138:   return "RISC for Lattice FPGA";
    case 139:   return "Seiko Epson C17";
    case 140:   return "Texas Instruments TMS320C6000 DSP";
    case 141:   return "Texas Instruments TMS320C2000 DSP";
    case 142:   return "Texas Instruments TMS320C55x DSP";
    case 143:   return "Texas Instruments App. Specific RISC";
    case 144:   return "Texas Instruments Prog. Realtime Unit";
    case 160:   return "STMicroelectronics 64bit VLIW DSP";
    case 161:   return "Cypress M8C";
    case 162:   return "Renesas R32C";
    case 163:   return "NXP Semi. TriMedia";
    case 164:   return "QUALCOMM DSP6";
    case 165:   return "Intel 8051 and variants";
    case 166:   return "STMicroelectronics STxP7x";
    case 167:   return "Andes Tech. compact code emb. RISC";
    case 168:   return "Cyan Technology eCOG1X";
    case 169:   return "Dallas Semi. MAXQ30 mc";
    case 170:   return "New Japan Radio (NJR) 16-bit DSP";
    case 171:   return "M2000 Reconfigurable RISC";
    case 172:   return "Cray NV2 vector architecture";
    case 173:   return "Renesas RX";
    case 174:   return "Imagination Tech. META";
    case 175:   return "MCST Elbrus";
    case 176:   return "Cyan Technology eCOG16";
    case 177:   return "National Semi. CompactRISC CR16";
    case 178:   return "Freescale Extended Time Processing Unit";
    case 179:   return "Infineon Tech. SLE9X";
    case 180:   return "Intel L10M";
    case 181:   return "Intel K10M";
    case 182:   return "ARM AARCH64";
    case 184:   return "Amtel 32-bit microprocessor";
    case 186:   return "STMicroelectronics STM8";
    case 187:   return "Tileta TILE64";
    case 188:   return "Tilera TILEPro";
    case 189:   return "Xilinx MicroBlaze";
    case 190:   return "NVIDIA CUDA";
    case 191:   return "Tilera TILE-Gx";
    case 192:   return "CloudShield";
    case 193:   return "KIPO-KAIST Core-A 1st gen";
    case 194:   return "KIPO-KAIST Core-A 2nd gen";
    case 195:   return "Synopsys ARCompact V2";
    case 196:   return "Open8 RISC";
    case 197:   return "Renesas RL78";
    case 198:   return "Broadcom VideoCore V";
    case 199:   return "Renesas 78KOR";
    case 200:   return "Freescale 56800EX DSC";
    case 201:   return "Beyond BA1";
    case 202:   return "Beyond BA2";
    case 203:   return "XMOS xCORE";
    case 204:   return "Microchip 8-bit PIC(r)";
    case 210:   return "KM211 KM32";
    case 211:   return "KM211 KMX32";
    case 212:   return "KM211 KMX16";
    case 213:   return "KM211 KMX8";
    case 214:   return "KM211 KVARC";
    case 215:   return "Paneve CDP";
    case 216:   return "Cognitive Smart Memory Processor";
    case 217:   return "Bluechip CoolEngine";
    case 218:   return "Nanoradio Optimized RISC";
    case 219:   return "CSR Kalimba";
    case 220:   return "Zilog Z80";
    case 221:   return "Controls and Data Services VISIUMcore";
    case 222:   return "FTDI Chip FT32";
    case 223:   return "Moxie processor";
    case 224:   return "AMD GPU";
    case 243:   return "RISC-V";
    case 247:   return "Linux BPF -- in-kernel virtual machine";
    default:    return "Unknown";
    }
}
