#ifndef CUSTOMTREEWIDGET_H
#define CUSTOMTREEWIDGET_H

#include <QObject>
#include <QTreeWidget>
#include <QMouseEvent>

class CustomTreeWidget : public QTreeWidget
{

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        QModelIndex index = currentIndex();
        // 仅允许双击第 2 列触发编辑
        if (index.column() == 1)
        {
            QTreeWidget::mouseDoubleClickEvent(event);
        }
        else
        {
            event->ignore(); // 忽略其他列的编辑触发
        }
    }
};

#endif // CUSTOMTREEWIDGET_H
