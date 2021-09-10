#include <QFile>
#include <QDebug>
#include "emulator.h"
#include "instruction.h"

Emulator::Emulator(QString filePath)
{
    memoryMap = new MemoryMapModel();
    registerMap = new RegisterMapModel();
    end = false;
    registerMap->temp_register[2] = MAX_MEMORY_SIZE*4;
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
                    file.read((char*)memoryMap->memory+program_header->p_vaddr, program_header->p_memsz);
                }
            }
            delete elf_header;
            delete program_header;
            file.close();
        }
    }
}

Emulator::~Emulator()
{
    delete registerMap;
    delete memoryMap;
}

QString Emulator::nextInstruction()
{
    int signedbit;
    int imm;
    registerMap->temp_register[0] = 0;
    QString str = "";
    Instruction instruction = {memoryMap->memory[pc/4]};
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
                registerMap->temp_register[instruction.R.rd] = registerMap->temp_register[instruction.R.rs1]+registerMap->temp_register[instruction.R.rs2];
                break;
            case 1:
                // opcode = "SLL";
                registerMap->temp_register[instruction.R.rd] = (registerMap->temp_register[instruction.R.rs1]<<(registerMap->temp_register[instruction.R.rs2]&31));
                break;
            case 2:
                // opcode = "SLT";
                registerMap->temp_register[instruction.R.rd] = registerMap->temp_register[instruction.R.rs1]<registerMap->temp_register[instruction.R.rs2]?1:0;
                break;
            case 3:
                // opcode = "SLTU";
                registerMap->temp_register[instruction.R.rd] = (uint32_t)(registerMap->temp_register[instruction.R.rs1])<(uint32_t)(registerMap->temp_register[instruction.R.rs2])?1:0;
                break;
            case 4:
                // opcode = "XOR";
                registerMap->temp_register[instruction.R.rd] = (registerMap->temp_register[instruction.R.rs1]^registerMap->temp_register[instruction.R.rs2]);
                break;
            case 5:
                // opcode = "SRL";
                registerMap->temp_register[instruction.R.rd] = ((uint32_t)(registerMap->temp_register[instruction.R.rs1])>>(registerMap->temp_register[instruction.R.rs2]&31));
                break;
            case 6:
                // opcode = "OR";
                registerMap->temp_register[instruction.R.rd] = (registerMap->temp_register[instruction.R.rs1]|registerMap->temp_register[instruction.R.rs2]);
                break;
            case 7:
                // opcode = "AND";
                registerMap->temp_register[instruction.R.rd] = (registerMap->temp_register[instruction.R.rs1]&registerMap->temp_register[instruction.R.rs2]);
                break;
            }
            break;
        case 32:
            switch (instruction.R.funct3)
            {
            case 0:
                // opcode = "SUB";
                registerMap->temp_register[instruction.R.rd] = registerMap->temp_register[instruction.R.rs1]-registerMap->temp_register[instruction.R.rs2];
                break;
            case 6:
                // opcode = "SRA";
                registerMap->temp_register[instruction.R.rd] = (registerMap->temp_register[instruction.R.rs1]>>(registerMap->temp_register[instruction.R.rs2]&31));
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
            memcpy(&byte, (int8_t*)(memoryMap->memory)+(registerMap->temp_register[instruction.I.rs1]+imm)*4/4, sizeof(int8_t));
            registerMap->temp_register[instruction.I.rd] = byte;
            break;
        case 1:
            // opcode = "LH";
            int16_t half;
            memcpy(&half, (int16_t*)(memoryMap->memory)+(registerMap->temp_register[instruction.I.rs1]+imm)*2/4, sizeof(int16_t));
            registerMap->temp_register[instruction.I.rd] = half;
            break;
        case 2:
            // opcode = "LW";
            int32_t word;
            memcpy(&word, memoryMap->memory+(registerMap->temp_register[instruction.I.rs1]+imm)/4, sizeof(int32_t));
            registerMap->temp_register[instruction.I.rd] = word;
            break;
        case 4:
            // opcode = "LBU";
            uint8_t u_byte;
            memcpy(&u_byte, (uint8_t*)(memoryMap->memory)+(registerMap->temp_register[instruction.I.rs1]+imm)*4/4, sizeof(uint8_t));
            registerMap->temp_register[instruction.I.rd] = u_byte;
            break;
        case 5:
            // opcode = "LHU";
            uint16_t u_half;
            memcpy(&u_half, (uint16_t*)(memoryMap->memory)+(registerMap->temp_register[instruction.I.rs1]+imm)*2/4, sizeof(uint16_t));
            registerMap->temp_register[instruction.I.rd] = u_half;
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
            registerMap->temp_register[instruction.I.rd] = registerMap->temp_register[instruction.I.rs1]+imm;
            break;
        case 2:
            // opcode = "SLTI";
            registerMap->temp_register[instruction.I.rd] = registerMap->temp_register[instruction.I.rs1]<imm?1:0;
            break;
        case 3:
            // opcode = "SLTIU";
            registerMap->temp_register[instruction.I.rd] = (uint32_t)(registerMap->temp_register[instruction.I.rs1])<(uint32_t)(imm)?1:0;
            break;
        case 4:
            // opcode = "XORI";
            registerMap->temp_register[instruction.I.rd] = (registerMap->temp_register[instruction.I.rs1]^imm);
            break;
        case 6:
            // opcode = "ORI";
            registerMap->temp_register[instruction.I.rd] = (registerMap->temp_register[instruction.I.rs1]|imm);
            break;
        case 7:
            // opcode = "ANDI";
            registerMap->temp_register[instruction.I.rd] = (registerMap->temp_register[instruction.I.rs1]&imm);
            break;
        case 1:
            // opcode = "SLLI";
            registerMap->temp_register[instruction.I.rd] = (registerMap->temp_register[instruction.I.rs1]<<(imm&31));
            break;
        case 5:
            if((instruction.I.imm11_0>>5) == 0){
                // opcode = "SRLI";
                registerMap->temp_register[instruction.I.rd] = (registerMap->temp_register[instruction.I.rs1]>>(imm&31));
            }
            else{
                // opcode = "SRAI";
                registerMap->temp_register[instruction.I.rd] = (int)((uint32_t)(registerMap->temp_register[instruction.I.rs1])>>(imm&31));
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
        registerMap->temp_register[instruction.I.rd] = pc+4;
        pc = (registerMap->temp_register[instruction.I.rs1]+imm);
        break;
    case 115:   // 1110011 I type
        if(instruction.I.imm11_0 == 0){
            // opcode = "ECALL";
            if(registerMap->temp_register[17]==57){ // close

            }
            else if(registerMap->temp_register[17]==62){ // lseek

            }
            else if(registerMap->temp_register[17]==63){ // read
                if(registerMap->temp_register[10]==0){
                    emit getStdin();
                    end = true;
                }
            }
            else if(registerMap->temp_register[17]==64){ // write
                if(registerMap->temp_register[10]==1){
                    for(int i=0;i<registerMap->temp_register[12];i++){
                        str.append(*((char*)(memoryMap->memory)+(registerMap->temp_register[11]+i)));
                        //printf("%c", *((char*)(memoryMap->memory)+(registerMap->temp_register[11]+i)));
                    }
                }
                registerMap->temp_register[10]=registerMap->temp_register[12];
            }
            else if(registerMap->temp_register[17]==80){ // fstat
                registerMap->temp_register[10] = 0;
            }
            else if(registerMap->temp_register[17]==93){ // exit with code
                end = true;
                str = QString("Program exited with code: %1\n").arg(registerMap->temp_register[10]);
            }
            else if(registerMap->temp_register[17]==214){ // brk
                registerMap->temp_register[10] = 0;
            }
            end = true;
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
            memcpy(&byte, (registerMap->temp_register+instruction.S.rs2), sizeof(uint8_t));
            memcpy((uint8_t*)(memoryMap->memory)+(registerMap->temp_register[instruction.S.rs1]+imm)*4/4, &byte, sizeof(uint8_t));
            break;
        case 1:
            // opcode = "SH";
            uint16_t half;
            memcpy(&half, (registerMap->temp_register+instruction.S.rs2), sizeof(uint16_t));
            memcpy((uint16_t*)(memoryMap->memory)+(registerMap->temp_register[instruction.S.rs1]+imm)*2/4, &half, sizeof(uint16_t));
            break;
        case 2:
            // opcode = "SW";
            uint32_t word;
            memcpy(&word, (registerMap->temp_register+instruction.S.rs2), sizeof(uint32_t));
            memcpy(memoryMap->memory+(registerMap->temp_register[instruction.S.rs1]+imm)/4, &word, sizeof(uint32_t));
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
            if(registerMap->temp_register[instruction.B.rs1]==registerMap->temp_register[instruction.B.rs2]){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 1:
            // opcode = "BNE";
            if(registerMap->temp_register[instruction.B.rs1]!=registerMap->temp_register[instruction.B.rs2]){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 4:
            // opcode = "BLT";
            if(registerMap->temp_register[instruction.B.rs1]<registerMap->temp_register[instruction.B.rs2]){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 5:
            // opcode = "BGE";
            if(registerMap->temp_register[instruction.B.rs1]>=registerMap->temp_register[instruction.B.rs2]){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 6:
            // opcode = "BLTU";
            if((uint32_t)(registerMap->temp_register[instruction.B.rs1])<(uint32_t)(registerMap->temp_register[instruction.B.rs2])){
                pc += imm;
            }
            else
                pc += 4;
            break;
        case 7:
            // opcode = "BGEU";
            if((uint32_t)(registerMap->temp_register[instruction.B.rs1])>=(uint32_t)(registerMap->temp_register[instruction.B.rs2])){
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
        registerMap->temp_register[instruction.U.rd] = (instruction.U.imm31_12<<12);
        pc += 4;
        break;
    case 23:    // 0010111 U type
        // // opcode = "AUIPC";
        registerMap->temp_register[instruction.U.rd] = (instruction.U.imm31_12<<12)+pc;
        pc += 4;
        break;
    case 111:   // 1101111 J type
        signedbit = instruction.J.imm20;
        imm = signedbit?(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1)-(2<<19):(instruction.J.imm19_12<<12)+(instruction.J.imm11<<11)+(instruction.J.imm10_1<<1);
        // opcode = "JAL";
        registerMap->temp_register[instruction.J.rd] = pc + 4;
        pc += imm;
        break;
    default:
        break;
    }
    //qDebug()<<QString::number(pc, 16);
    return str;
}

void Emulator::setStdin(QString str)
{
    qDebug()<<str<<" "<<str.length();
    qDebug()<<registerMap->temp_register[10]<<" "<<registerMap->temp_register[11]<<" "<<registerMap->temp_register[12]<<" "<<registerMap->temp_register[17];
    if(registerMap->temp_register[17]==63){
        for(int i=0;i<registerMap->temp_register[12] && i<str.length();i++){
            qDebug()<<registerMap->temp_register[11]+i<<" "<<str.at(i).toLatin1();
            uint8_t byte = str.at(i).toLatin1();
            memcpy((uint8_t*)(memoryMap->memory)+(registerMap->temp_register[11]+i)*4/4, &byte, sizeof(uint8_t));
            //memoryMap->memory[registerMap->temp_register[11]+i] = str.at(i).toLatin1();
        }
        registerMap->temp_register[10] = str.length();
    }
    end = false;
}

RegisterMapModel* Emulator::getRegisterMapModel()
{
    return registerMap;
}

MemoryMapModel* Emulator::getMemoryMapModel()
{
    return memoryMap;
}

void Emulator::updateRegisterMapModel()
{
    registerMap->registerMapChanged();
}

void Emulator::updateMemoryMapModel()
{
    memoryMap->memoryMapChanged();
}

int Emulator::getPC()
{
    return pc;
}

int Emulator::getEntry()
{
    return entry;
}

bool Emulator::isEnd()
{
    return end;
}
