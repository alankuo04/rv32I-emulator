#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<sstream>
#include<iomanip>
using namespace std;

struct ELF_Header
{
    unsigned char   e_ident[16];
    uint16_t        e_type;
    uint16_t        e_machine;
    uint32_t        e_version;
    uint32_t        e_entry;
    uint32_t        e_phoff;
    uint32_t        e_shoff;
    uint32_t        e_flags;
    uint16_t        e_ehsize;
    uint16_t        e_phentsize;
    uint16_t        e_phnum;
    uint16_t        e_shentsize;
    uint16_t        e_shnum;
    uint16_t        e_shstrndx;
};

struct Program_Header{
    uint32_t    p_type;
    uint32_t    p_offset;
    uint32_t    p_vaddr;
    uint32_t    p_paddr;
    uint32_t    p_filesz;
    uint32_t    p_memsz;
    uint32_t    p_flags;
    uint32_t    p_align;  
};

struct Section_Header{
    uint32_t    sh_name;
    uint32_t    sh_type;
    uint32_t    sh_flags;
    uint32_t    sh_addr;
    uint32_t    sh_offset;
    uint32_t    sh_size;
    uint32_t    sh_link;
    uint32_t    sh_info;
    uint32_t    sh_addralign;
    uint32_t    sh_entsize;
};

void show_elf_header(ELF_Header* elf_header){
    cout<<"ELF:\t\t";
    switch (elf_header->e_ident[4])
    {
    case 1:
        cout<<"ELF32"<<endl;
        break;
    case 2:
        cout<<"ELF64"<<endl;
        break;
    default:
        cout<<"Unknown"<<endl;
        break;
    }
    cout<<"Data:\t\t";
    switch (elf_header->e_ident[5])
    {
    case 1:
        cout<<"little endian"<<endl;
        break;
    case 2:
        cout<<"big endian"<<endl;
        break;
    default:
        cout<<"Unknown"<<endl;
        break;
    }
    cout<<"Version:\t";
    switch (elf_header->e_ident[6])
    {
    case 1:
        cout<<"1(current)"<<endl;
        break;
    default:
        cout<<"Unknown"<<endl;
        break;
    }
    cout<<"OS/ABI:\t\t";
    switch(elf_header->e_ident[7]){
    case 0:
        cout<<"No extensions or unspecified"<<endl;
        break;
    case 1:
        cout<<"Hewlett-Packard HP-UX"<<endl;
        break;
    case 2:
        cout<<"NetBSD"<<endl;
        break;
    case 3:
        cout<<"Linux"<<endl;
        break;
    case 6:
        cout<<"Sun Solaris"<<endl;
        break;
    case 7:
        cout<<"AIX"<<endl;
        break;
    case 8:
        cout<<"IRIX"<<endl;
        break;
    case 9:
        cout<<"FreeBSD"<<endl;
        break;
    case 10:
        cout<<"Compaq TRU64 UNIX"<<endl;
        break;
    case 11:
        cout<<"Novell Modesto"<<endl;
        break;
    case 12:
        cout<<"Open BSD"<<endl;
        break;
    case 13:
        cout<<"Open VMS"<<endl;
        break;
    case 14:
        cout<<"Hewlett-Packard Non-Stop Kernel"<<endl;
        break;
    }
    cout<<"ABI Version:\t";
    cout<<int(elf_header->e_ident[8])<<endl;
    
    cout<<"Type:\t\t";
    switch (elf_header->e_type)
    {
    case 1:
        cout<<"REL"<<endl;
        break;
    case 2:
        cout<<"EXEC"<<endl;
        break;
    case 3:
        cout<<"DYN"<<endl;
        break;
    case 4:
        cout<<"CORE"<<endl;
        break;
    case 0xff00:
        cout<<"LOPROC"<<endl;
        break;
    case 0xffff:
        cout<<"HIPROC"<<endl;
        break;
    default:
        cout<<"Unknown"<<endl;
        break;
    }
    cout<<"Machine:\t";
    switch (elf_header->e_machine)
    {
    case 1:
        cout<<"AT&T WE 32100"<<endl;
        break;
    case 2:
        cout<<"SPARC"<<endl;
        break;
    case 3:
        cout<<"Intel 80386"<<endl;
        break;
    case 4:
        cout<<"Motorola 68000"<<endl;
        break;
    case 5:
        cout<<"Motorola 88000"<<endl;
        break;
    case 6:
        cout<<"Reserved for future use (was EM_486)"<<endl;
        break;
    case 7:
        cout<<"Intel 80860"<<endl;
        break;
    case 8:
        cout<<"MIPS I Architecture"<<endl;
        break;
    case 9:
        cout<<"IBM System/370 Processor"<<endl;
        break;
    case 10:
        cout<<"MIPS RS3000 Little-endian"<<endl;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        cout<<"Reserved for future use"<<endl;
        break;
    case 15:
        cout<<"Hewlett-Packard PA-RISC"<<endl;
        break;
    case 16:
        cout<<"Reserved for future use"<<endl;
        break;
    case 17:
        cout<<"Fujitsu VPP500"<<endl;
        break;
    case 18:
        cout<<"Enhanced instruction set SPARC"<<endl;
        break;
    case 19:
        cout<<"Intel 80960"<<endl;
        break;
    case 20:
        cout<<"PowerPC"<<endl;
        break;
    case 21:
        cout<<"64-bit PowerPC"<<endl;
        break;
    case 22:
        cout<<"IBM System/390 Processor"<<endl;
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
        cout<<"Reserved for future use"<<endl;
        break;
    case 36:
        cout<<"NEC V800"<<endl;
        break;
    case 37:
        cout<<"Fujitsu FR20"<<endl;
        break;
    case 38:
        cout<<"TRW RH-32"<<endl;
        break;
    case 39:
        cout<<"Motorola RCE"<<endl;
        break;
    case 40:
        cout<<"Advanced RISC Machines ARM"<<endl;
        break;
    case 41:
        cout<<"Digital Alpha"<<endl;
        break;
    case 42:
        cout<<"Hitachi SH"<<endl;
        break;
    case 43:
        cout<<"SPARC Version 9"<<endl;
        break;
    case 44:
        cout<<"Siemens TriCore embedded processor"<<endl;
        break;
    case 45:
        cout<<"Argonaut RISC Core, Argonaut Technologies Inc."<<endl;
        break;
    case 46:
        cout<<"Hitachi H8/300"<<endl;
        break;
    case 47:
        cout<<"Hitachi H8/300H"<<endl;
        break;
    case 48:
        cout<<"Hitachi H8S"<<endl;
        break;
    case 49:
        cout<<"Hitachi H8/500"<<endl;
        break;
    case 50:
        cout<<"Intel IA-64 processor architecture"<<endl;
        break;
    case 51:
        cout<<"Stanford MIPS-X"<<endl;
        break;
    case 52:
        cout<<"Motorola ColdFire"<<endl;
        break;
    case 53:
        cout<<"Motorola M68HC12"<<endl;
        break;
    case 54:
        cout<<"Fujitsu MMA Multimedia Accelerator"<<endl;
        break;
    case 55:
        cout<<"Siemens PCP"<<endl;
        break;
    case 56:
        cout<<"Sony nCPU embedded RISC processor"<<endl;
        break;
    case 57:
        cout<<"Denso NDR1 microprocessor"<<endl;
        break;
    case 58:
        cout<<"Motorola Star*Core processor"<<endl;
        break;
    case 59:
        cout<<"Toyota ME16 processor"<<endl;
        break;
    case 60:
        cout<<"STMicroelectronics ST100 processor"<<endl;
        break;
    case 61:
        cout<<"Advanced Logic Corp. TinyJ embedded processor family"<<endl;
        break;
    case 62:
        cout<<"AMD x86-64 architecture"<<endl;
        break;
    case 63:
        cout<<"Sony DSP Processor"<<endl;
        break;
    case 64:
        cout<<"Digital Equipment Corp. PDP-10"<<endl;
        break;
    case 65:
        cout<<"Digital Equipment Corp. PDP-11"<<endl;
        break;
    case 66:
        cout<<"Siemens FX66 microcontroller"<<endl;
        break;
    case 67:
        cout<<"STMicroelectronics ST9+ 8/16 bit microcontroller"<<endl;
        break;
    case 68:
        cout<<"STMicroelectronics ST7 8-bit microcontroller"<<endl;
        break;
    case 69:
        cout<<"Motorola MC68HC16 Microcontroller"<<endl;
        break;
    case 70:
        cout<<"Motorola MC68HC11 Microcontroller"<<endl;
        break;
    case 71:
        cout<<"Motorola MC68HC08 Microcontroller"<<endl;
        break;
    case 72:
        cout<<"Motorola MC68HC05 Microcontroller"<<endl;
        break;
    case 73:
        cout<<"Silicon Graphics SVx"<<endl;
        break;
    case 74:
        cout<<"STMicroelectronics ST19 8-bit microcontroller"<<endl;
        break;
    case 75:
        cout<<"Digital VAX"<<endl;
        break;
    case 76:
        cout<<"Axis Communications 32-bit embedded processor"<<endl;
        break;
    case 77:
        cout<<"Infineon Technologies 32-bit embedded processor"<<endl;
        break;
    case 78:
        cout<<"Element 14 64-bit DSP Processor"<<endl;
        break;
    case 79:
        cout<<"LSI Logic 16-bit DSP Processor"<<endl;
        break;
    case 80:
        cout<<"Donald Knuth's educational 64-bit processor"<<endl;
        break;
    case 81:
        cout<<"Harvard University machine-independent object files"<<endl;
        break;
    case 82:
        cout<<"SiTera Prism"<<endl;
        break;
    case 83:
        cout<<"Atmel AVR 8-bit microcontroller"<<endl;
        break;
    case 84:
        cout<<"Fujitsu FR30"<<endl;
        break;
    case 85:
        cout<<"Mitsubishi D10V"<<endl;
        break;
    case 86:
        cout<<"Mitsubishi D30V"<<endl;
        break;
    case 87:
        cout<<"NEC v850"<<endl;
        break;
    case 88:
        cout<<"Mitsubishi M32R"<<endl;
        break;
    case 89:
        cout<<"Matsushita MN10300"<<endl;
        break;
    case 90:
        cout<<"Matsushita MN10200"<<endl;
        break;
    case 91:
        cout<<"picoJava"<<endl;
        break;
    case 92:
        cout<<"OpenRISC 32-bit embedded processor"<<endl;
        break;
    case 93:
        cout<<"ARC Cores Tangent-A5"<<endl;
        break;
    case 94:
        cout<<"Tensilica Xtensa Architecture"<<endl;
        break;
    case 95:
        cout<<"Alphamosaic VideoCore processor"<<endl;
        break;
    case 96:
        cout<<"Thompson Multimedia General Purpose Processor"<<endl;
        break;
    case 97:
        cout<<"National Semiconductor 32000 series"<<endl;
        break;
    case 98:
        cout<<"Tenor Network TPC processor"<<endl;
        break;
    case 99:
        cout<<"Trebia SNP 1000 processor"<<endl;
        break;
    case 100:
        cout<<"STMicroelectronics (www.st.com) ST200 microcontroller"<<endl;
        break;
    default:
        cout<<"Unknown"<<endl;
        break;
    }
    cout<<"Version:\t";
    switch (elf_header->e_version)
    {
    case 1:
        cout<<"1"<<endl;
        break;
    default:
        cout<<"Unknown"<<endl;
        break;
    }
    cout<<"Entry:\t\t";
    cout<<elf_header->e_entry<<endl;
    cout<<"PH offset:\t";
    cout<<elf_header->e_phoff<<endl;
    cout<<"SH offset:\t";
    cout<<elf_header->e_shoff<<endl;
    cout<<"Flags:\t\t";
    cout<<elf_header->e_flags<<endl;
    cout<<"EH size:\t";
    cout<<elf_header->e_ehsize<<endl;
    cout<<"PH size:\t";
    cout<<elf_header->e_phentsize<<endl;
    cout<<"PH num:\t\t";
    cout<<elf_header->e_phnum<<endl;
    cout<<"SH size:\t";
    cout<<elf_header->e_shentsize<<endl;
    cout<<"SH num:\t\t";
    cout<<elf_header->e_shnum<<endl;
    cout<<"SH ST index:\t";
    cout<<elf_header->e_shstrndx<<endl;
}

void show_program_header(Program_Header* program_header){
    switch (program_header->p_type)
    {
    case 1:
        cout<<"LOAD"<<"\t";
        break;
    case 2:
        cout<<"DYNAMIC"<<"\t";
        break;
    case 3:
        cout<<"INTERP"<<"\t";
        break;
    case 4:
        cout<<"NOTE"<<"\t";
        break;
    case 5:
        cout<<"SHLIB"<<"\t";
        break;
    case 6:
        cout<<"PHDR"<<"\t";
        break;
    case 0x70000000:
        cout<<"LOPROC"<<"\t";
        break;
    case 0x7fffffff:
        cout<<"HIPROC"<<"\t";
        break;
    default:
        cout<<"Unknown"<<"\t";
        break;
    }
    switch (program_header->p_flags)
    {
    case 1:
        cout<<"  X"<<"\t";
        break;
    case 2:
        cout<<" W "<<"\t";
        break;
    case 3:
        cout<<" WX"<<"\t";
        break;
    case 4:
        cout<<"R  "<<"\t";
        break;
    case 5:
        cout<<"R X"<<"\t";
        break;
    case 6:
        cout<<"RW "<<"\t";
        break;
    case 7:
        cout<<"RWX"<<"\t";
        break;
    case 0x0ff00000:
        cout<<"MASKOS"<<"\t";
        break;
    case 0xf0000000:
        cout<<"MASKPROC"<<"\t";
        break;
    default:
        cout<<"Unknown"<<"\t";
    }
    cout<<program_header->p_offset<<"\t";
    cout<<program_header->p_vaddr<<"\t";
    cout<<program_header->p_paddr<<"\t";
    cout<<program_header->p_filesz<<"\t";
    cout<<program_header->p_memsz<<"\t";
    cout<<program_header->p_align<<"\t";
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

int main(int argc, char** argv){
    fstream file;
    file.open(argv[1], ios::in);
    ELF_Header *elf_header = new ELF_Header;
    Program_Header *program_header = new Program_Header;
    Section_Header *section_header = new Section_Header;
    char buf[1024];
    file.read(buf, sizeof(ELF_Header));
    memcpy(elf_header, buf, sizeof(ELF_Header));
    show_elf_header(elf_header);
    
    cout<<"There are "<<elf_header->e_phnum<<" program headers"<<endl;
    cout<<"Entry point:\t";
    cout<<elf_header->e_phoff<<endl;
    cout<<"Type\tFlag\tOffset\tV_addr\tP_addr\tFilesz\tMemsz\tAlign"<<endl;
    for(int i=0;i<elf_header->e_phnum;i++){
        file.read(buf, sizeof(Program_Header));
        memcpy(program_header, buf, sizeof(Program_Header));
        show_program_header(program_header);    
    }

    file.seekg(elf_header->e_shoff+elf_header->e_shstrndx*elf_header->e_shentsize, file.beg);
    file.read(buf, sizeof(Section_Header));
    memcpy(section_header, buf, sizeof(Section_Header));
    file.seekg(section_header->sh_offset, file.beg);
    
    char string_table[section_header->sh_size];
    file.read(string_table, section_header->sh_size);

    cout<<"There are "<<elf_header->e_shnum<<" section headers"<<endl;
    file.seekg(elf_header->e_shoff, file.beg);
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
        file.read(buf, sizeof(Section_Header));
        memcpy(section_header, buf, sizeof(Section_Header));
        show_section_header(string_table, section_header);    
    }
}