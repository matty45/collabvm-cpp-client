#pragma once
#include <qabstractitemmodel.h>
#include <qnamespace.h>

namespace cvm::models
{

    class chat_message_list : public QAbstractListModel
    {
        Q_OBJECT

    public:

        enum user_roles : quint16 {
            username_role = Qt::UserRole + 1,
            message_role = Qt::UserRole + 2
        };

        explicit chat_message_list(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        chat_message* message_at_index(const QModelIndex& index) const;

        Q_INVOKABLE void append(chat_message* message);
        Q_INVOKABLE void remove_row(int row);
        Q_INVOKABLE void clear();

    private:
        QList<chat_message*> m_chat_message_qlist; //Probably not the best variable type to store these.
    };

}
