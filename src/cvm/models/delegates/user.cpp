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
        QString username = index.data(Qt::DisplayRole).toString();
        int rank = index.data(models::user_list::rank_role).toInt();
        QString countryCode = index.data(models::user_list::country_role).toString();

        QString countryEmoji = get_country_emoji(countryCode);
        QString rankEmoji = get_rank_emoji(rank);

        QRect rect = option.rect;

        // Draw selection background  
        if (option.state & QStyle::State_Selected) {
            painter->save();
            painter->setBrush(option.palette.highlight());
            painter->setPen(Qt::NoPen);
            painter->drawRect(option.rect);
            painter->setPen(QPen(option.palette.highlightedText(), 0));
        }

        QFontMetrics fm(option.font);
        int emojiWidth = fm.horizontalAdvance("🏳️ "); // Approximate emoji width  
        int spacing = 8; // Spacing between elements  

        // Calculate positions  
        QRect countryRect = rect;
        countryRect.setWidth(emojiWidth);

        QRect rankRect = rect;
        rankRect.setLeft(rect.right() - emojiWidth);

        QRect usernameRect = rect;
        usernameRect.setLeft(countryRect.right() + spacing);
        usernameRect.setRight(rankRect.left() - spacing);

        // Draw country emoji (left)  
        if (!countryEmoji.isEmpty()) {
            painter->drawText(countryRect, Qt::AlignLeft | Qt::AlignVCenter, countryEmoji);
        }

        // Draw username (center)  
        painter->drawText(usernameRect, Qt::AlignCenter | Qt::AlignVCenter | Qt::TextSingleLine, username);

        // Draw rank emoji (right)  
        if (!rankEmoji.isEmpty()) {
            painter->drawText(rankRect, Qt::AlignRight | Qt::AlignVCenter, rankEmoji);
        }

        if (option.state & QStyle::State_Selected) {
            painter->restore();
        }
	}

	QSize user::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
        QString username = index.data(Qt::DisplayRole).toString();
        QFontMetrics fm(option.font);
        int emojiWidth = fm.horizontalAdvance("🏳️ ") * 2; // Space for both emojis  
        int textWidth = fm.horizontalAdvance(username);
        int spacing = 16;

        return QSize(textWidth + emojiWidth + spacing, fm.height() + 4);
	}

    QString user::get_rank_emoji(int rank)
    {
        switch (rank) {
        case 2: // admin  
            return "👑";
        case 3: // moderator  
            return "🛡️";
        default:
            return "";
        }
    }

    QString user::get_country_emoji(const QString& countryCode)
    {
        if (countryCode.length() != 2) {
            return "🌐"; // Default globe emoji  
        }

        // Convert country code to regional indicator symbols  
        // e.g., "US" -> 🇺🇸 (U+1F1FA U+1F1F8)  
        QString emoji;
        for (QChar c : countryCode.toUpper()) {
            if (c >= 'A' && c <= 'Z') {
                // Regional indicator symbol letter A = U+1F1E6  
                emoji += QChar(0xD83C); // High surrogate  
                emoji += QChar(0xDDE6 + (c.unicode() - 'A')); // Low surrogate  
            }
        }
        return emoji.isEmpty() ? "🌐" : emoji;
    }

}
