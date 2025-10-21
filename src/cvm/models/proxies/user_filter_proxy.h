#pragma once
#include <QSortFilterProxyModel>


namespace cvm::models::proxies
{
    class user_filter_proxy : public QSortFilterProxyModel
    {
        Q_OBJECT

    public:
        user_filter_proxy(QObject* parent = nullptr);
        void set_filter_server(const QUrl& server);

    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

    private:
        QUrl m_filter_server;
    };
}