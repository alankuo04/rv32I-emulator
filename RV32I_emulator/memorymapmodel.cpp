#include "memorymapmodel.h"

#define MAX_MEMORY_SIZE 536870908

MemoryMapModel::MemoryMapModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int MemoryMapModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 128;
}

int MemoryMapModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QVariant MemoryMapModel::data(const QModelIndex &index, int role) const
{
    /*if (role == Qt::DisplayRole)
       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);
    */
    return QVariant();
}
