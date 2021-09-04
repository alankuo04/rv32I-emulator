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
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addStartIndex();
    void subStartIndex();
    void setStartIndex(int index);

    uint32_t *memory;
public slots:
    void memoryMapChanged();

private:
    int startIndex;
};

#endif // MEMORYMAPMODEL_H
