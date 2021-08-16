#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include"my_elf.h"
using namespace std;

#define MAX_MEMORY_SIZE 1048576

int main(int argc, char **argv){
    fstream file;
    file.open(argv[1], ios::in);
    ELF_Header *elf_header = new ELF_Header;
    Program_Header *program_header = new Program_Header;
    uint32_t *memory = new uint32_t[MAX_MEMORY_SIZE];
    char buf[1024];
    file.read(buf, sizeof(ELF_Header));
    memcpy(elf_header, buf, sizeof(ELF_Header));
    file.seekg(elf_header->e_phoff, file.beg);
    for(int i=0;i<elf_header->e_phnum;i++){
        file.seekg(elf_header->e_phoff+elf_header->e_phentsize*i, file.beg);
        file.read(buf, sizeof(Program_Header));
        memcpy(program_header, buf, sizeof(Program_Header));
        if(program_header->p_type == 1){
            file.seekg(program_header->p_offset, file.beg);
            //cout<<file.tellg()<<" "<<program_header->p_vaddr<<" "<<program_header->p_memsz<<endl;
            file.read((char*)memory+program_header->p_vaddr, program_header->p_memsz);
        }
    }
    int entry = elf_header->e_entry/4;
    /*for(int i=entry;i<entry+10;i++){
        printf("%08X\n", memory[i]);
        //cout<<int(memory[elf_header->e_entry+i])<<endl;
    }*/
    file.close();

    string next, opcode;
    int pc = elf_header->e_entry;
    int temp_register[32]={};
    temp_register[2]=MAX_MEMORY_SIZE-1;
    int signedbit;
    uint32_t imm;
    while(getline(cin, next)){
        printf("%08X\n", memory[pc/4]);
        Instruction instruction = {memory[pc/4]};
        switch (instruction.general.opcode)
        {
        case 51:    // 0110011 R type
            switch (instruction.R.funct7)
            {
            case 0:
                switch (instruction.R.funct3)
                {
                case 0:
                    opcode = "ADD";
                    break;
                case 1:
                    opcode = "SLL";
                    break;
                case 2:
                    opcode = "SLT";
                    break;
                case 3:
                    opcode = "SLTU";
                    break;
                case 4:
                    opcode = "XOR";
                    break;
                case 5:
                    opcode = "SRL";
                    break;
                case 6:
                    opcode = "OR";
                    break;
                case 7:
                    opcode = "AND";
                    break;            
                }
                break;
            case 32:
                switch (instruction.R.funct3)
                {
                case 0:
                    opcode = "SUB";
                    break;
                case 6:
                    opcode = "SRA";
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            printf("%s %s, %s, %s\n", opcode.c_str(), register_list[instruction.R.rd].c_str(), register_list[instruction.R.rs1].c_str(), register_list[instruction.R.rs2].c_str());
            break;
        case 3:     // 0000011 I type
            signedbit = (instruction.I.imm11_0>>11);
            imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
            switch (instruction.I.funct3)
            {
            case 0:
                opcode = "LB";
                break;
            case 1:
                opcode = "LH";
                break;
            case 2:
                opcode = "LW";
                break;
            case 4:
                opcode = "LBU";
                break;
            case 5:
                opcode = "LHU";
                break;
            default:
                break;
            }
            printf("%s %s, %s, %u\n", opcode.c_str(), register_list[instruction.I.rd].c_str(), register_list[instruction.I.rs1].c_str(), imm);
            break;
        case 19:    // 0010011 I type
            signedbit = (instruction.I.imm11_0>>11);
            imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
            switch (instruction.I.funct3)
            {
            case 0:
                opcode = "ADDI";
                break;
            case 2:
                opcode = "SLTI";
                break;
            case 3:
                opcode = "SLTIU";
                break;
            case 4:
                opcode = "XORI";
                break;
            case 6:
                opcode = "ORI";
                break;
            case 7:
                opcode = "ANDI";
                break;
            case 1:
                opcode = "SLLI";
                break;
            case 5:
                if((instruction.I.imm11_0>>5) == 0)
                    opcode = "SRLI";
                else{
                    opcode = "SRAI";
                    imm = imm-(2<<9);
                }
                break;
            default:
                break;
            }
            printf("%s %s, %s, %u\n", opcode.c_str(), register_list[instruction.I.rd].c_str(), register_list[instruction.I.rs1].c_str(), imm);
            break;
        case 103:   // 1100111 I type
            signedbit = (instruction.I.imm11_0>>11);
            imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
            opcode = "JALR";
            printf("%s %s, %s, %u\n", opcode.c_str(), register_list[instruction.I.rd].c_str(), register_list[instruction.I.rs1].c_str(), imm);
            break;
        case 115:   // 1110011 I type
            if(instruction.I.imm11_0 == 0)
                opcode = "ECALL";
            else
                opcode = "EBREAK";
            printf("%s\n", opcode.c_str());
            break;
        case 35:    // 0100011 S type
            signedbit = (instruction.S.imm11_5>>6);
            imm = signedbit?((instruction.S.imm11_5<<5)+instruction.S.imm4_0)-(2<<11):((instruction.S.imm11_5<<5)+instruction.S.imm4_0);
            switch (instruction.S.funct3)
            {
            case 0:
                opcode = "SB";
                break;
            case 1:
                opcode = "SH";
                break;
            case 2:
                opcode = "SW";
                break;
            default:
                break;
            }
            printf("%s %s, %s, %u\n", opcode.c_str(), register_list[instruction.S.rs2].c_str(), register_list[instruction.S.rs1].c_str(), imm);
            break;
        case 99:    // 1100011 B type
            signedbit = instruction.B.imm12;
            imm = signedbit?(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+(instruction.B.imm4_1<<1)-(2<<11):(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+(instruction.B.imm4_1<<1);
            switch (instruction.B.funct3)
            {
            case 0:
                opcode = "BEQ";
                break;
            case 1:
                opcode = "BNE";
                break;
            case 4:
                opcode = "BLT";
                break;
            case 5:
                opcode = "BGE";
                break;
            case 6:
                opcode = "BLTU";
                break;
            case 7:
                opcode = "BGEU";
                break;
            default:
                break;
            }
            printf("%s %s, %s, %u\n", opcode.c_str(), register_list[instruction.B.rs2].c_str(), register_list[instruction.B.rs1].c_str(), imm);
            break;
        case 55:    // 0110111 U type
            opcode = "LUI";
            printf("%s %s, %u\n", opcode.c_str(), register_list[instruction.U.rd].c_str(), instruction.U.imm31_12);
            break;
        case 23:    // 0010111 U type
            opcode = "AUIPC";
            printf("%s %s, %u\n", opcode.c_str(), register_list[instruction.U.rd].c_str(), instruction.U.imm31_12);
            break;
        case 111:   // 1101111 J type
            signedbit = instruction.J.imm20;
            imm = signedbit?(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1)-(2<<19):(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1);
            opcode = "JAL";
            printf("%s %s, %u\n", opcode.c_str(), register_list[instruction.J.rd].c_str(), imm);
            break;        
        default:
            break;
        }
        pc+=4;
    }
}