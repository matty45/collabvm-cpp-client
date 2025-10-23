#pragma once
#include <qabstractitemmodel.h>
#include <qnamespace.h>

#include "src/cvm/cvm.h"

namespace cvm::models
{

    class user_list : public QAbstractListModel
    {
        Q_OBJECT

    public:

        enum user_roles : quint16 {
            rank_icon_role = Qt::UserRole + 1,
            country_icon_role = Qt::UserRole + 2
        };

        explicit user_list(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        user* user_at_index(const QModelIndex& index) const;

        Q_INVOKABLE void append(user* user);
        Q_INVOKABLE void remove(const user* marked_for_death);
        Q_INVOKABLE void clear();

    private:
        QList<user*> m_user_list;
    };

}
