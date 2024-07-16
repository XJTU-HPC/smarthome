#include "chatitemdelegate.h"
#include <QFontMetrics>
#include <QApplication>

ChatItemDelegate::ChatItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void ChatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    QRect rect = option.rect;

    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    QString text = index.data(Qt::DisplayRole).toString();

    // 获取填充色
    QColor fillColor = index.data(Qt::UserRole + 1).value<QColor>();
    if (!fillColor.isValid()) {
        // 如果没有设置填充色，使用默认值
        fillColor = Qt::white;
    }

    QSize iconSize(30, 30);
    QRect iconRect(rect.left() + 5,
                   rect.top() + (rect.height() - iconSize.height()) / 2,
                   iconSize.width(),
                   iconSize.height());

    QFont font = painter->font();
    font.setPointSize(14);
    painter->setFont(font);

    QFontMetrics fm(font);
    int maxTextWidth = rect.width() - iconRect.width() - 20;
    QRect textRect = fm.boundingRect(QRect(0, 0, maxTextWidth, 0), Qt::TextWordWrap, text);
    textRect.moveLeft(iconRect.right() + 5);
    textRect.moveTop(rect.top() + (rect.height() - textRect.height()) / 2);

    // 画填充色
    painter->setPen(Qt::NoPen);
    painter->setBrush(fillColor);
    painter->drawRoundedRect(rect, 10, 10);

    // 画边框
    QPen pen(Qt::lightGray);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(rect, 10, 10);

    icon.paint(painter, iconRect);

    // 设置字体颜色为黑色
    QPen textPen(Qt::black);
    painter->setPen(textPen);

    textRect = textRect.intersected(rect);
    painter->drawText(textRect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignVCenter, text);

    painter->restore();
}

QSize ChatItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString text = index.data(Qt::DisplayRole).toString();

    QFont font = option.font;
    font.setPointSize(14);

    QFontMetrics fm(font);
    int maxTextWidth = option.rect.width() - 30 - 20; // icon width + margins
    int textHeight = fm.boundingRect(QRect(0, 0, maxTextWidth, 0), Qt::TextWordWrap, text).height();

    int totalHeight = qMax(30, textHeight) + 10; // icon height + padding

    return QSize(option.rect.width(), totalHeight);
}
