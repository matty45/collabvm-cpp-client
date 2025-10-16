#pragma once
#include <qabstractitemmodel.h>
#include <qnamespace.h>

#include "src/cvm/cvm.h"

namespace cvm::models
{

    class vm_list : public QAbstractListModel
    {
        Q_OBJECT

    public:
        explicit vm_list(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        cvm::vm vm(const QModelIndex& index) const;

        Q_INVOKABLE void append(const QString& id, const QString& display_name, const QString& thumbnail);
        Q_INVOKABLE void remove(int row);

    private:
        QList<cvm::vm> m_vm_list;
    };

}
