#include "my_lineedit.h"
#include <QDebug>

My_lineEdit::My_lineEdit(QWidget *parent) :
    QLineEdit(parent)
{

}

void My_lineEdit::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit send_show(this->text());

}
