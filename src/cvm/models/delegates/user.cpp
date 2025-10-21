#include "user.h"

#include <QPainter>

#include "src/cvm/models/user_list.h"

namespace cvm::delegates
{

	user::user(QObject* parent)
	{
	}

	void user::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        // Reserve space for icons  
        const int iconSize = option.rect.height() - 4; // 2px padding on each side  
        const int iconSpacing = 4;

        // Modify the option to give us room for both icons  
        QStyleOptionViewItem opt = option;
        opt.rect.adjust(iconSize + iconSpacing, 0, -(iconSize + iconSpacing), 0);

        // Paint the standard item (text, background, selection, etc.)  
        QStyledItemDelegate::paint(painter, opt, index);

        painter->save();

        // Paint country icon on the left  
        QVariant countryData = index.data(models::user_list::country_icon_role);
        if (countryData.isValid()) {
            QIcon countryIcon = qvariant_cast<QIcon>(countryData);
            if (!countryIcon.isNull()) {
                QRect countryRect(option.rect.x() + 2,
                    option.rect.y() + 2,
                    iconSize,
                    iconSize);
                countryIcon.paint(painter, countryRect, Qt::AlignCenter);
            }
        }

        // Paint rank icon on the right  
        QVariant rankData = index.data(models::user_list::rank_icon_role);
        if (rankData.isValid()) {
            QIcon rankIcon = qvariant_cast<QIcon>(rankData);
            if (!rankIcon.isNull()) {
                QRect rankRect(option.rect.x() + option.rect.width() - iconSize - 2,
                    option.rect.y() + 2,
                    iconSize,
                    iconSize);
                rankIcon.paint(painter, rankRect, Qt::AlignCenter);
            }
        }

        painter->restore();
    }

    QSize user::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        // Get the base size from the parent  
        QSize size = QStyledItemDelegate::sizeHint(option, index);

        // Add extra width for the two icons plus spacing  
        const int iconSize = size.height() - 4;
        const int iconSpacing = 4;
        size.setWidth(size.width() + 2 * (iconSize + iconSpacing));

        return size;
    }

}
