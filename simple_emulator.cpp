#include<iostream>
#include<string>
#include<cstring>
#include"elf_function.h"
#include"dis_function.h"
using namespace std;

void show_register(int* temp_register){
    for(int i=0;i<32;i++){
        cout<<register_list[i]<<" "<<temp_register[i]<<endl;
    }
}

void show_memory(char* temp_memory){
    for(int i=0;i<32;i++){
        cout<<i<<" "<<temp_memory[i]<<endl;
    }
}

int main(int argc, char **argv){
    Text_section *text = get_text_section(argv[1]);
    string next;
    int i=0;
    int pc=0;
    int temp_register[32]={};
    char temp_memory[2048]={};
    temp_register[2] = 2048;
    while(getline(cin, next) && pc/4 < text->size){
        //cout<<text->text_section[i]<<endl;
        cout<<i<<endl;
        Instruction instruction = {text->text_section[pc/4]};
        Assembly assembly;
        switch (get_instruction_type(instruction))
        {
        case 'R':
            assembly = get_R_type(instruction);
            if(assembly.name=="ADD")
                temp_register[assembly.r1] = temp_register[assembly.r2]+temp_register[assembly.amount];
            else if(assembly.name=="SUB")
                temp_register[assembly.r1] = temp_register[assembly.r2]-temp_register[assembly.amount];
            else if(assembly.name=="SLL")
                temp_register[assembly.r1] = (temp_register[assembly.r2]<<((temp_register[assembly.amount]<<27)>>27));
            else if(assembly.name=="SLT")
                temp_register[assembly.r1] = temp_register[assembly.r2]<temp_register[assembly.amount]?1:0;
            else if(assembly.name=="SLTU")
                temp_register[assembly.r1] = temp_register[assembly.r2]<temp_register[assembly.amount]?1:0;
            else if(assembly.name=="XOR")
                temp_register[assembly.r1] = (temp_register[assembly.r2]^temp_register[assembly.amount]);
            else if(assembly.name=="SRL")
                temp_register[assembly.r1] = (temp_register[assembly.r2]>>((temp_register[assembly.amount]<<27)>>27));
            else if(assembly.name=="SRA")
                temp_register[assembly.r1] = (temp_register[assembly.r2]>>((temp_register[assembly.amount]<<27)>>27));
            else if(assembly.name=="OR")
                temp_register[assembly.r1] = (temp_register[assembly.r2]|temp_register[assembly.amount]);
            else if(assembly.name=="AND")
                temp_register[assembly.r1] = (temp_register[assembly.r2]&temp_register[assembly.amount]);
            pc+=4;
            break;
        case 'I':
            assembly = get_I_type(instruction);
            if(assembly.name=="LB"){
                int8_t byte;
                memcpy((void*)&byte, (void*)&temp_memory[temp_register[assembly.r2]+int(assembly.amount)], sizeof(int8_t));
                temp_register[assembly.r1] = (int8_t)byte;
            }
            else if(assembly.name=="LH"){
                int16_t halfword;
                memcpy((void*)&halfword, (void*)&temp_memory[temp_register[assembly.r2]+int(assembly.amount)], sizeof(int16_t));
                temp_register[assembly.r1] = (int16_t)halfword;
            }
            else if(assembly.name=="LW"){
                int32_t word;
                memcpy((void*)&word, (void*)&temp_memory[temp_register[assembly.r2]+int(assembly.amount)], sizeof(int32_t));
                temp_register[assembly.r1] = (int32_t)word;
            }
            else if(assembly.name=="LBU"){
                uint8_t byte;
                memcpy((void*)&byte, (void*)&temp_memory[temp_register[assembly.r2]+int(assembly.amount)], sizeof(uint8_t));
                temp_register[assembly.r1] = (uint8_t)byte;
            }
            else if(assembly.name=="LHU"){
                uint16_t halfword;
                memcpy((void*)&halfword, (void*)&temp_memory[temp_register[assembly.r2]+int(assembly.amount)], sizeof(uint16_t));
                temp_register[assembly.r1] = (uint16_t)halfword;
            }
            else if(assembly.name=="ADDI")
                temp_register[assembly.r1] = temp_register[assembly.r2]+int(assembly.amount);
            else if(assembly.name=="SLTI")
                temp_register[assembly.r1] = (temp_register[assembly.r2]<temp_register[int(assembly.amount)])?1:0;
            else if(assembly.name=="SLTIU")
                temp_register[assembly.r1] = (temp_register[assembly.r2]<temp_register[int(assembly.amount)])?1:0;
            else if(assembly.name=="XORI")
                temp_register[assembly.r1] = (temp_register[assembly.r2]^assembly.amount);
            else if(assembly.name=="ORI")
                temp_register[assembly.r1] = (temp_register[assembly.r2]|assembly.amount);
            else if(assembly.name=="ANDI")
                temp_register[assembly.r1] = (temp_register[assembly.r2]&assembly.amount);
            else if(assembly.name=="SLLI")
                temp_register[assembly.r1] = (temp_register[assembly.r2]<<int(assembly.amount));
            else if(assembly.name=="SRLI")
                temp_register[assembly.r1] = (temp_register[assembly.r2]>>int(assembly.amount));
            else if(assembly.name=="SRAI")
                temp_register[assembly.r1] = (temp_register[assembly.r2]<<int(assembly.amount));
            else if(assembly.name=="JALR"){
                pc = (temp_register[assembly.r2]+int(assembly.amount));
                temp_register[assembly.r1] = pc+4;
            }
            else if(assembly.name=="ECALL")
                ;
            else if(assembly.name=="EBREAK")
                ;       
            if(assembly.name!="JALR")
                pc+=4;
            break;
        case 'S':
            assembly = get_S_type(instruction);
            /*cout<<assembly.name<<" "<<assembly.r1<<" "<<assembly.r2<<" "<<assembly.amount<<endl;
            cout<<temp_register[assembly.r1]<<endl;
            cout<<temp_register[assembly.r2]<<endl;*/
            if(assembly.name=="SB"){
                uint8_t byte;
                memcpy((void*)&byte, (void*)&temp_register[assembly.r2], sizeof(uint8_t));
                temp_memory[temp_register[assembly.r1]+int(assembly.amount)] = byte;
            }
            else if(assembly.name=="SH"){
                uint16_t halfword;
                memcpy((void*)&halfword, (void*)&temp_register[assembly.r2], sizeof(uint16_t));
                temp_memory[temp_register[assembly.r1]+int(assembly.amount)] = halfword;
            }
            else if(assembly.name=="SW"){
                uint32_t word;
                memcpy((void*)&word, (void*)&temp_register[assembly.r2], sizeof(uint32_t));
                temp_memory[temp_register[assembly.r1]+int(assembly.amount)] = word;
            }
            pc+=4;
            break;
        case 'B':
            assembly = get_B_type(instruction);
            if(assembly.name=="BEQ")
                if(temp_register[assembly.r1]==temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
                else
                    pc += 4;
            else if(assembly.name=="BNE")
                if(temp_register[assembly.r1]!=temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
                else
                    pc += 4;
            else if(assembly.name=="BLT")
                if(temp_register[assembly.r1]<temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
                else
                    pc += 4;
            else if(assembly.name=="BGE")
                if(temp_register[assembly.r1]>=temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
                else
                    pc += 4;
            else if(assembly.name=="BLTU")
                if(temp_register[assembly.r1]<temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
                else
                    pc += 4;
            else if(assembly.name=="BGEU")
                if(temp_register[assembly.r1]>=temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
                else
                    pc += 4;
            break;
        case 'U':
            assembly = get_U_type(instruction);
            if(assembly.name=="LUI")
                temp_register[assembly.r1] = (assembly.amount<<12);
            else if(assembly.name=="AUIPC")
                temp_register[assembly.r1] = (assembly.amount<<12)+pc;
            pc+=4;
            break;
        case 'J':
            assembly = get_J_type(instruction);
            if(assembly.name=="JAL"){
                temp_register[assembly.r1] = pc+4;
                pc = pc + int(assembly.amount);
            }
            break;
        default:
            break;
        }
        temp_register[0]=0;
        cout<<assembly.name<<" "<<register_list[assembly.r1]<<" "<<register_list[assembly.r2]<<" "<<assembly.amount<<endl;
        cout<<"pc: "<<pc<<endl;
        show_register(temp_register);
        //show_memory(temp_memory);
        i++;
    }
}