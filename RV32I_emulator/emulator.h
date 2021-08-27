#ifndef EMULATOR_H
#define EMULATOR_H

#include <QString>
#include "elfreader.h"

#define MAX_MEMORY_SIZE 536870908

class Emulator
{
public:
    Emulator(QString filePath);
    QString nextInstruction();
    QList<uint32_t>* getRegisterList();
    QList<QList<uint32_t>>* getMemoryList();
    int getPC();
    bool isEnd();

private:
    uint32_t *memory;
    int *temp_register;
    int entry;
    int pc;
    bool end;
};

#endif // EMULATOR_H
