#include "memorymapmodel.h"

MemoryMapModel::MemoryMapModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    memory = new uint32_t[MAX_MEMORY_SIZE];
    startIndex = 0;
}

void MemoryMapModel::memoryMapChanged()
{
    emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

int MemoryMapModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 32;
}

int MemoryMapModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 6;
}

QVariant MemoryMapModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = startIndex+(rowCount()-index.row()+1);
        if(index.column()==0)
        {
            return QString("0x")+QString("%1").arg(row*4, 8, 16, QLatin1Char('0')).toUpper();
        }
        else if(index.column()==1)
        {
            return QString("0x")+QString("%1").arg(memory[row], 8, 16, QLatin1Char('0')).toUpper();
        }
        else if(index.column()==2)
        {
            return QString("0x")+QString("%1").arg((memory[row]>>24) & 0xFF, 2, 16, QLatin1Char('0')).toUpper();
        }
        else if(index.column()==3)
        {
            return QString("0x")+QString("%1").arg((memory[row]>>16) & 0xFF, 2, 16, QLatin1Char('0')).toUpper();
        }
        else if(index.column()==4)
        {
            return QString("0x")+QString("%1").arg((memory[row]>>8) & 0xFF, 2, 16, QLatin1Char('0')).toUpper();
        }
        else if(index.column()==5)
        {
            return QString("0x")+QString("%1").arg(memory[row] & 0xFF, 2, 16, QLatin1Char('0')).toUpper();
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant MemoryMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return QString("Address");
        case 1:
            return QString("Word");
        case 2:
            return QString("Byte3");
        case 3:
            return QString("Byte2");
        case 4:
            return QString("Byte1");
        case 5:
            return QString("Byte0");
        }
    }
    return QVariant();
}

void MemoryMapModel::addStartIndex()
{
    if(startIndex <= MAX_MEMORY_SIZE-32)
        startIndex += 2;
}

void MemoryMapModel::subStartIndex()
{
    if(startIndex >= 0)
        startIndex -= 2;
}

void MemoryMapModel::setStartIndex(int index)
{
    if(startIndex <= MAX_MEMORY_SIZE && startIndex >= 0)
    {
        startIndex = (index/4)>MAX_MEMORY_SIZE-32?MAX_MEMORY_SIZE-32:index/4;
    }
}
