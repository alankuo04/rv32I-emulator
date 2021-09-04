#include<string>
#include"dis_function.h"
using namespace std;

string register_list[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", 
                        "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", 
                        "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", 
                        "t3", "t4", "t5", "t6"};

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
        return {"", 0, 0, 0};
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
            return {"LB", instruction.I.rd, instruction.I.rs1, imm};
        case 1:
            return {"LH", instruction.I.rd, instruction.I.rs1, imm};
        case 2:
            return {"LW", instruction.I.rd, instruction.I.rs1, imm};
        case 4:
            return {"LBU", instruction.I.rd, instruction.I.rs1, imm};
        case 5:
            return {"LHU", instruction.I.rd, instruction.I.rs1, imm};
        default:
            break;
        }
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
                return {"SRAI", instruction.I.rd, instruction.I.rs1, imm-(2<<9)};
        default:
            break;
        }
    case 103:
        return {"JALR", instruction.I.rd, instruction.I.rs1, imm};
    case 115:
        if(instruction.I.imm11_0 == 0)
            return {"ECALL"};
        else
            return {"EBREAK"};
    default:
        return {"", 0, 0, 0};
    }
}

Assembly get_S_type(Instruction instruction){
    int signedbit = (instruction.S.imm11_5>>6);
    uint32_t imm = signedbit?((instruction.S.imm11_5<<5)+instruction.S.imm4_0)-(2<<11):((instruction.S.imm11_5<<5)+instruction.S.imm4_0);
    switch (instruction.S.funct3)
    {
    case 0:
        return {"SB", instruction.S.rs2, instruction.S.rs1, imm};
    case 1:
        return {"SH", instruction.S.rs2, instruction.S.rs1, imm};
    case 2:
        return {"SW", instruction.S.rs2, instruction.S.rs1, imm};
    default:
        return {"", 0, 0, 0};
    }
}

Assembly get_B_type(Instruction instruction){
    int signedbit = instruction.B.imm12;
    uint32_t imm = signedbit?(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+(instruction.B.imm4_1<<1)-(2<<11):(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+(instruction.B.imm4_1<<1);
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
        return {"", 0, 0, 0};
    }
}

Assembly get_U_type(Instruction instruction){
    switch (instruction.U.opcode)
    {
    case 55:
        return {"LUI", instruction.U.rd, 0, instruction.U.imm31_12};
    case 23:
        return {"AUIPC", instruction.U.rd, 0, instruction.U.imm31_12};
    default:
        return {"", 0, 0, 0};
    }
}

Assembly get_J_type(Instruction instruction){
    int signedbit = instruction.J.imm20;
    uint32_t imm = signedbit?(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1)-(2<<19):(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1);
    return {"JAL", instruction.J.rd, 0, imm};
}