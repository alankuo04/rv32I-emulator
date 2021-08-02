#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<sstream>
#include<iomanip>
#include"readelf.h"
using namespace std;

void show_elf_header(ELF_Header* elf_header){
    cout<<left<<setw(20)<<"ELF:";
    switch (elf_header->e_ident[4])
    {
    case 1:
        cout<<left<<setw(20)<<"ELF32"<<endl;
        break;
    case 2:
        cout<<left<<setw(20)<<"ELF64"<<endl;
        break;
    default:
        cout<<left<<setw(20)<<"Unknown"<<endl;
        break;
    }
    cout<<left<<setw(20)<<"Data:";
    switch (elf_header->e_ident[5])
    {
    case 1:
        cout<<left<<setw(20)<<"little endian"<<endl;
        break;
    case 2:
        cout<<left<<setw(20)<<"big endian"<<endl;
        break;
    default:
        cout<<left<<setw(20)<<"Unknown"<<endl;
        break;
    }
    cout<<left<<setw(20)<<"Version:";
    switch (elf_header->e_ident[6])
    {
    case 1:
        cout<<left<<setw(20)<<"1(current)"<<endl;
        break;
    default:
        cout<<left<<setw(20)<<"Unknown"<<endl;
        break;
    }
    cout<<left<<setw(20)<<"OS/ABI:";
    switch(elf_header->e_ident[7]){
    case 0:
        cout<<left<<setw(20)<<"No extensions or unspecified"<<endl;
        break;
    case 1:
        cout<<left<<setw(20)<<"Hewlett-Packard HP-UX"<<endl;
        break;
    case 2:
        cout<<left<<setw(20)<<"NetBSD"<<endl;
        break;
    case 3:
        cout<<left<<setw(20)<<"Linux"<<endl;
        break;
    case 6:
        cout<<left<<setw(20)<<"Sun Solaris"<<endl;
        break;
    case 7:
        cout<<left<<setw(20)<<"AIX"<<endl;
        break;
    case 8:
        cout<<left<<setw(20)<<"IRIX"<<endl;
        break;
    case 9:
        cout<<left<<setw(20)<<"FreeBSD"<<endl;
        break;
    case 10:
        cout<<left<<setw(20)<<"Compaq TRU64 UNIX"<<endl;
        break;
    case 11:
        cout<<left<<setw(20)<<"Novell Modesto"<<endl;
        break;
    case 12:
        cout<<left<<setw(20)<<"Open BSD"<<endl;
        break;
    case 13:
        cout<<left<<setw(20)<<"Open VMS"<<endl;
        break;
    case 14:
        cout<<left<<setw(20)<<"Hewlett-Packard Non-Stop Kernel"<<endl;
        break;
    }
    cout<<left<<setw(20)<<"ABI Version:";
    cout<<left<<setw(20)<<int(elf_header->e_ident[8])<<endl;
    
    cout<<left<<setw(20)<<"Type:";
    switch (elf_header->e_type)
    {
    case 1:
        cout<<left<<setw(20)<<"REL"<<endl;
        break;
    case 2:
        cout<<left<<setw(20)<<"EXEC"<<endl;
        break;
    case 3:
        cout<<left<<setw(20)<<"DYN"<<endl;
        break;
    case 4:
        cout<<left<<setw(20)<<"CORE"<<endl;
        break;
    case 0xff00:
        cout<<left<<setw(20)<<"LOPROC"<<endl;
        break;
    case 0xffff:
        cout<<left<<setw(20)<<"HIPROC"<<endl;
        break;
    default:
        cout<<left<<setw(20)<<"Unknown"<<endl;
        break;
    }
    cout<<left<<setw(20)<<"Machine:";
    switch (elf_header->e_machine)
    {
    case 1:
        cout<<left<<setw(20)<<"AT&T WE 32100"<<endl;
        break;
    case 2:
        cout<<left<<setw(20)<<"SPARC"<<endl;
        break;
    case 3:
        cout<<left<<setw(20)<<"Intel 80386"<<endl;
        break;
    case 4:
        cout<<left<<setw(20)<<"Motorola 68000"<<endl;
        break;
    case 5:
        cout<<left<<setw(20)<<"Motorola 88000"<<endl;
        break;
    case 6:
        cout<<left<<setw(20)<<"Reserved for future use (was EM_486)"<<endl;
        break;
    case 7:
        cout<<left<<setw(20)<<"Intel 80860"<<endl;
        break;
    case 8:
        cout<<left<<setw(20)<<"MIPS I Architecture"<<endl;
        break;
    case 9:
        cout<<left<<setw(20)<<"IBM System/370 Processor"<<endl;
        break;
    case 10:
        cout<<left<<setw(20)<<"MIPS RS3000 Little-endian"<<endl;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        cout<<left<<setw(20)<<"Reserved for future use"<<endl;
        break;
    case 15:
        cout<<left<<setw(20)<<"Hewlett-Packard PA-RISC"<<endl;
        break;
    case 16:
        cout<<left<<setw(20)<<"Reserved for future use"<<endl;
        break;
    case 17:
        cout<<left<<setw(20)<<"Fujitsu VPP500"<<endl;
        break;
    case 18:
        cout<<left<<setw(20)<<"Enhanced instruction set SPARC"<<endl;
        break;
    case 19:
        cout<<left<<setw(20)<<"Intel 80960"<<endl;
        break;
    case 20:
        cout<<left<<setw(20)<<"PowerPC"<<endl;
        break;
    case 21:
        cout<<left<<setw(20)<<"64-bit PowerPC"<<endl;
        break;
    case 22:
        cout<<left<<setw(20)<<"IBM System/390 Processor"<<endl;
        break;
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
        cout<<left<<setw(20)<<"Reserved for future use"<<endl;
        break;
    case 36:
        cout<<left<<setw(20)<<"NEC V800"<<endl;
        break;
    case 37:
        cout<<left<<setw(20)<<"Fujitsu FR20"<<endl;
        break;
    case 38:
        cout<<left<<setw(20)<<"TRW RH-32"<<endl;
        break;
    case 39:
        cout<<left<<setw(20)<<"Motorola RCE"<<endl;
        break;
    case 40:
        cout<<left<<setw(20)<<"Advanced RISC Machines ARM"<<endl;
        break;
    case 41:
        cout<<left<<setw(20)<<"Digital Alpha"<<endl;
        break;
    case 42:
        cout<<left<<setw(20)<<"Hitachi SH"<<endl;
        break;
    case 43:
        cout<<left<<setw(20)<<"SPARC Version 9"<<endl;
        break;
    case 44:
        cout<<left<<setw(20)<<"Siemens TriCore embedded processor"<<endl;
        break;
    case 45:
        cout<<left<<setw(20)<<"Argonaut RISC Core, Argonaut Technologies Inc."<<endl;
        break;
    case 46:
        cout<<left<<setw(20)<<"Hitachi H8/300"<<endl;
        break;
    case 47:
        cout<<left<<setw(20)<<"Hitachi H8/300H"<<endl;
        break;
    case 48:
        cout<<left<<setw(20)<<"Hitachi H8S"<<endl;
        break;
    case 49:
        cout<<left<<setw(20)<<"Hitachi H8/500"<<endl;
        break;
    case 50:
        cout<<left<<setw(20)<<"Intel IA-64 processor architecture"<<endl;
        break;
    case 51:
        cout<<left<<setw(20)<<"Stanford MIPS-X"<<endl;
        break;
    case 52:
        cout<<left<<setw(20)<<"Motorola ColdFire"<<endl;
        break;
    case 53:
        cout<<left<<setw(20)<<"Motorola M68HC12"<<endl;
        break;
    case 54:
        cout<<left<<setw(20)<<"Fujitsu MMA Multimedia Accelerator"<<endl;
        break;
    case 55:
        cout<<left<<setw(20)<<"Siemens PCP"<<endl;
        break;
    case 56:
        cout<<left<<setw(20)<<"Sony nCPU embedded RISC processor"<<endl;
        break;
    case 57:
        cout<<left<<setw(20)<<"Denso NDR1 microprocessor"<<endl;
        break;
    case 58:
        cout<<left<<setw(20)<<"Motorola Star*Core processor"<<endl;
        break;
    case 59:
        cout<<left<<setw(20)<<"Toyota ME16 processor"<<endl;
        break;
    case 60:
        cout<<left<<setw(20)<<"STMicroelectronics ST100 processor"<<endl;
        break;
    case 61:
        cout<<left<<setw(20)<<"Advanced Logic Corp. TinyJ embedded processor family"<<endl;
        break;
    case 62:
        cout<<left<<setw(20)<<"AMD x86-64 architecture"<<endl;
        break;
    case 63:
        cout<<left<<setw(20)<<"Sony DSP Processor"<<endl;
        break;
    case 64:
        cout<<left<<setw(20)<<"Digital Equipment Corp. PDP-10"<<endl;
        break;
    case 65:
        cout<<left<<setw(20)<<"Digital Equipment Corp. PDP-11"<<endl;
        break;
    case 66:
        cout<<left<<setw(20)<<"Siemens FX66 microcontroller"<<endl;
        break;
    case 67:
        cout<<left<<setw(20)<<"STMicroelectronics ST9+ 8/16 bit microcontroller"<<endl;
        break;
    case 68:
        cout<<left<<setw(20)<<"STMicroelectronics ST7 8-bit microcontroller"<<endl;
        break;
    case 69:
        cout<<left<<setw(20)<<"Motorola MC68HC16 Microcontroller"<<endl;
        break;
    case 70:
        cout<<left<<setw(20)<<"Motorola MC68HC11 Microcontroller"<<endl;
        break;
    case 71:
        cout<<left<<setw(20)<<"Motorola MC68HC08 Microcontroller"<<endl;
        break;
    case 72:
        cout<<left<<setw(20)<<"Motorola MC68HC05 Microcontroller"<<endl;
        break;
    case 73:
        cout<<left<<setw(20)<<"Silicon Graphics SVx"<<endl;
        break;
    case 74:
        cout<<left<<setw(20)<<"STMicroelectronics ST19 8-bit microcontroller"<<endl;
        break;
    case 75:
        cout<<left<<setw(20)<<"Digital VAX"<<endl;
        break;
    case 76:
        cout<<left<<setw(20)<<"Axis Communications 32-bit embedded processor"<<endl;
        break;
    case 77:
        cout<<left<<setw(20)<<"Infineon Technologies 32-bit embedded processor"<<endl;
        break;
    case 78:
        cout<<left<<setw(20)<<"Element 14 64-bit DSP Processor"<<endl;
        break;
    case 79:
        cout<<left<<setw(20)<<"LSI Logic 16-bit DSP Processor"<<endl;
        break;
    case 80:
        cout<<left<<setw(20)<<"Donald Knuth's educational 64-bit processor"<<endl;
        break;
    case 81:
        cout<<left<<setw(20)<<"Harvard University machine-independent object files"<<endl;
        break;
    case 82:
        cout<<left<<setw(20)<<"SiTera Prism"<<endl;
        break;
    case 83:
        cout<<left<<setw(20)<<"Atmel AVR 8-bit microcontroller"<<endl;
        break;
    case 84:
        cout<<left<<setw(20)<<"Fujitsu FR30"<<endl;
        break;
    case 85:
        cout<<left<<setw(20)<<"Mitsubishi D10V"<<endl;
        break;
    case 86:
        cout<<left<<setw(20)<<"Mitsubishi D30V"<<endl;
        break;
    case 87:
        cout<<left<<setw(20)<<"NEC v850"<<endl;
        break;
    case 88:
        cout<<left<<setw(20)<<"Mitsubishi M32R"<<endl;
        break;
    case 89:
        cout<<left<<setw(20)<<"Matsushita MN10300"<<endl;
        break;
    case 90:
        cout<<left<<setw(20)<<"Matsushita MN10200"<<endl;
        break;
    case 91:
        cout<<left<<setw(20)<<"picoJava"<<endl;
        break;
    case 92:
        cout<<left<<setw(20)<<"OpenRISC 32-bit embedded processor"<<endl;
        break;
    case 93:
        cout<<left<<setw(20)<<"ARC Cores Tangent-A5"<<endl;
        break;
    case 94:
        cout<<left<<setw(20)<<"Tensilica Xtensa Architecture"<<endl;
        break;
    case 95:
        cout<<left<<setw(20)<<"Alphamosaic VideoCore processor"<<endl;
        break;
    case 96:
        cout<<left<<setw(20)<<"Thompson Multimedia General Purpose Processor"<<endl;
        break;
    case 97:
        cout<<left<<setw(20)<<"National Semiconductor 32000 series"<<endl;
        break;
    case 98:
        cout<<left<<setw(20)<<"Tenor Network TPC processor"<<endl;
        break;
    case 99:
        cout<<left<<setw(20)<<"Trebia SNP 1000 processor"<<endl;
        break;
    case 100:
        cout<<left<<setw(20)<<"STMicroelectronics (www.st.com) ST200 microcontroller"<<endl;
        break;
    default:
        cout<<left<<setw(20)<<"Unknown"<<endl;
        break;
    }
    cout<<left<<setw(20)<<"Version:";
    switch (elf_header->e_version)
    {
    case 1:
        cout<<left<<setw(20)<<"1"<<endl;
        break;
    default:
        cout<<left<<setw(20)<<"Unknown"<<endl;
        break;
    }
    cout<<left<<setw(20)<<"Entry:";
    cout<<left<<setw(20)<<elf_header->e_entry<<endl;
    cout<<left<<setw(20)<<"PH offset:";
    cout<<left<<setw(20)<<elf_header->e_phoff<<endl;
    cout<<left<<setw(20)<<"SH offset:";
    cout<<left<<setw(20)<<elf_header->e_shoff<<endl;
    cout<<left<<setw(20)<<"Flags:";
    cout<<left<<setw(20)<<elf_header->e_flags<<endl;
    cout<<left<<setw(20)<<"EH size:";
    cout<<left<<setw(20)<<elf_header->e_ehsize<<endl;
    cout<<left<<setw(20)<<"PH size:";
    cout<<left<<setw(20)<<elf_header->e_phentsize<<endl;
    cout<<left<<setw(20)<<"PH num:";
    cout<<left<<setw(20)<<elf_header->e_phnum<<endl;
    cout<<left<<setw(20)<<"SH size:";
    cout<<left<<setw(20)<<elf_header->e_shentsize<<endl;
    cout<<left<<setw(20)<<"SH num:";
    cout<<left<<setw(20)<<elf_header->e_shnum<<endl;
    cout<<left<<setw(20)<<"SH ST index:";
    cout<<left<<setw(20)<<elf_header->e_shstrndx<<endl;
}

void show_program_header(Program_Header* program_header){
    switch (program_header->p_type)
    {
    case 1:
        cout<<left<<setw(8)<<"LOAD";
        break;
    case 2:
        cout<<left<<setw(8)<<"DYNAMIC";
        break;
    case 3:
        cout<<left<<setw(8)<<"INTERP";
        break;
    case 4:
        cout<<left<<setw(8)<<"NOTE";
        break;
    case 5:
        cout<<left<<setw(8)<<"SHLIB";
        break;
    case 6:
        cout<<left<<setw(8)<<"PHDR";
        break;
    case 0x70000000:
        cout<<left<<setw(8)<<"LOPROC";
        break;
    case 0x7fffffff:
        cout<<left<<setw(8)<<"HIPROC";
        break;
    default:
        cout<<left<<setw(8)<<"Unknown";
        break;
    }
    switch (program_header->p_flags)
    {
    case 1:
        cout<<left<<setw(8)<<"  X";
        break;
    case 2:
        cout<<left<<setw(8)<<" W ";
        break;
    case 3:
        cout<<left<<setw(8)<<" WX";
        break;
    case 4:
        cout<<left<<setw(8)<<"R  ";
        break;
    case 5:
        cout<<left<<setw(8)<<"R X";
        break;
    case 6:
        cout<<left<<setw(8)<<"RW ";
        break;
    case 7:
        cout<<left<<setw(8)<<"RWX";
        break;
    case 0x0ff00000:
        cout<<left<<setw(8)<<"MASKOS";
        break;
    case 0xf0000000:
        cout<<left<<setw(8)<<"MASKPROC";
        break;
    default:
        cout<<left<<setw(8)<<"Unknown";
    }
    cout<<left<<setw(8)<<program_header->p_offset;
    cout<<left<<setw(8)<<program_header->p_vaddr;
    cout<<left<<setw(8)<<program_header->p_paddr;
    cout<<left<<setw(8)<<program_header->p_filesz;
    cout<<left<<setw(8)<<program_header->p_memsz;
    cout<<left<<setw(8)<<program_header->p_align;
    cout<<endl;
}

void show_section_header(char *string_table, Section_Header* section_header){
    stringstream ss(string_table+section_header->sh_name);
    string name;
    ss>>name;
    cout<<left<<setw(20)<<name;
    switch(section_header->sh_type){
    case 0:
        cout<<left<<setw(15)<<"NULL";
        break;
    case 1:
        cout<<left<<setw(15)<<"PROGBITS";
        break;
    case 2:
        cout<<left<<setw(15)<<"SYMTAB";
        break;
    case 3:
        cout<<left<<setw(15)<<"STRTAB";
        break;
    case 4:
        cout<<left<<setw(15)<<"RELA";
        break;
    case 5:
        cout<<left<<setw(15)<<"HASH";
        break;
    case 6:
        cout<<left<<setw(15)<<"DYNAMIC";
        break;
    case 7:
        cout<<left<<setw(15)<<"NOTE";
        break;
    case 8:
        cout<<left<<setw(15)<<"NOBITS";
        break;
    case 9:
        cout<<left<<setw(15)<<"REL";
        break;
    case 10:
        cout<<left<<setw(15)<<"SHLIB";
        break;
    case 11:
        cout<<left<<setw(15)<<"DYNSYM";
        break;
    case 14:
        cout<<left<<setw(15)<<"INIT_ARRAY";
        break;
    case 15:
        cout<<left<<setw(15)<<"FINI_ARRAY";
        break;
    case 16:
        cout<<left<<setw(15)<<"PREINIT_ARRAY";
        break;
    case 17:
        cout<<left<<setw(15)<<"GROUP";
        break;
    case 18:
        cout<<left<<setw(15)<<"SYMTAB_SHNDX";
        break;
    case 19:
        cout<<left<<setw(15)<<"NUM";
        break;
    case 0x60000000:
        cout<<left<<setw(15)<<"LOOS";
        break;
    case 0x6ffffff5:
        cout<<left<<setw(15)<<"GNU_ATTRIBUTES";
        break;
    case 0x6ffffff6:
        cout<<left<<setw(15)<<"GNU_HASH";
        break;
    case 0x6ffffff7:
        cout<<left<<setw(15)<<"GNU_LIBLIST";
        break;
    case 0x6ffffff8:
        cout<<left<<setw(15)<<"CHECKSUM";
        break;
    case 0x6ffffffa:
        cout<<left<<setw(15)<<"LOSUNW";
        break;
    case 0x6ffffffb:
        cout<<left<<setw(15)<<"SUNW_COMDAT";
        break;
    case 0x6ffffffc:
        cout<<left<<setw(15)<<"SUNW_syminfo";
        break;
    case 0x6ffffffd:
        cout<<left<<setw(15)<<"GNU_verdef";
        break;
    case 0x6ffffffe:
        cout<<left<<setw(15)<<"GNU_verneed";
        break;
    case 0x6fffffff:
        cout<<left<<setw(15)<<"GNU_versym";
        break;
    case 0x70000000:
        cout<<left<<setw(15)<<"LOPROC";
        break;
    case 0x7fffffff:
        cout<<left<<setw(15)<<"HIPROC";
        break;
    case 0x80000000:
        cout<<left<<setw(15)<<"LOUSER";
        break;
    case 0x8fffffff:
        cout<<left<<setw(15)<<"HIUSER";
        break;
    }
    switch(section_header->sh_flags){
    case 0:
        cout<<left<<setw(5)<<"";
        break;
    case 1:
        cout<<left<<setw(5)<<"W";
        break;
    case 2:
        cout<<left<<setw(5)<<"A";
        break;
    case 3:
        cout<<left<<setw(5)<<"WA";
        break;
    case 4:
        cout<<left<<setw(5)<<"E";
        break;
    case 5:
        cout<<left<<setw(5)<<"WE";
        break;
    case 6:
        cout<<left<<setw(5)<<"WE";
        break;
    case 7:
        cout<<left<<setw(5)<<"WAE";
        break;
    case 16:
        cout<<left<<setw(5)<<"M";
        break;
    case 32:
        cout<<left<<setw(5)<<"S";
        break;
    case 48:
        cout<<left<<setw(5)<<"MS";
        break;
    case 0x0ff00000:
        cout<<left<<setw(5)<<"MASKOS";
        break;
    case 0xf0000000:
        cout<<left<<setw(5)<<"MASKPROC";
        break;
    default:
        break;
    }
    cout<<left<<setw(8)<<section_header->sh_addr;
    cout<<left<<setw(8)<<section_header->sh_offset;
    cout<<left<<setw(8)<<section_header->sh_size;
    cout<<left<<setw(5)<<section_header->sh_link;
    cout<<left<<setw(5)<<section_header->sh_info;
    cout<<left<<setw(6)<<section_header->sh_addralign;
    cout<<left<<setw(8)<<section_header->sh_entsize;
    cout<<endl;
}

void show_programe_header_table(ELF_Header *elf_header, Program_Header *program_header){
    cout<<"There are "<<elf_header->e_phnum<<" program headers"<<endl;
    cout<<"Entry point:"<<elf_header->e_phoff<<endl;
    cout<<left<<setw(8)<<"Type";
    cout<<left<<setw(8)<<"Flag";
    cout<<left<<setw(8)<<"Offset";
    cout<<left<<setw(8)<<"V_Addr";
    cout<<left<<setw(8)<<"P_Addr";
    cout<<left<<setw(8)<<"Filesz";
    cout<<left<<setw(8)<<"Memsz";
    cout<<left<<setw(8)<<"Align";
    cout<<endl;

    for(int i=0;i<elf_header->e_phnum;i++){
        show_program_header((program_header+i));    
    }
}

void show_section_header_table(ELF_Header *elf_header, char *string_table, Section_Header *section_header){
    cout<<"There are "<<elf_header->e_shnum<<" section headers"<<endl;
    cout<<left<<setw(20)<<"Name";
    cout<<left<<setw(15)<<"Type";
    cout<<left<<setw(5)<<"Flag";
    cout<<left<<setw(8)<<"Addr";
    cout<<left<<setw(8)<<"Offset";
    cout<<left<<setw(8)<<"Size";
    cout<<left<<setw(5)<<"Link";
    cout<<left<<setw(5)<<"Info";
    cout<<left<<setw(6)<<"Align";
    cout<<left<<setw(8)<<"EntSize";
    cout<<endl;

    for(int i=0;i<elf_header->e_shnum;i++){
        show_section_header(string_table, section_header+i);    
    }
}

void show_text_section(Text_section *text){
    for(int i=0;i<text->size;i++){
        printf("%08X\n", text->text_section[i]);
    }
}

ELF_Header *get_elf_header(string file_name){
    fstream file;
    file.open(file_name, ios::in);
    ELF_Header *elf_header = new ELF_Header;
    char buf[1024];
    file.read(buf, sizeof(ELF_Header));
    memcpy(elf_header, buf, sizeof(ELF_Header));
    file.close();
    return elf_header;
}

Program_Header *get_program_header(string file_name, ELF_Header *elf_header){
    fstream file;
    file.open(file_name, ios::in);
    Program_Header *program_header = new Program_Header[elf_header->e_phnum];
    char buf[1024];
    file.seekg(elf_header->e_phoff, file.beg);
    for(int i=0;i<elf_header->e_phnum;i++){
        file.read(buf, sizeof(Program_Header));
        memcpy((program_header+i), buf, sizeof(Program_Header));
    }
    file.close();
    return program_header;
}

Section_Header *get_section_header(string file_name, ELF_Header *elf_header){
    fstream file;
    file.open(file_name, ios::in);
    Section_Header *section_header = new Section_Header[elf_header->e_shnum];
    char buf[1024];
    file.seekg(elf_header->e_shoff, file.beg);
    for(int i=0;i<elf_header->e_shnum;i++){
        file.read(buf, sizeof(Section_Header));
        memcpy((section_header+i), buf, sizeof(Section_Header));
    }
    file.close();
    return section_header;
}

char *get_string_table(string file_name, ELF_Header *elf_header){
    fstream file;
    file.open(file_name, ios::in);
    Section_Header *section_header = new Section_Header;
    char buf[1024];
    
    file.seekg(elf_header->e_shoff+elf_header->e_shstrndx*elf_header->e_shentsize, file.beg);
    file.read(buf, sizeof(Section_Header));
    memcpy(section_header, buf, sizeof(Section_Header));
    file.seekg(section_header->sh_offset, file.beg);

    char *string_table = new char[section_header->sh_size];
    file.read(string_table, section_header->sh_size);
    return string_table;
}

Text_section *get_text_section(string file_name){
    fstream file;
    file.open(file_name, ios::in);
    ELF_Header *elf_header = get_elf_header(file_name);
    char *string_table = get_string_table(file_name, elf_header);
    Section_Header *section_header = get_section_header(file_name, elf_header);
    Text_section *text = new Text_section;
    for(int i=0;i<elf_header->e_shnum;i++){
        stringstream ss(string_table+(section_header+i)->sh_name);
        string name;
        ss>>name;
        if(name == ".text"){
            file.seekg((section_header+i)->sh_offset, file.beg);
            text->text_section = new uint32_t[(section_header+i)->sh_size/sizeof(uint32_t)];
            text->size = (section_header+i)->sh_size/sizeof(uint32_t);
            file.read((char*)text->text_section, (section_header+i)->sh_size);
            break;
        }
    }
    file.close();
    return text;
}

int main(int argc, char** argv){

    ELF_Header *elf_header;
    Program_Header *program_header;
    Section_Header *section_header;

    elf_header = get_elf_header(argv[1]);
    if(elf_header->e_ident[1]!='E' && elf_header->e_ident[2]!='L' && elf_header->e_ident[3]!='F')
        return 0;
    show_elf_header(elf_header);
    
    program_header = get_program_header(argv[1], elf_header);
    show_programe_header_table(elf_header, program_header);

    char *string_table = get_string_table(argv[1], elf_header);
    section_header = get_section_header(argv[1], elf_header);
    show_section_header_table(elf_header, string_table, section_header);

    Text_section *text;

    text = get_text_section(argv[1]);
    show_text_section(text);
    
}