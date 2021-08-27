#include <QFile>
#include <QDebug>
#include "emulator.h"
#include "instruction.h"

Emulator::Emulator(QString filePath)
{
    memory = new uint32_t[MAX_MEMORY_SIZE];
    temp_register = new int[32];
    end = false;
    temp_register[2] = MAX_MEMORY_SIZE*4;
    QFile file(filePath);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            ELF_Header *elf_header = new ELF_Header;
            Program_Header *program_header = new Program_Header;
            char buf[1024];
            file.read(buf, sizeof(ELF_Header));
            memcpy(elf_header, buf, sizeof(ELF_Header));
            entry = pc = elf_header->e_entry;
            for(int i=0;i<elf_header->e_phnum;i++)
            {
                file.seek(elf_header->e_phoff+elf_header->e_phentsize*i);
                file.read(buf, sizeof(Program_Header));
                memcpy(program_header, buf, sizeof(Program_Header));
                if(program_header->p_type == 1)
                {
                    file.seek(program_header->p_offset);
                    file.read((char*)memory+program_header->p_vaddr, program_header->p_memsz);
                }
            }
            delete elf_header;
            delete program_header;
            file.close();
        }
    }
}

QString Emulator::nextInstruction()
{
    int signedbit;
    int imm;
    temp_register[0] = 0;
    QString str = "";
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
                // opcode = "ADD";
                temp_register[instruction.R.rd] = temp_register[instruction.R.rs1]+temp_register[instruction.R.rs2];
                break;
            case 1:
                // opcode = "SLL";
                temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]<<(temp_register[instruction.R.rs2]&31));
                break;
            case 2:
                // opcode = "SLT";
                temp_register[instruction.R.rd] = temp_register[instruction.R.rs1]<temp_register[instruction.R.rs2]?1:0;
                break;
            case 3:
                // opcode = "SLTU";
                temp_register[instruction.R.rd] = (uint32_t)(temp_register[instruction.R.rs1])<(uint32_t)(temp_register[instruction.R.rs2])?1:0;
                break;
            case 4:
                // opcode = "XOR";
                temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]^temp_register[instruction.R.rs2]);
                break;
            case 5:
                // opcode = "SRL";
                temp_register[instruction.R.rd] = ((uint32_t)(temp_register[instruction.R.rs1])>>(temp_register[instruction.R.rs2]&31));
                break;
            case 6:
                // opcode = "OR";
                temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]|temp_register[instruction.R.rs2]);
                break;
            case 7:
                // opcode = "AND";
                temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]&temp_register[instruction.R.rs2]);
                break;
            }
            break;
        case 32:
            switch (instruction.R.funct3)
            {
            case 0:
                // opcode = "SUB";
                temp_register[instruction.R.rd] = temp_register[instruction.R.rs1]-temp_register[instruction.R.rs2];
                break;
            case 6:
                // opcode = "SRA";
                temp_register[instruction.R.rd] = (temp_register[instruction.R.rs1]>>(temp_register[instruction.R.rs2]&31));
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        pc += 4;
        break;
    case 3:     // 0000011 I type
        signedbit = (instruction.I.imm11_0>>11);
        imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
        switch (instruction.I.funct3)
        {
        case 0:
            // opcode = "LB";
            int8_t byte;
            memcpy(&byte, (int8_t*)(memory)+(temp_register[instruction.I.rs1]+imm)*4/4, sizeof(int8_t));
            temp_register[instruction.I.rd] = byte;
            break;
        case 1:
            // opcode = "LH";
            int16_t half;
            memcpy(&half, (int16_t*)(memory)+(temp_register[instruction.I.rs1]+imm)*2/4, sizeof(int16_t));
            temp_register[instruction.I.rd] = half;
            break;
        case 2:
            // opcode = "LW";
            int32_t word;
            memcpy(&word, memory+(temp_register[instruction.I.rs1]+imm)/4, sizeof(int32_t));
            temp_register[instruction.I.rd] = word;
            break;
        case 4:
            // opcode = "LBU";
            uint8_t u_byte;
            memcpy(&u_byte, (uint8_t*)(memory)+(temp_register[instruction.I.rs1]+imm)*4/4, sizeof(uint8_t));
            temp_register[instruction.I.rd] = u_byte;
            break;
        case 5:
            // opcode = "LHU";
            uint16_t u_half;
            memcpy(&u_half, (uint16_t*)(memory)+(temp_register[instruction.I.rs1]+imm)*2/4, sizeof(uint16_t));
            temp_register[instruction.I.rd] = u_half;
            break;
        default:
            break;
        }
        pc += 4;
        break;
    case 19:    // 0010011 I type
        signedbit = (instruction.I.imm11_0>>11);
        imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
        switch (instruction.I.funct3)
        {
        case 0:
            // opcode = "ADDI";
            temp_register[instruction.I.rd] = temp_register[instruction.I.rs1]+imm;
            break;
        case 2:
            // opcode = "SLTI";
            temp_register[instruction.I.rd] = temp_register[instruction.I.rs1]<imm?1:0;
            break;
        case 3:
            // opcode = "SLTIU";
            temp_register[instruction.I.rd] = (uint32_t)(temp_register[instruction.I.rs1])<(uint32_t)(imm)?1:0;
            break;
        case 4:
            // opcode = "XORI";
            temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]^imm);
            break;
        case 6:
            // opcode = "ORI";
            temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]|imm);
            break;
        case 7:
            // opcode = "ANDI";
            temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]&imm);
            break;
        case 1:
            // opcode = "SLLI";
            temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]<<(imm&31));
            break;
        case 5:
            if((instruction.I.imm11_0>>5) == 0){
                // opcode = "SRLI";
                temp_register[instruction.I.rd] = (temp_register[instruction.I.rs1]>>(imm&31));
            }
            else{
                // opcode = "SRAI";
                temp_register[instruction.I.rd] = (int)((uint32_t)(temp_register[instruction.I.rs1])>>(imm&31));
            }
            break;
        default:
            break;
        }
        pc += 4;
        break;
    case 103:   // 1100111 I type
        signedbit = (instruction.I.imm11_0>>11);
        imm = signedbit?instruction.I.imm11_0-(2<<11):instruction.I.imm11_0;
        // opcode = "JALR";
        temp_register[instruction.I.rd] = pc+4;
        pc = (temp_register[instruction.I.rs1]+imm);
        break;
    case 115:   // 1110011 I type
        if(instruction.I.imm11_0 == 0){
            // opcode = "ECALL";
            if(temp_register[17]==64){
                if(temp_register[10]==1){
                    for(int i=0;i<temp_register[12];i++){
                        str.append(*((char*)(memory)+(temp_register[11]+i)));
                        //printf("%c", *((char*)(memory)+(temp_register[11]+i)));
                    }
                }
                temp_register[10]=temp_register[12];
            }
            else if(temp_register[17]==93){
                end = true;
                str = QString("Program exited with code: %1\n").arg(temp_register[10]);
            }
        }
        else
        {
            // opcode = "EBREAK";
        }
        pc += 4;
        break;
    case 35:    // 0100011 S type
        signedbit = (instruction.S.imm11_5>>6);
        imm = signedbit?((instruction.S.imm11_5<<5)+instruction.S.imm4_0)-(2<<11):((instruction.S.imm11_5<<5)+instruction.S.imm4_0);
        switch (instruction.S.funct3)
        {
        case 0:
            // opcode = "SB";
            uint8_t byte;
            memcpy(&byte, (temp_register+instruction.S.rs2), sizeof(uint8_t));
            memcpy((uint8_t*)(memory)+(temp_register[instruction.S.rs1]+imm)*4/4, &byte, sizeof(uint8_t));
            break;
        case 1:
            // opcode = "SH";
            uint16_t half;
            memcpy(&half, (temp_register+instruction.S.rs2), sizeof(uint16_t));
            memcpy((uint16_t*)(memory)+(temp_register[instruction.S.rs1]+imm)*2/4, &half, sizeof(uint16_t));
            break;
        case 2:
            // opcode = "SW";
            uint32_t word;
            memcpy(&word, (temp_register+instruction.S.rs2), sizeof(uint32_t));
            memcpy(memory+(temp_register[instruction.S.rs1]+imm)/4, &word, sizeof(uint32_t));
            break;
        default:
            break;
        }
        pc += 4;
        break;
    case 99:    // 1100011 B type
        signedbit = instruction.B.imm12;
        imm = signedbit?(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+(instruction.B.imm4_1<<1)-(2<<11):(instruction.B.imm11<<11)+(instruction.B.imm10_5<<5)+(instruction.B.imm4_1<<1);
        switch (instruction.B.funct3)
        {
        case 0:
            // opcode = "BEQ";
            if(temp_register[instruction.B.rs1]==temp_register[instruction.B.rs2]){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 1:
            // opcode = "BNE";
            if(temp_register[instruction.B.rs1]!=temp_register[instruction.B.rs2]){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 4:
            // opcode = "BLT";
            if(temp_register[instruction.B.rs1]<temp_register[instruction.B.rs2]){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 5:
            // opcode = "BGE";
            if(temp_register[instruction.B.rs1]>=temp_register[instruction.B.rs2]){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 6:
            // opcode = "BLTU";
            if((uint32_t)(temp_register[instruction.B.rs1])<(uint32_t)(temp_register[instruction.B.rs2])){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 7:
            // opcode = "BGEU";
            if((uint32_t)(temp_register[instruction.B.rs1])>=(uint32_t)(temp_register[instruction.B.rs2])){
                pc += imm;
            }
            else
                pc += 4;
            break;
        default:
            break;
        }
        break;
    case 55:    // 0110111 U type
        // opcode = "LUI";
        temp_register[instruction.U.rd] = (instruction.U.imm31_12<<12);
        pc += 4;
        break;
    case 23:    // 0010111 U type
        // // opcode = "AUIPC";
        temp_register[instruction.U.rd] = (instruction.U.imm31_12<<12)+pc;
        pc += 4;
        break;
    case 111:   // 1101111 J type
        signedbit = instruction.J.imm20;
        imm = signedbit?(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1)-(2<<19):(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1);
        // opcode = "JAL";
        temp_register[instruction.J.rd] = pc + 4;
        pc += imm;
        break;
    default:
        break;
    }
    //qDebug()<<QString::number(pc, 16);
    return str;
}

int Emulator::getPC()
{
    return pc;
}

bool Emulator::isEnd()
{
    return end;
}
