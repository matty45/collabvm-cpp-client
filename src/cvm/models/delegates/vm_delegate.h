#pragma once  
#include <QStyledItemDelegate>  
#include <QTextDocument>  

namespace cvm::delegates
{
    class vm_delegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        explicit vm_delegate(QObject* parent = nullptr);

        void paint(QPainter* painter,
            const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;

        QSize sizeHint(const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;
    };
}