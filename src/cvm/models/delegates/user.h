#pragma once  
#include <QStyledItemDelegate>  
#include <QTextDocument>  

namespace cvm::delegates
{
    class user : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        explicit user(QObject* parent = nullptr);

        void paint(QPainter* painter,
            const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;

        QSize sizeHint(const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;

    private:
        static QString get_rank_emoji(int rank);
        static QString get_country_emoji(const QString& countryCode);
    };
}