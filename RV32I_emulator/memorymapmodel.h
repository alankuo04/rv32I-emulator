#ifndef MEMORYMAPMODEL_H
#define MEMORYMAPMODEL_H

#include <QAbstractTableModel>

class MemoryMapModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MemoryMapModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // MEMORYMAPMODEL_H
