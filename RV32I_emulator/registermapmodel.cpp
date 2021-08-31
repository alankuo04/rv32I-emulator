#include "registermapmodel.h"

RegisterMapModel::RegisterMapModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    temp_register = new int[32];
    for(int i=0;i<32;i++)
        temp_register[i] = 0;
}

void RegisterMapModel::registerMapChanged()
{
    emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

int RegisterMapModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 32;
}

int RegisterMapModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

QVariant RegisterMapModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(index.column()==2 && index.row()==0)
        {
            return QString("0x")+QString("%1").arg(0, 8, 16, QLatin1Char('0')).toUpper();
        }
        else if(index.column()==0)
        {
            return QString("x%1").arg(index.row());
        }
        else if(index.column()==1)
        {
            return QString("%1").arg(register_list[index.row()]);
        }
        else if(index.column()==2)
        {
            return QString("0x")+QString("%1").arg((uint32_t)temp_register[index.row()], 8, 16, QLatin1Char('0')).toUpper();
        }
    }
    return QVariant();
}

QVariant RegisterMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return QString("Name");
        case 1:
            return QString("Alias");
        case 2:
            return QString("Value");
        }
    }
    return QVariant();
}

