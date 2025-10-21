#include "user_filter.h"

#include "src/cvm/models/user_list.h"

namespace cvm::models::proxies
{
    user_filter::user_filter(QObject* parent)
    {
    }

    void user_filter::set_filter_server(const QUrl& server)
    {
        m_filter_server = server;
        invalidate(); // Trigger re-filtering  
    }

    bool user_filter::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
    {
        if (m_filter_server.isEmpty())
            return true; // No filter set, accept all rows  

        // Get the source model index for this row  
        QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);

        // Get the user_list model and retrieve the user  
        auto* userListModel = qobject_cast<user_list*>(sourceModel());
        if (!userListModel)
            return false;

        user u = userListModel->user_at_index(sourceIndex);

        qDebug() << "ass" << u.m_server.url() << "aaa" << m_filter_server.url();

        // Filter by comparing m_server  
        return u.m_server == m_filter_server;
    }
}