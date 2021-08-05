#include<iostream>
#include<string>
#include"elf_function.h"
using namespace std;

union instruction
{
    struct R{
        uint32_t funct7 :   7;
        uint32_t rs2    :   5;
        uint32_t rs1    :   5;
        uint32_t funct3 :   3;
        uint32_t rd     :   5;
        uint32_t opcode :   7;
    };
    struct I{
        uint32_t imm11_0:   12;
        uint32_t rs1    :   5;
        uint32_t funct3 :   3;
        uint32_t rd     :   5;
        uint32_t opcode :   7;
    };
    struct S{
        uint32_t imm11_5:   7;
        uint32_t rs2    :   5;
        uint32_t rs1    :   5;
        uint32_t funct3 :   3;
        uint32_t imm4_0 :   5;
        uint32_t opcode :   7;
    };
    struct B{
        uint32_t imm12  :   1;
        uint32_t imm10_5:   6;
        uint32_t rs2    :   5;
        uint32_t rs1    :   5;
        uint32_t funct3 :   3;
        uint32_t imm4_1 :   4;
        uint32_t imm11  :   1;
        uint32_t opcode :   7;
    };
};


int main(int argc, char **argv){
    Text_section *text = get_text_section(argv[1]);
    for(int i=0;i<text->size;i++){
        printf("%08X\n", text->text_section[i]);
    }
}