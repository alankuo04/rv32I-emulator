#include<iostream>
#include<string>
#include"elf_function.h"
using namespace std;

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
    uint32_t r1;
    uint32_t r2;
    uint32_t amount;
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
            return {"ADD", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        case 1:
            return {"SLL", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        case 2:
            return {"SLT", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        case 3:
            return {"SLTU", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        case 4:
            return {"XOR", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        case 5:
            return {"SRL", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        case 6:
            return {"OR", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        case 7:
            return {"AND", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        default:
            break;
        }
        break;
    case 32:
        switch (instruction.R.funct3)
        {
        case 0:
            return {"SUB", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};
        case 6:
            return {"SRA", instruction.R.rd, instruction.R.rs1, instruction.R.rs2};        
        default:
            break;
        }
    default:
        break;
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
            return {"LB", instruction.I.rd, imm, instruction.I.rs1};
        case 1:
            return {"LH", instruction.I.rd, imm, instruction.I.rs1};
        case 2:
            return {"LW", instruction.I.rd, imm, instruction.I.rs1};
        case 4:
            return {"LBU", instruction.I.rd, imm, instruction.I.rs1};
        case 5:
            return {"LHU", instruction.I.rd, imm, instruction.I.rs1};
        default:
            break;
        }
        break;
    case 19:
        switch (instruction.I.funct3)
        {
        case 0:
            return {"ADDI", instruction.I.rd, instruction.I.rs1, imm};
        case 2:
            return {"SLTI", instruction.I.rd, instruction.I.rs1, imm};
        case 3:
            return {"SLTIU", instruction.I.rd, instruction.I.rs1, imm};
        case 4:
            return {"XORI", instruction.I.rd, instruction.I.rs1, imm};
        case 6:
            return {"ORI", instruction.I.rd, instruction.I.rs1, imm};
        case 7:
            return {"ANDI", instruction.I.rd, instruction.I.rs1, imm};
        case 1:
            return {"SLLI", instruction.I.rd, instruction.I.rs1, imm};
        case 5:
            if(instruction.I.imm11_0>>5 == 0)
                return {"SRLI", instruction.I.rd, instruction.I.rs1, imm};
            else
                return {"SRAI", instruction.I.rd, instruction.I.rs1, imm};
        default:
            break;
        }
        break;
    case 103:
        return {"JALR", instruction.I.rd, imm, instruction.I.rs1};
    case 115:
        if(instruction.I.imm11_0 == 0)
            return {"ECALL", 0, 0, 0};
        else
            return {"EBREAK", 0 ,0 ,0};
    default:
        break;
    }
    
}

Assembly get_S_type(Instruction instruction){
    int signedbit = (instruction.S.imm11_5>>6);
    uint32_t imm = signedbit?((instruction.S.imm11_5<<5)+instruction.S.imm4_0)-(2<<11):((instruction.S.imm11_5<<5)+instruction.S.imm4_0);
    switch (instruction.S.funct3)
    {
    case 0:
        return {"SB", instruction.S.rs2, imm, instruction.S.rs1};
    case 1:
        return {"SH", instruction.S.rs2, imm, instruction.S.rs1};
    case 2:
        return {"SW", instruction.S.rs2, imm, instruction.S.rs1};
    default:
        break;
    }
}

Assembly get_B_type(Instruction instruction){
    uint32_t imm = (instruction.B.imm12<<12)+(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+instruction.B.imm4_1;
    switch (instruction.B.funct3)
    {
    case 0:
        return {"BEQ", instruction.B.rs1, instruction.B.rs2, imm};
    case 1:
        return {"BNE", instruction.B.rs1, instruction.B.rs2, imm};
    case 4:
        return {"BLT", instruction.B.rs1, instruction.B.rs2, imm};
    case 5:
        return {"BGE", instruction.B.rs1, instruction.B.rs2, imm};
    case 6:
        return {"BLTU", instruction.B.rs1, instruction.B.rs2, imm};
    case 7:
        return {"BGEU", instruction.B.rs1, instruction.B.rs2, imm};
    default:
        break;
    }
}

Assembly get_U_type(Instruction instruction){
    switch (instruction.U.opcode)
    {
    case 55:
        return {"LUI", instruction.U.rd, instruction.U.imm31_12};
    case 23:
        return {"AUIPC", instruction.U.rd, instruction.U.imm31_12};
    default:
        break;
    }
}

Assembly get_J_type(Instruction instruction){
    uint32_t imm = (instruction.J.imm20<<20)+(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+instruction.J.imm10_1;
    return {"JAL", instruction.J.rd, imm};
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
        cout<<assembly.name<<" "<<assembly.r1<<" "<<int32_t(assembly.r2)<<" "<<int32_t(assembly.amount)<<""<<endl;
    }

}