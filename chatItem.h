#ifndef CHATITEM_H
#define CHATITEM_H
#include <QApplication>
#include <QListView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QIcon>

class ChatItemDelegate : public QStyledItemDelegate {
public:
    ChatItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        painter->save();

        QRect rect = option.rect;
        // 设置圆角矩形的边框颜色和绘制
        QPen pen(Qt::lightGray);  // 浅灰色
        pen.setWidth(2);  // 设置边框宽度（你可以调整为所需的宽度）
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);  // 无填充颜色
        painter->drawRoundedRect(rect, 10, 10);  // 绘制圆角矩形，圆角半径为10

        QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
        QString text = index.data(Qt::DisplayRole).toString();

        QRect iconRect = rect;
        iconRect.setSize(QSize(30, 30));  // Assume a fixed size for icons
        iconRect.moveLeft(rect.left() + 5);
        iconRect.moveTop(rect.top() + (rect.height() - iconRect.height()) / 2);  // 垂直居中

        QRect textRect = rect;
        textRect.setLeft(iconRect.right() + 5);
        // 设置文本字体及其大小
        QFont font = painter->font();
        font.setPointSize(14);  // 设置字体大小，例如12
        painter->setFont(font);

        icon.paint(painter, iconRect);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

        painter->restore();
    }
};

#endif // CHATITEM_H
