#include<iostream>
#include<string>

std::string register_list[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", 
                        "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", 
                        "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", 
                        "t3", "t4", "t5", "t6"};

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

union Instruction{
    uint32_t instruction;
    struct
    {
        uint32_t opcode :   7;
        uint32_t other  :   25;
    }general; 
    struct
    {
        uint32_t opcode :   7;
        uint32_t rd     :   5;
        uint32_t funct3 :   3;
        uint32_t rs1    :   5;
        uint32_t rs2    :   5;
        uint32_t funct7 :   7;
    }R;
    struct 
    {
        uint32_t opcode :   7;
        uint32_t rd     :   5;
        uint32_t funct3 :   3;
        uint32_t rs1    :   5;
        uint32_t imm11_0:   12;
    }I;
    struct
    {
        uint32_t opcode :   7;
        uint32_t imm4_0 :   5;
        uint32_t funct3 :   3;
        uint32_t rs1    :   5;
        uint32_t rs2    :   5;
        uint32_t imm11_5:   7;
    }S;
    struct
    {
        uint32_t opcode :   7;
        uint32_t imm11  :   1;
        uint32_t imm4_1 :   4;
        uint32_t funct3 :   3;
        uint32_t rs1    :   5;
        uint32_t rs2    :   5;
        uint32_t imm10_5:   6;
        uint32_t imm12  :   1;
    }B;
    struct
    {
        uint32_t opcode     :   7;
        uint32_t rd         :   5;
        uint32_t imm31_12   :   20;
    }U;
    struct
    {
        uint32_t opcode     :   7;
        uint32_t rd         :   5;
        uint32_t imm19_12   :   8;
        uint32_t imm11      :   1;
        uint32_t imm10_1    :   10;
        uint32_t imm20      :   1;
    }J;
};