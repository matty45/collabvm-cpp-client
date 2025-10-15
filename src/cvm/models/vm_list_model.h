#pragma once
#include <qabstractitemmodel.h>
#include <qnamespace.h>

#include "src/cvm/cvm.h"


class vm_list_model : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit vm_list_model(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    cvm::vm vm(const QModelIndex& index) const;

//public Q_SLOTS:
//    void populate();

private:
    QList<cvm::vm> vm_list;
};
