#ifndef MYSHOWQTREEWIDGET_H
#define MYSHOWQTREEWIDGET_H
#include <QApplication>
#include <QTreeWidget>
#include <QHeaderView>
#include <QMenu>
#include <QClipboard>
#include <QMimeData>
#include <QVector>
#include <QMessageBox>
#include <QInputDialog>

class MyShowTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    MyShowTreeWidget(QWidget *parent = nullptr) : QTreeWidget(parent)
    {
        // 设置基本属性
        setColumnCount(2);
        //setHeaderLabels(QStringList() << "名称" << "值");  //不需要
        setSelectionMode(QAbstractItemView::ExtendedSelection);

        // 启用自定义上下文菜单
       // setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &MyShowTreeWidget::customContextMenuRequested,
                this, &MyShowTreeWidget::showContextMenu);

        // 填充示例数据
        //initSampleData();  //不需要
    }

private slots:
    void showContextMenu(const QPoint &pos)
    {
        QMenu menu(this);

        // 添加操作
        QAction *addTopLevelAction = menu.addAction("Add top-Level node");
        connect(addTopLevelAction, &QAction::triggered, this, &MyShowTreeWidget::addTopItem);

        QAction *addChildAction = menu.addAction("Add child node");
        addChildAction->setEnabled(currentItem() != nullptr); // 只有选中项时才启用
        connect(addChildAction, &QAction::triggered, this, &MyShowTreeWidget::addChildItem);

        menu.addSeparator();


        QAction *copyAction = menu.addAction("Copy");
        copyAction->setEnabled(!selectedItems().isEmpty());
        connect(copyAction, &QAction::triggered, this, &MyShowTreeWidget::copyItems);

        QAction *pasteAction = menu.addAction("Paste");
        pasteAction->setEnabled(!copiedItems.isEmpty());
        connect(pasteAction, &QAction::triggered, this, &MyShowTreeWidget::pasteItems);


        // 添加删除操作
        QAction *deleteAction = menu.addAction("Delete");
        deleteAction->setEnabled(!selectedItems().isEmpty());
        connect(deleteAction, &QAction::triggered, this, &MyShowTreeWidget::deleteItems);

        menu.exec(viewport()->mapToGlobal(pos));
    }

    // 添加顶级节点
    void addTopItem()
    {
        bool ok;
        QString name = QInputDialog::getText(this, "Add top-Level node",
                                           "Input Node Name:",
                                           QLineEdit::Normal,
                                           "New Node",
                                           &ok);
        if (!ok || name.isEmpty()) return;

        QString value = QInputDialog::getText(this, "Add top-Level node",
                                             "Input Node Value:",
                                             QLineEdit::Normal,
                                             "",
                                             &ok);
        if (!ok) return;

        // 创建新节点
        QTreeWidgetItem *newItem = new QTreeWidgetItem(this);
        newItem->setText(0, name);
        newItem->setText(1, value);

        // 自动展开并选中新节点
        setCurrentItem(newItem);
    }


    //
     void addChildItem()
     {
         QTreeWidgetItem *parent = currentItem();
         if (!parent) return;

         bool ok;
         QString name = QInputDialog::getText(this, "Add child Node",
                                            "Input Child Node Name:",
                                            QLineEdit::Normal,
                                            "Child Name",
                                            &ok);
         if (!ok || name.isEmpty()) return;

         QString value = QInputDialog::getText(this, "Add child Node",
                                             "Input Child Node Value:",
                                             QLineEdit::Normal,
                                             "",
                                             &ok);
         if (!ok) return;

         // 创建新子节点
         QTreeWidgetItem *child = new QTreeWidgetItem(parent);
         child->setText(0, name);
         child->setText(1, value);

         // 展开父节点并选中新子节点
         parent->setExpanded(true);
         setCurrentItem(child);
     }

    void copyItems()
    {
        copiedItems.clear();
        const QList<QTreeWidgetItem*> selected = selectedItems();

        for (QTreeWidgetItem *item : selected)
        {
            // 深度复制项目（包括子项目）
            copiedItems.append(copyItemWithChildren(item));
        }
    }

    void deleteItems()
    {
        // 获取所有选中的项目
        QList<QTreeWidgetItem*> selected = selectedItems();
        if (selected.isEmpty()) return;

        // 确认对话框
        int result = QMessageBox::question(
            this,
            "确认删除",
            QString("确定要删除选中的 %1 个项目吗？").arg(selected.size()),
            QMessageBox::Yes | QMessageBox::No
        );

        if (result != QMessageBox::Yes) return;

         // 删除选中的项目（注意：删除父项目会自动删除其子项目）
        for (QTreeWidgetItem *item : selected)
        {
             // 确保项目未被删除（可能在删除父项时已被删除）
            if (item->parent())
            {
               // 从父项中移除
               item->parent()->removeChild(item);
            }
            else
            {
                // 从树中移除顶级项
                int index = indexOfTopLevelItem(item);
                if (index >= 0)
                {
                    takeTopLevelItem(index);
                }
            }

            // 安全删除项目及其所有子项
            delete item;
        }

    }


    void pasteItems()
    {
        if (copiedItems.isEmpty()) return;

        QTreeWidgetItem *target = currentItem();

        for (QTreeWidgetItem *original : copiedItems)
        {
            // 深度复制项目（创建新实例）
            QTreeWidgetItem *newItem = copyItemWithChildren(original);

            if (!target)
            {
                // 粘贴到根节点
                addTopLevelItem(newItem);
            }
            else if (target->isSelected())
            {
                // 粘贴为兄弟节点
                target->parent() ? target->parent()->addChild(newItem) : addTopLevelItem(newItem);
            }
            else
            {
                // 粘贴为子节点
                target->addChild(newItem);
                target->setExpanded(true);
            }
        }
    }

private:
    // 存储复制的项目（深度复制）
    QVector<QTreeWidgetItem*> copiedItems;

    void initSampleData()
    {
        for (int i = 0; i < 3; ++i)
        {
            QTreeWidgetItem *parent = new QTreeWidgetItem(this, QStringList() << QString("父节点 %1").arg(i+1));
            for (int j = 0; j < 2; ++j)
            {
                new QTreeWidgetItem(parent, QStringList()
                    << QString("子节点 %1").arg(j+1)
                    << QString("值 %1").arg(j+1));
            }
            parent->setExpanded(true);
        }
    }

    // 深度复制项目及其子项目
    QTreeWidgetItem* copyItemWithChildren(QTreeWidgetItem *item)
    {
        // 复制当前项目
        QTreeWidgetItem *newItem = new QTreeWidgetItem();
        for (int i = 0; i < item->columnCount(); ++i)
        {
            newItem->setText(i, item->text(i));
            newItem->setFlags(item->flags());
        }

        // 递归复制子项目
        for (int i = 0; i < item->childCount(); ++i)
        {
            newItem->addChild(copyItemWithChildren(item->child(i)));
        }

        return newItem;
    }
};

#endif // MYSHOWQTREEWIDGET_H
