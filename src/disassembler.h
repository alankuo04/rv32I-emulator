#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include<string>
#include<QString>

class Disassembler
{
public:
    Disassembler(uint32_t binary_code);
    QString getInstruction();
private:
    QString str;
};

#endif // DISASSEMBLER_H
