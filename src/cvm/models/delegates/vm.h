#pragma once  
#include <QStyledItemDelegate>  
#include <QTextDocument>  

namespace cvm::delegates
{
    class vm : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        explicit vm(QObject* parent = nullptr);

        void paint(QPainter* painter,
            const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;

        QSize sizeHint(const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;
    };
}