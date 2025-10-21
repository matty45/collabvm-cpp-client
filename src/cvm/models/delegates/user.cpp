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
        painter->save();

        // Draw background and selection highlight  
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
        }

        const int margin = 4;
        const int iconSize = 16; // or use option.decorationSize.width()  

        // Get data from model  
        QIcon countryIcon = qvariant_cast<QIcon>(index.data(cvm::models::user_list::country_icon_role));
        QIcon rankIcon = qvariant_cast<QIcon>(index.data(cvm::models::user_list::rank_icon_role));
        QString username = index.data(Qt::DisplayRole).toString();

        int x = option.rect.left() + margin;
        int y = option.rect.top();
        int height = option.rect.height();

        // Draw country icon on the left  
        if (!countryIcon.isNull()) {
            QRect countryRect(x, y + (height - iconSize) / 2, iconSize, iconSize);
            countryIcon.paint(painter, countryRect);
            x += iconSize + margin;
        }

        // Calculate text area (center)  
        int textWidth = option.rect.right() - x - iconSize - margin * 2;
        QRect textRect(x, y, textWidth, height);

        // Draw username text  
        QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled)
            ? QPalette::Normal : QPalette::Disabled;
        if (option.state & QStyle::State_Selected) {
            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
        }
        else {
            painter->setPen(option.palette.color(cg, QPalette::Text));
        }
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, username);

        // Draw rank icon on the right  
        if (!rankIcon.isNull()) {
            QRect rankRect(option.rect.right() - iconSize - margin,
                y + (height - iconSize) / 2, iconSize, iconSize);
            rankIcon.paint(painter, rankRect);
        }

        painter->restore();
    }

	QSize user::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        const int margin = 4;
        const int iconSize = 16;

        // Get username text  
        QString username = index.data(Qt::DisplayRole).toString();
        QFontMetrics fm(option.font);
        int textWidth = fm.horizontalAdvance(username);

        // Calculate total width: left icon + margin + text + margin + right icon + margins  
        int width = iconSize + margin + textWidth + margin + iconSize + margin * 2;
        int height = qMax(iconSize, fm.height()) + margin * 2;

        return QSize(width, height);
    }

}
