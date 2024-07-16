#ifndef CHATITEMDELEGATE_H
#define CHATITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class ChatItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit ChatItemDelegate(QObject *parent = nullptr);

    // Override paint method
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // Override sizeHint method
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CHATITEMDELEGATE_H
