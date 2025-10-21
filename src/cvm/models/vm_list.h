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

        vm vm_at_index(const QModelIndex& index) const;

        Q_INVOKABLE void append(const QString& id, const QString& display_name, const QString& thumbnail, const QUrl& server);
        Q_INVOKABLE void remove(int row);
        Q_INVOKABLE void clear();

    private:
        QList<vm> m_vm_list;
    };

}
