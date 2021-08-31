#ifndef EMULATOR_H
#define EMULATOR_H

#include <QString>
#include "elfreader.h"
#include "memorymapmodel.h"
#include "registermapmodel.h"

class Emulator
{
public:
    Emulator(QString filePath);
    QString nextInstruction();
    RegisterMapModel* getRegisterMapModel();
    MemoryMapModel* getMemoryMapModel();
    void updateRegisterMapModel();
    void updateMemoryMapModel();
    int getPC();
    int getEntry();
    bool isEnd();

private:
    RegisterMapModel *registerMap;
    MemoryMapModel *memoryMap;
    int entry;
    int pc;
    bool end;
};

#endif // EMULATOR_H
