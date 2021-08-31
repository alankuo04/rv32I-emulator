#ifndef MEMORYMAPMODEL_H
#define MEMORYMAPMODEL_H

#include <QAbstractTableModel>

#define MAX_MEMORY_SIZE 536870908

class MemoryMapModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MemoryMapModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    uint32_t *memory;
public slots:
    void memoryMapChanged();
};

#endif // MEMORYMAPMODEL_H
