#include<iostream>
#include<string>
#include"elf_function.h"
#include"dis_function.h"
using namespace std;

void show_register(int* temp_register){
    for(int i=0;i<32;i++){
        cout<<register_list[i]<<" "<<temp_register[i]<<endl;
    }
}

int main(int argc, char **argv){
    Text_section *text = get_text_section(argv[1]);
    string next;
    int i=0;
    int pc=0;
    int temp_register[32]={};
    char temp_memory[2048*2048]={};
    while(getline(cin, next) && i<text->size){
        //cout<<text->text_section[i]<<endl;
        cout<<i<<endl;
        Instruction instruction = {text->text_section[i]};
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
            break;
        case 'I':
            assembly = get_I_type(instruction);
            if(assembly.name=="LB")
                temp_register[assembly.r1] = temp_memory[temp_register[assembly.r2]+int(assembly.amount)];
            else if(assembly.name=="LH")
                temp_register[assembly.r1] = temp_memory[temp_register[assembly.r2]+int(assembly.amount)];
            else if(assembly.name=="LW")
                temp_register[assembly.r1] = temp_memory[temp_register[assembly.r2]+int(assembly.amount)];
            else if(assembly.name=="LBU")
                temp_register[assembly.r1] = temp_memory[temp_register[assembly.r2]+int(assembly.amount)];
            else if(assembly.name=="LHU")
                temp_register[assembly.r1] = temp_memory[temp_register[assembly.r2]+int(assembly.amount)];
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
                temp_register[assembly.r1] = pc;
            }
            else if(assembly.name=="ECALL")
                ;
            else if(assembly.name=="EBREAK")
                ;
            break;
        case 'S':
            assembly = get_S_type(instruction);
            cout<<assembly.name<<" "<<assembly.r1<<" "<<assembly.r2<<" "<<assembly.amount<<endl;
            cout<<temp_register[assembly.r1]<<endl;
            cout<<temp_register[assembly.r2]<<endl;
            if(assembly.name=="SB")
                temp_memory[temp_register[assembly.r1]+int(assembly.amount)] = temp_register[assembly.r2];
            else if(assembly.name=="SH")
                temp_memory[temp_register[assembly.r1]+int(assembly.amount)] = temp_register[assembly.r2];
            else if(assembly.name=="SW")
                temp_memory[temp_register[assembly.r1]+int(assembly.amount)] = temp_register[assembly.r2];
            break;
        case 'B':
            assembly = get_B_type(instruction);
            if(assembly.name=="BEQ")
                if(temp_register[assembly.r1]==temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
            else if(assembly.name=="BNE")
                if(temp_register[assembly.r1]!=temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
            else if(assembly.name=="BLT")
                if(temp_register[assembly.r1]<temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
            else if(assembly.name=="BGE")
                if(temp_register[assembly.r1]>=temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
            else if(assembly.name=="BLTU")
                if(temp_register[assembly.r1]<temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
            else if(assembly.name=="BGEU")
                if(temp_register[assembly.r1]>=temp_register[assembly.r2])
                    pc = pc + int(assembly.amount);
            break;
        case 'U':
            assembly = get_U_type(instruction);
            if(assembly.name=="LUI")
                temp_register[assembly.r1] = (assembly.amount<<12);
            else if(assembly.name=="AUIPC")
                temp_register[assembly.r1] = (assembly.amount<<12)+pc;
            break;
        case 'J':
            assembly = get_J_type(instruction);
            if(assembly.name=="JAL"){
                temp_register[assembly.r1] = pc+4;
                pc = assembly.amount;
            }
            break;
        default:
            break;
        }
        cout<<assembly.name<<" "<<assembly.r1<<" "<<assembly.r2<<" "<<assembly.amount<<endl;
        cout<<"pc: "<<pc<<endl;
        pc+=4;
        show_register(temp_register);
        i++;
    }
}