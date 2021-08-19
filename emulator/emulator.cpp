#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include"my_elf.h"
using namespace std;

// Max memory size 2GB (uint32 * 536870908)
#define MAX_MEMORY_SIZE 536870908

void show_register(int *temp_register){
    for(int i=0;i<32;i++){
        printf("%4s: 0x%08X\n", register_list[i], temp_register[i]);
    }
}

int main(int argc, char **argv){
    fstream file;
    file.open(argv[1], ios::in);
    bool flag = (argc>2 && strcmp(argv[2],"--show-instruction")==0)?true:false;
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
        if(program_header->p_type == 1){ // PH_LOAD
            file.seekg(program_header->p_offset, file.beg);
            //cout<<file.tellg()<<" "<<program_header->p_vaddr<<" "<<program_header->p_memsz<<endl;
            file.read((char*)memory+program_header->p_vaddr, program_header->p_memsz);
        }
    }
    /*int entry = elf_header->e_entry/4;
    for(int i=0x13020;i>=0x13004;i-=4){
        printf("%08X: %08X\n", i, memory[i/4]);
        //cout<<int(memory[elf_header->e_entry+i])<<endl;
    }*/

    file.close();
    char const *opcode;
    int pc = elf_header->e_entry; // start of text section
    int temp_register[32]={};
    temp_register[2]=MAX_MEMORY_SIZE*4;
    int signedbit;
    int imm;
    bool end=false;

    //while(getline(cin, next)){
    while(!end){
        temp_register[0] = 0; // x0 register always be 0
        //show_register(temp_register);
        if(flag){
            printf("%05X: ", pc);
            printf("%08X ", memory[pc/4]);
        }
        //printf("%08X %08X %08X %08X ", temp_register[8], temp_register[13], temp_register[14], temp_register[15]);
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
                    temp_register[instruction.R.rd] = temp_register[instruction.R.rs1]+temp_register[instruction.R.rs2];
                    break;
                case 1:
                    opcode = "SLL";
                    temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]<<(temp_register[instruction.R.rs2]&31));
                    break;
                case 2:
                    opcode = "SLT";
                    temp_register[instruction.R.rd] = temp_register[instruction.R.rs1]<temp_register[instruction.R.rs2]?1:0;
                    break;
                case 3:
                    opcode = "SLTU";
                    temp_register[instruction.R.rd] = (uint32_t)(temp_register[instruction.R.rs1])<(uint32_t)(temp_register[instruction.R.rs2])?1:0;
                    break;
                case 4:
                    opcode = "XOR";
                    temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]^temp_register[instruction.R.rs2]);
                    break;
                case 5:
                    opcode = "SRL";
                    temp_register[instruction.R.rd] = ((uint32_t)(temp_register[instruction.R.rs1])>>(temp_register[instruction.R.rs2]&31));
                    break;
                case 6:
                    opcode = "OR";
                    temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]|temp_register[instruction.R.rs2]);
                    break;
                case 7:
                    opcode = "AND";
                    temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]&temp_register[instruction.R.rs2]);
                    break;
                }
                break;
            case 32:
                switch (instruction.R.funct3)
                {
                case 0:
                    opcode = "SUB";
                    temp_register[instruction.R.rd] = temp_register[instruction.R.rs1]-temp_register[instruction.R.rs2];
                    break;
                case 6:
                    opcode = "SRA";
                    temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]>>(temp_register[instruction.R.rs2]&31));
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            if(flag)
                printf("%s %s, %s, %s\n", opcode, register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]);
            pc += 4;
            break;
        case 3:     // 0000011 I type
            signedbit = (instruction.I.imm11_0>>11);
            imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
            switch (instruction.I.funct3)
            {
            case 0:
                opcode = "LB";
                int8_t byte;
                memcpy(&byte, (int8_t*)(memory)+(temp_register[instruction.I.rs1]+imm)*4/4, sizeof(int8_t));
                temp_register[instruction.I.rd] = byte;
                break;
            case 1:
                opcode = "LH";
                int16_t half;
                memcpy(&half, (int16_t*)(memory)+(temp_register[instruction.I.rs1]+imm)*2/4, sizeof(int16_t));
                temp_register[instruction.I.rd] = half;
                break;
            case 2:
                opcode = "LW";
                int32_t word;
                memcpy(&word, memory+(temp_register[instruction.I.rs1]+imm)/4, sizeof(int32_t));
                temp_register[instruction.I.rd] = word;
                break;
            case 4:
                opcode = "LBU";
                uint8_t u_byte;
                memcpy(&u_byte, (uint8_t*)(memory)+(temp_register[instruction.I.rs1]+imm)*4/4, sizeof(uint8_t));
                temp_register[instruction.I.rd] = u_byte;
                break;
            case 5:
                opcode = "LHU";
                uint16_t u_half;
                memcpy(&u_half, (uint16_t*)(memory)+(temp_register[instruction.I.rs1]+imm)*2/4, sizeof(uint16_t));
                temp_register[instruction.I.rd] = u_half;
                break;
            default:
                break;
            }
            if(flag)
                printf("%s %s, %d(%s)\n", opcode, register_list[instruction.I.rd], imm, register_list[instruction.I.rs1]);
            pc += 4;
            break;
        case 19:    // 0010011 I type
            signedbit = (instruction.I.imm11_0>>11);
            imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
            switch (instruction.I.funct3)
            {
            case 0:
                opcode = "ADDI";
                temp_register[instruction.I.rd] = temp_register[instruction.I.rs1]+imm;
                break;
            case 2:
                opcode = "SLTI";
                temp_register[instruction.I.rd] = temp_register[instruction.I.rs1]<imm?1:0;
                break;
            case 3:
                opcode = "SLTIU";
                temp_register[instruction.I.rd] = (uint32_t)(temp_register[instruction.I.rs1])<imm?1:0;
                break;
            case 4:
                opcode = "XORI";
                temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]^imm);
                break;
            case 6:
                opcode = "ORI";
                temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]|imm);
                break;
            case 7:
                opcode = "ANDI";
                temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]&imm);
                break;
            case 1:
                opcode = "SLLI";
                temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]<<(imm&31));
                break;
            case 5:
                if((instruction.I.imm11_0>>5) == 0){
                    opcode = "SRLI";
                    temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]>>(imm&31));
                }
                else{
                    opcode = "SRAI";
                    temp_register[instruction.I.rd] = (int)((uint32_t)(temp_register[instruction.I.rs1])>>(imm&31));
                }
                break;
            default:
                break;
            }
            if(flag)
                printf("%s %s, %s, %d\n", opcode, register_list[instruction.I.rd], register_list[instruction.I.rs1], imm);
            pc += 4;
            break;
        case 103:   // 1100111 I type
            signedbit = (instruction.I.imm11_0>>11);
            imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
            opcode = "JALR";
            temp_register[instruction.I.rd] = pc+4;
            pc = (temp_register[instruction.I.rs1]+imm);
            if(flag)
                printf("%s %s, %s, %d\n", opcode, register_list[instruction.I.rd], register_list[instruction.I.rs1], imm);
            break;
        case 115:   // 1110011 I type
            if(instruction.I.imm11_0 == 0){
                opcode = "ECALL";
                if(flag)
                    printf("%s\n", opcode);
                if(temp_register[17]==64){
                    if(temp_register[10]==1){
                        for(int i=0;i<temp_register[12];i++){
                            printf("%c", *((char*)(memory)+(temp_register[11]+i)));
                        }
                    }
                    temp_register[10]=temp_register[12];
                }
                else if(temp_register[17]==93){
                    printf("Program exited with code: %d\n", temp_register[10]);
                    end = true;
                }
            }
            else{
                opcode = "EBREAK";
                if(flag)
                    printf("%s\n", opcode);
            }
            pc += 4;
            break;
        case 35:    // 0100011 S type
            signedbit = (instruction.S.imm11_5>>6);
            imm = signedbit?((instruction.S.imm11_5<<5)+instruction.S.imm4_0)-(2<<11):((instruction.S.imm11_5<<5)+instruction.S.imm4_0);
            switch (instruction.S.funct3)
            {
            case 0:
                opcode = "SB";
                uint8_t byte;
                memcpy(&byte, (temp_register+instruction.S.rs2), sizeof(uint8_t));
                memcpy((uint8_t*)(memory)+(temp_register[instruction.S.rs1]+imm)*4/4, &byte, sizeof(uint8_t));
                break;
            case 1:
                opcode = "SH";
                uint16_t half;
                memcpy(&half, (temp_register+instruction.S.rs2), sizeof(uint16_t));
                memcpy((uint16_t*)(memory)+(temp_register[instruction.S.rs1]+imm)*2/4, &half, sizeof(uint16_t));
                break;
            case 2:
                opcode = "SW";
                uint32_t word;
                memcpy(&word, (temp_register+instruction.S.rs2), sizeof(uint32_t));
                memcpy(memory+(temp_register[instruction.S.rs1]+imm)/4, &word, sizeof(uint32_t));
                break;
            default:
                break;
            }
            pc += 4;
            if(flag)
                printf("%s %s, %d(%s)\n", opcode, register_list[instruction.S.rs2], imm, register_list[instruction.S.rs1]);
            break;
        case 99:    // 1100011 B type
            signedbit = instruction.B.imm12;
            imm = signedbit?(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+(instruction.B.imm4_1<<1)-(2<<11):(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+(instruction.B.imm4_1<<1);
            switch (instruction.B.funct3)
            {
            case 0:
                opcode = "BEQ";
                if(temp_register[instruction.B.rs1]==temp_register[instruction.B.rs2]){
                    pc += imm;
                }
                else
                    pc += 4;
                break;
            case 1:
                opcode = "BNE";
                if(temp_register[instruction.B.rs1]!=temp_register[instruction.B.rs2]){
                    pc += imm;
                }
                else
                    pc += 4;
                break;
            case 4:
                opcode = "BLT";
                if(temp_register[instruction.B.rs1]<temp_register[instruction.B.rs2]){
                    pc += imm;
                }
                else
                    pc += 4;
                break;
            case 5:
                opcode = "BGE";
                if(temp_register[instruction.B.rs1]>=temp_register[instruction.B.rs2]){
                    pc += imm;
                }
                else
                    pc += 4;
                break;
            case 6:
                opcode = "BLTU";
                if((uint32_t)(temp_register[instruction.B.rs1])<(uint32_t)(temp_register[instruction.B.rs2])){
                    pc += imm;
                }
                else
                    pc += 4;
                break;
            case 7:
                opcode = "BGEU";
                if((uint32_t)(temp_register[instruction.B.rs1])>=(uint32_t)(temp_register[instruction.B.rs2])){
                    pc += imm;
                }
                else
                    pc += 4;
                break;
            default:
                break;
            }
            if(flag)
                printf("%s %s, %s, %d\n", opcode, register_list[instruction.B.rs2], register_list[instruction.B.rs1], imm);
            break;
        case 55:    // 0110111 U type
            opcode = "LUI";
            temp_register[instruction.U.rd] = (instruction.U.imm31_12<<12);
            if(flag)
                printf("%s %s, %d\n", opcode, register_list[instruction.U.rd], instruction.U.imm31_12);
            pc += 4;
            break;
        case 23:    // 0010111 U type
            opcode = "AUIPC";
            temp_register[instruction.U.rd] = (instruction.U.imm31_12<<12)+pc;
            if(flag)
                printf("%s %s, %d\n", opcode, register_list[instruction.U.rd], instruction.U.imm31_12);
            pc += 4;
            break;
        case 111:   // 1101111 J type
            signedbit = instruction.J.imm20;
            imm = signedbit?(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1)-(2<<19):(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1);
            opcode = "JAL";
            temp_register[instruction.J.rd] = pc + 4;
            pc += imm;
            if(flag)
                printf("%s %s, %d\n", opcode, register_list[instruction.J.rd], imm);
            break;        
        default:
            break;
        }
    }
    //show_register(temp_register);
    delete memory;
}