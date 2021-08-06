#include"elf_function.h"

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

    //text = get_text_section(argv[1]);
    //show_text_section(text);
    
}