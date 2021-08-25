#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include<string>

char register_list[32][5] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0",
                        "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
                        "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
                        "t3", "t4", "t5", "t6"};

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

#endif // INSTRUCTION_H
