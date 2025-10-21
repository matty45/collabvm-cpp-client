#include "vm.h"  
#include <QPainter>  
#include <QTextDocument>  
#include <QAbstractTextDocumentLayout>  
#include <QApplication>  

namespace cvm::delegates
{
    vm::vm(QObject* parent)
        : QStyledItemDelegate(parent)
    {
    }

    void vm::paint(QPainter* painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const
    {
        // Save painter state  
        painter->save();

        // Draw background and selection state  
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

        // Get the HTML text from Qt::DisplayRole  
        QString htmlText = index.data(Qt::DisplayRole).toString();

        // Get the thumbnail from Qt::DecorationRole  
        QPixmap thumbnail = index.data(Qt::DecorationRole).value<QPixmap>();

        // Calculate layout for IconMode with thumbnail on top, text below  
        QRect iconRect = opt.rect;
        QRect textRect = opt.rect;

        if (!thumbnail.isNull()) {
            // Draw thumbnail at top center  
            int iconHeight = thumbnail.height();
            iconRect.setHeight(iconHeight);
            iconRect.moveTop(opt.rect.top() + 10);

            QRect scaledIconRect = QStyle::alignedRect(
                opt.direction, Qt::AlignHCenter | Qt::AlignTop,
                thumbnail.size(), iconRect);

            painter->drawPixmap(scaledIconRect, thumbnail);

            // Position text below thumbnail  
            textRect.setTop(iconRect.bottom() + 10);
        }

        // Create QTextDocument for HTML rendering  
        QTextDocument doc;
        doc.setHtml(htmlText);
        doc.setTextWidth(textRect.width());
        doc.setDefaultFont(opt.font);
        
        // Translate painter to text area  
        painter->translate(textRect.topLeft());

        // Clip to text rect  
        QRect clipRect(0, 0, textRect.width(), textRect.height());
        painter->setClipRect(clipRect);

        // Draw the HTML text  
        QAbstractTextDocumentLayout::PaintContext ctx;

        // Apply selection colors if item is selected  
        if (opt.state & QStyle::State_Selected) {
            ctx.palette.setColor(QPalette::Text,
                opt.palette.color(QPalette::HighlightedText));
        }

        doc.documentLayout()->draw(painter, ctx);

        painter->restore();
    }

    QSize vm::sizeHint(const QStyleOptionViewItem& option,
        const QModelIndex& index) const
    {
        // Since uniformItemSizes is true, we need to return a consistent size  
        QString htmlText = index.data(Qt::DisplayRole).toString();
        QPixmap thumbnail = index.data(Qt::DecorationRole).value<QPixmap>();

        QTextDocument doc;
        doc.setHtml(htmlText);
        doc.setDefaultFont(option.font);
        doc.setTextWidth(option.rect.width() > 0 ? option.rect.width() : 200);

        int textHeight = static_cast<int>(doc.size().height());
        int thumbnailHeight = thumbnail.isNull() ? 0 : thumbnail.height();

        // Total height: thumbnail + spacing + text + padding  
        int totalHeight = thumbnailHeight + (thumbnailHeight > 0 ? 10 : 0) + textHeight + 20;

        // Width should accommodate thumbnail or text, whichever is wider  
        int totalWidth = qMax(thumbnail.width(), static_cast<int>(doc.idealWidth()));

        return {totalWidth, totalHeight};
    }
}