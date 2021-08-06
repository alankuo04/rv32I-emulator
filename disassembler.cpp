#include<iostream>
#include<string>
#include"elf_function.h"
using namespace std;

string register_list[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", 
                        "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", 
                        "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", 
                        "t3", "t4", "t5", "t6"};

union Instruction
{
    uint32_t instruction;
    struct General{
        uint32_t opcode :   7;
        uint32_t other  :   25;
    }General;
    struct R{
        uint32_t opcode :   7;
        uint32_t rd     :   5;
        uint32_t funct3 :   3;
        uint32_t rs1    :   5;
        uint32_t rs2    :   5;
        uint32_t funct7 :   7;
    }R;
    struct I{
        uint32_t opcode :   7;
        uint32_t rd     :   5;
        uint32_t funct3 :   3;
        uint32_t rs1    :   5;
        uint32_t imm11_0:   12;
    }I;
    struct S{
        uint32_t opcode :   7;
        uint32_t imm4_0 :   5;
        uint32_t funct3 :   3;
        uint32_t rs1    :   5;
        uint32_t rs2    :   5;
        uint32_t imm11_5:   7;
    }S;
    struct B{
        uint32_t opcode :   7;
        uint32_t imm11  :   1;
        uint32_t imm4_1 :   4;
        uint32_t funct3 :   3;
        uint32_t rs1    :   5;
        uint32_t rs2    :   5;
        uint32_t imm10_5:   6;
        uint32_t imm12  :   1;
    }B;
    struct U{
        uint32_t opcode     :   7;
        uint32_t rd         :   5;
        uint32_t imm31_12   :   20;
    }U;
    struct J{
        uint32_t opcode     :   7;
        uint32_t rd         :   5;
        uint32_t imm19_12   :   8;
        uint32_t imm11      :   1;
        uint32_t imm10_1    :   10;
        uint32_t imm20      :   1;
    }J;
};

struct Assembly{
    string name;
    string r1;
    string r2;
    string amount;
};

char get_instruction_type(Instruction instruction){
    //cout<<instruction.General.opcode<<endl;
    switch (instruction.General.opcode)
    {
    case 51:    // 0110011 R type
        return 'R';
    case 3:     // 0000011 I type
    case 19:    // 0010011 I type
    case 103:   // 1100111 I type
    case 115:   // 1110011 I type
        return 'I';
    case 35:    // 0100011 S type
        return 'S';
    case 99:    // 1100011 B type
        return 'B';
    case 55:    // 0110111 U type
    case 23:    // 0010111 U type
        return 'U';
    case 111:   // 1101111 J type
        return 'J';
    default:
        return 0;
    }
}

Assembly get_R_type(Instruction instruction){
    switch (instruction.R.funct7)
    {
    case 0:
        switch (instruction.R.funct3)
        {
        case 0:
            return {"ADD", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        case 1:
            return {"SLL", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        case 2:
            return {"SLT", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        case 3:
            return {"SLTU", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        case 4:
            return {"XOR", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        case 5:
            return {"SRL", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        case 6:
            return {"OR", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        case 7:
            return {"AND", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        default:
            break;
        }
    case 32:
        switch (instruction.R.funct3)
        {
        case 0:
            return {"SUB", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};
        case 6:
            return {"SRA", register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]};        
        default:
            break;
        }
    default:
        return {"", "", "", ""};
    }
}

Assembly get_I_type(Instruction instruction){
    int signedbit = (instruction.I.imm11_0>>11);
    uint32_t imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
    switch (instruction.I.opcode)
    {
    case 3:
        switch (instruction.I.funct3)
        {
        case 0:
            return {"LB", register_list[instruction.I.rd], to_string(int(imm)), register_list[instruction.I.rs1]};
        case 1:
            return {"LH", register_list[instruction.I.rd], to_string(int(imm)), register_list[instruction.I.rs1]};
        case 2:
            return {"LW", register_list[instruction.I.rd], to_string(int(imm)), register_list[instruction.I.rs1]};
        case 4:
            return {"LBU", register_list[instruction.I.rd], to_string(int(imm)), register_list[instruction.I.rs1]};
        case 5:
            return {"LHU", register_list[instruction.I.rd], to_string(int(imm)), register_list[instruction.I.rs1]};
        default:
            break;
        }
    case 19:
        switch (instruction.I.funct3)
        {
        case 0:
            return {"ADDI", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
        case 2:
            return {"SLTI", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
        case 3:
            return {"SLTIU", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
        case 4:
            return {"XORI", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
        case 6:
            return {"ORI", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
        case 7:
            return {"ANDI", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
        case 1:
            return {"SLLI", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
        case 5:
            if(instruction.I.imm11_0>>5 == 0)
                return {"SRLI", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
            else
                return {"SRAI", register_list[instruction.I.rd], register_list[instruction.I.rs1], to_string(int(imm))};
        default:
            break;
        }
    case 103:
        return {"JALR", register_list[instruction.I.rd], to_string(int(imm)), register_list[instruction.I.rs1]};
    case 115:
        if(instruction.I.imm11_0 == 0)
            return {"ECALL"};
        else
            return {"EBREAK"};
    default:
        return {"", "", "", ""};
    }
}

Assembly get_S_type(Instruction instruction){
    int signedbit = (instruction.S.imm11_5>>6);
    uint32_t imm = signedbit?((instruction.S.imm11_5<<5)+instruction.S.imm4_0)-(2<<11):((instruction.S.imm11_5<<5)+instruction.S.imm4_0);
    switch (instruction.S.funct3)
    {
    case 0:
        return {"SB", register_list[instruction.S.rs2], to_string(int(imm)), register_list[instruction.S.rs1]};
    case 1:
        return {"SH", register_list[instruction.S.rs2], to_string(int(imm)), register_list[instruction.S.rs1]};
    case 2:
        return {"SW", register_list[instruction.S.rs2], to_string(int(imm)), register_list[instruction.S.rs1]};
    default:
        return {"", "", "", ""};
    }
}

Assembly get_B_type(Instruction instruction){
    uint32_t imm = (instruction.B.imm12<<12)+(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+instruction.B.imm4_1;
    switch (instruction.B.funct3)
    {
    case 0:
        return {"BEQ", register_list[instruction.B.rs1], register_list[instruction.B.rs2], to_string(int(imm))};
    case 1:
        return {"BNE", register_list[instruction.B.rs1], register_list[instruction.B.rs2], to_string(int(imm))};
    case 4:
        return {"BLT", register_list[instruction.B.rs1], register_list[instruction.B.rs2], to_string(int(imm))};
    case 5:
        return {"BGE", register_list[instruction.B.rs1], register_list[instruction.B.rs2], to_string(int(imm))};
    case 6:
        return {"BLTU", register_list[instruction.B.rs1], register_list[instruction.B.rs2], to_string(int(imm))};
    case 7:
        return {"BGEU", register_list[instruction.B.rs1], register_list[instruction.B.rs2], to_string(int(imm))};
    default:
        return {"", "", "", ""};
    }
}

Assembly get_U_type(Instruction instruction){
    switch (instruction.U.opcode)
    {
    case 55:
        return {"LUI", register_list[instruction.U.rd], to_string(instruction.U.imm31_12)};
    case 23:
        return {"AUIPC", register_list[instruction.U.rd], to_string(instruction.U.imm31_12)};
    default:
        return {"", "", "", ""};
    }
}

Assembly get_J_type(Instruction instruction){
    uint32_t imm = (instruction.J.imm20<<20)+(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1);
    return {"JAL", register_list[instruction.J.rd], to_string(int(imm))};
}

int main(int argc, char **argv){
    Text_section *text = get_text_section(argv[1]);
    for(int i=0;i<text->size;i++){
        printf("%08X ", text->text_section[i]);
        Instruction instruction = {text->text_section[i]};
        //cout<<get_instruction_type(instruction)<<endl;
        Assembly assembly;
        switch (get_instruction_type(instruction))
        {
        case 'R':
            assembly = get_R_type(instruction);
            break;
        case 'I':
            assembly = get_I_type(instruction);
            break;
        case 'S':
            assembly = get_S_type(instruction);
            break;
        case 'B':
            assembly = get_B_type(instruction);
            break;
        case 'U':
            assembly = get_U_type(instruction);
            break;
        case 'J':
            assembly = get_J_type(instruction);
            break;
        default:
            break;
        }
        cout<<assembly.name<<" "<<assembly.r1<<" "<<assembly.r2<<" "<<assembly.amount<<endl;
    }

}