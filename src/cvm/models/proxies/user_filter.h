#pragma once
#include <QSortFilterProxyModel>


namespace cvm::models::proxies
{
    class user_filter : public QSortFilterProxyModel
    {
        Q_OBJECT

    public:
        user_filter(QObject* parent = nullptr);
        void set_filter_server(server* server);

    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

    private:
        server* m_filter_server;
    };
}