#include<iostream>
#include<string>

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

struct Text_section{
    uint32_t *text_section;
    int size;
};

void show_elf_header(ELF_Header* elf_header);

void show_program_header(Program_Header* program_header);

void show_section_header(char *string_table, Section_Header* section_header);

void show_programe_header_table(ELF_Header *elf_header, Program_Header *program_header);

void show_section_header_table(ELF_Header *elf_header, char* string_table, Section_Header *section_header);

void show_text_section(Text_section *text);

ELF_Header *get_elf_header(std::string file_name);

Program_Header *get_program_header(std::string file_name, ELF_Header *elf_header);

Section_Header *get_section_header(std::string file_name, ELF_Header *elf_header);

char *get_string_table(std::string file_name, ELF_Header *elf_header);

Text_section *get_text_section(std::string file_name);