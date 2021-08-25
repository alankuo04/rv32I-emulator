#include "disassembler.h"
#include "instruction.h"
#include<QDebug>

Disassembler::Disassembler(uint32_t binary_code)
{
    Instruction instruction = {binary_code};
    char const *opcode = "";
    int signedbit;
    int imm = 0;
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
        str = QString("%1 %2, %3, %4\n").arg(opcode, register_list[instruction.R.rd], register_list[instruction.R.rs1], register_list[instruction.R.rs2]);
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
        str = QString("%1 %2, %3(%4)\n").arg(opcode, register_list[instruction.I.rd]).arg(imm).arg(register_list[instruction.I.rs1]);
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
            if((instruction.I.imm11_0>>5) == 0){
                opcode = "SRLI";
            }
            else{
                opcode = "SRAI";
            }
            break;
        default:
            break;
        }
        str = QString("%1 %2, %3, %4\n").arg(opcode, register_list[instruction.I.rd], register_list[instruction.I.rs1]).arg(imm);
        break;
    case 103:   // 1100111 I type
        signedbit = (instruction.I.imm11_0>>11);
        imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
        opcode = "JALR";
        str = QString("%1 %2, %3, %4\n").arg(opcode, register_list[instruction.I.rd], register_list[instruction.I.rs1]).arg(imm);
        break;
    case 115:   // 1110011 I type
        if(instruction.I.imm11_0 == 0){
            opcode = "ECALL";
        }
        else{
            opcode = "EBREAK";
        }
        str = QString("%1\n").arg(opcode);
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
        str = QString("%1 %2, %3(%4)\n").arg(opcode, register_list[instruction.S.rs2]).arg(imm).arg(register_list[instruction.S.rs1]);
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
        str = QString("%1 %2, %3, %4\n").arg(opcode, register_list[instruction.B.rs2], register_list[instruction.B.rs1]).arg(imm);
        //str = QString("%1 %2, %3, %4\n").arg(opcode).arg(register_list[instruction.B.rs2]).arg(register_list[instruction.B.rs1]).arg(imm);
        break;
    case 55:    // 0110111 U type
        opcode = "LUI";
        str = QString("%1 %2, %3\n").arg(opcode, register_list[instruction.U.rd]).arg(instruction.U.imm31_12);
        break;
    case 23:    // 0010111 U type
        opcode = "AUIPC";
        str = QString("%1 %2, %3\n").arg(opcode, register_list[instruction.U.rd]).arg(instruction.U.imm31_12);
        break;
    case 111:   // 1101111 J type
        signedbit = instruction.J.imm20;
        imm = signedbit?(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1)-(2<<19):(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1);
        opcode = "JAL";
        str = QString("%1 %2, %3\n").arg(opcode, register_list[instruction.J.rd]).arg(imm);
        break;
    default:
        break;
    }
}

QString Disassembler::getInstruction()
{
    return str;
}
