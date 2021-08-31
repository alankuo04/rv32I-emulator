#include "memorymapmodel.h"

MemoryMapModel::MemoryMapModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    memory = new uint32_t[MAX_MEMORY_SIZE];
}

void MemoryMapModel::memoryMapChanged()
{
    emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

int MemoryMapModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 16;
}

int MemoryMapModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant MemoryMapModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(index.column()==0)
        {
            return QString("0x")+QString("%1").arg(0+index.row()*4, 8, 16, QLatin1Char('0')).toUpper();
        }
        else if(index.column()==1)
        {
            return QString("0x")+QString("%1").arg(memory[index.row()], 8, 16, QLatin1Char('0')).toUpper();
        }
    }
    return QVariant();
}
