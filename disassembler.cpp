#include<iostream>
#include<string>
#include"elf_function.h"
#include"dis.h"
using namespace std;

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