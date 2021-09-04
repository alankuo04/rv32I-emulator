#ifndef REGISTERMAPMODEL_H
#define REGISTERMAPMODEL_H

#include <QAbstractTableModel>
#include "instruction.h"

class RegisterMapModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RegisterMapModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int* temp_register;

public slots:
    void registerMapChanged();
};

#endif // REGISTERMAPMODEL_H
