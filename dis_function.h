#include<string>

extern std::string register_list[];

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
    std::string name;
    uint32_t r1;
    uint32_t r2;
    uint32_t amount;
};

char get_instruction_type(Instruction instruction);

Assembly get_R_type(Instruction instruction);

Assembly get_I_type(Instruction instruction);

Assembly get_S_type(Instruction instruction);

Assembly get_B_type(Instruction instruction);

Assembly get_U_type(Instruction instruction);

Assembly get_J_type(Instruction instruction);