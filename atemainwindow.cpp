#include "atemainwindow.h"
#include "ui_atemainwindow.h"

ATEMainWindow::ATEMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ATEMainWindow)
{
    ui->setupUi(this);

    InitUI();

    // load xml 文件


}

ATEMainWindow::~ATEMainWindow()
{
    delete ui;
}

void ATEMainWindow::InitUI()
{
    //

    // 使用自定义的 CustomTreeWidget
     m_customtreeWidget = new CustomTreeWidget();

     m_ParametersTable = new QTableWidget();

    // 获取主窗口的菜单栏
    QMenuBar *menuBar = this->menuBar();

    // 1. 创建“文件”菜单
    QMenu *fileMenu = menuBar->addMenu("文件(&F)");

    // 添加“新建”动作
    QAction *newAction = new QAction("新建(&N)", this);
    newAction->setShortcut(QKeySequence::New); // 快捷键 Ctrl+N
    fileMenu->addAction(newAction);

    //打开文件
    QAction *openAction = new QAction("打开文件", this);
    //openAction->setShortcut(QKeySequence::NewOpen) // 快捷键 Ctrl+N
    fileMenu->addAction(openAction);

    // 连接“打开”动作的触发信号到关闭窗口的槽
    connect(openAction, &QAction::triggered, this, &ATEMainWindow::OpenFile);

    //添加save 动作
    QAction *saveAction = new QAction("保存", this);
    saveAction->setShortcut(QKeySequence::Save); // 快捷键 Ctrl+Q
    fileMenu->addAction(saveAction);

    // 连接“save”动作的触发信号到关闭窗口的槽
    connect(saveAction, &QAction::triggered, this, &ATEMainWindow::SaveItem);

    // 添加“退出”动作
    QAction *exitAction = new QAction("退出(&Q)", this);
    exitAction->setShortcut(QKeySequence::Quit); // 快捷键 Ctrl+Q
    fileMenu->addAction(exitAction);

    // 连接“退出”动作的触发信号到关闭窗口的槽
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);


    // 创建工具栏
    QToolBar *toolbar = addToolBar("主工具栏");

    // 添加“新建”动作（与菜单栏共享）
    toolbar->addAction(newAction);

    // 添加分隔符
    toolbar->addSeparator();
    toolbar->addAction(openAction);

    // 添加分隔符
    toolbar->addSeparator();
    toolbar->addAction(saveAction);

    // 添加分隔符
    toolbar->addSeparator();

    // 添加“退出”动作（与菜单栏共享）
    toolbar->addAction(exitAction);

    // 连接动作信号
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QTabWidget *MtabWidget = new QTabWidget(this);
    setCentralWidget(MtabWidget); // 将 QTabWidget 设为主窗口的中心部件

    // 添加第一个标签页：表格
    QWidget *tableTab = new QWidget();
    MtabWidget->addTab(tableTab, "Tree Test View"); // 标签标题为“数据表”


    QVBoxLayout *mainLayout = new QVBoxLayout(tableTab);

    // 添加水平布局到主布局
    QHBoxLayout *hLayout1 = new QHBoxLayout;

    //

    QSplitter *splitter = new QSplitter(Qt::Horizontal, tableTab);

    hLayout1->addWidget(splitter);

    mainLayout->addLayout(hLayout1); // 添加水平布局1

    // 左侧树控件
    treeTestViewWidget = new MyShowTreeWidget();
    treeTestViewWidget->setHeaderLabel("Script");
    QTreeWidgetItem *root = new QTreeWidgetItem(treeTestViewWidget);
    root->setText(0, "xxt");
    treeTestViewWidget->expandAll();

    // 配置表头允许自由调整大小
    treeTestViewWidget->header()->setSectionsMovable(false);
    treeTestViewWidget->header()->setSectionResizeMode(QHeaderView::Interactive); // 允许手动调整
    treeTestViewWidget->header()->setStretchLastSection(true); // 最后一列自动拉伸

    // 关键步骤：启用自定义上下文菜单
    treeTestViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(treeTestViewWidget, &QTreeWidget::customContextMenuRequested,
//                this, &ATEMainWindow::onCustomContextMenuRequested);
    // 右侧 TabWidget

    // 连接信号
    connect(treeTestViewWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(onTreeItemClicked(QTreeWidgetItem*, int)));


    QTabWidget *tabWidget = new QTabWidget();

//    QTreeWidget *treeRegistryWidget = new QTreeWidget();
//    treeRegistryWidget->setHeaderLabel("Registry");
//    QTreeWidgetItem *Registryroot = new QTreeWidgetItem(treeRegistryWidget);
//    Registryroot->setText(0, "This PC");
//    treeRegistryWidget->expandAll();

//    tabWidget->addTab(treeRegistryWidget, "Tree Registry");

    m_customtreeWidget->setHeaderLabel("Properties");
    m_customtreeWidget->setHeaderHidden(true);
    m_customtreeWidget->setColumnCount(2); // 关键：设置为 2 列
    m_customtreeWidget->setColumnWidth(0,300);

    // 添加父节点（可折叠）
    QTreeWidgetItem *rootItem1 = new QTreeWidgetItem(m_customtreeWidget);
    rootItem1->setText(0, "Attributes");

//    QTreeWidgetItem *child11 = new QTreeWidgetItem(rootItem1);
//    child11->setFlags(child11->flags() | Qt::ItemIsEditable); // 必须允许编辑
//    child11->setText(0, "BranchOnError");
//    child11->setText(1, "50");
//    QComboBox *comboBox = new QComboBox();
//    comboBox->addItems({"true", "false"});
//    m_customtreeWidget->setItemWidget(child12,1,comboBox);


//    QTreeWidgetItem *rootItem2 = new QTreeWidgetItem(m_customtreeWidget);
//    rootItem2->setText(0, "Debug");

//    QTreeWidgetItem *child201 = new QTreeWidgetItem(rootItem2);
//    child201->setFlags(child201->flags() | Qt::ItemIsEditable); // 必须允许编辑
//    child201->setText(0, "BreakPoint");



    m_customtreeWidget->expandAll();

    tabWidget->addTab(m_customtreeWidget, "ProPerties");

    tabWidget->addTab(m_ParametersTable,"Parameters");

    //
    // 设置水平表头标签

    m_ParametersTable->setRowCount(4);
    m_ParametersTable->setColumnCount(8);

    QStringList headlist;
    headlist<<"Parameter Name"<<"Value"<<"Unit"<<"Description"<<"Data Logging"<<"GV Set"<<"GV Get"<<"";
    m_ParametersTable->setHorizontalHeaderLabels(headlist);


    // 将子部件添加到分割器
    splitter->addWidget(treeTestViewWidget);
    splitter->addWidget(tabWidget);

    // 设置拉伸因子（左:右 = 1:3）
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    // 设置初始尺寸比例
    //splitter->setSizes({200, 600});

    // 确保分割器是窗口的中央部件
    //setCentralWidget(splitter);     

}


// 自定义槽函数：显示右键菜单
void ATEMainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    // 获取当前点击的项
    QTreeWidgetItem *item = treeTestViewWidget->itemAt(pos);
    if (!item) return; // 如果点在空白处，不显示菜单

    // 创建菜单
    QMenu menu(treeTestViewWidget);

    // 添加动作（可根据item类型动态添加）
    QAction *action1 = menu.addAction("Cut");
    QAction *action2 = menu.addAction("Copy");
    QAction *action3 = menu.addAction("Paste");

    // 连接动作的触发信号
    connect(action1, &QAction::triggered, [=](){ qDebug() << "Cut:" << item->text(0); });
    connect(action2, &QAction::triggered, [=](){
        qDebug() << "Copy:" << item->text(0);
    });
    connect(action3, &QAction::triggered, [=](){ qDebug() << ":" << item->text(0); });

    // 显示菜单（使用全局坐标）
    menu.exec(treeTestViewWidget->viewport()->mapToGlobal(pos));
}



// 递归解析 XML 节点

void ATEMainWindow::OpenFile()
{
    //
    // 打开文件选择对话框
    QString filePath = QFileDialog::getOpenFileName
    (
        this,                // 父窗口
        tr("选择文件"),       // 对话框标题
        //QDir::homePath(),    // 默认打开目录（这里用用户主目录）
        m_selectedFilePath,
        tr("所有文件 (*.*)")  // 文件过滤器
    );

    // 检查用户是否选择了文件（取消选择时返回空字符串）
    if (!filePath.isEmpty())
    {
        // 保存文件路径（此处以成员变量为例）
        m_selectedFilePath = filePath;

        // 打印路径（可选）
        qDebug() << "已选择文件：" << filePath;

        // 这里可以添加其他处理逻辑...

    }
    else
    {
        qDebug() << "用户取消选择";
        return;
    }

    //打开xml文件，显示在界面
    populateTreeFromXml(treeTestViewWidget,m_selectedFilePath);
}

void ATEMainWindow::SaveItem()
{
    //
    if(nullptr==treeTestViewWidget->currentItem())
    {
        qInfo()<<"没有选择左边树结构";
        return;
    }
    else
    {
        qInfo()<<treeTestViewWidget->currentItem()->text(0);
    }

    //判断右边是否为空
    if(0==m_customtreeWidget->topLevelItemCount())
    {
        qInfo()<<"右边树结构没有值";
        return;
    }
    else
    {
        qInfo()<<m_customtreeWidget->topLevelItem(0)->text(0);
    }

    //修改name的值
    qInfo()<<m_customtreeWidget->topLevelItem(1)->child(0)->text(1);
    if(m_customtreeWidget->topLevelItem(1)->child(0)->text(1).isEmpty())
    {
        qInfo()<<"Name不能为空";
        return;
    }
    treeTestViewWidget->currentItem()->setText(0,m_customtreeWidget->topLevelItem(1)->child(0)->text(1));

    //修改Value的
    //判断左边树结构是否有Value的子节点，如果有则修改，如果没有则不需要修改
    for (int i = 0; i < treeTestViewWidget->currentItem()->childCount(); ++i)
    {
        //
        QTreeWidgetItem* childItem = treeTestViewWidget->currentItem()->child(i);

        if("Value"==childItem->text(0))
        {
            //修改子节点的值
            childItem->child(0)->setText(0,m_customtreeWidget->topLevelItem(1)->child(1)->text(1));
        }

    }

    //修改属性值
    //右边属性大循环
    for(int i=0;i<m_customtreeWidget->topLevelItem(0)->childCount();i++)
    {
        QTreeWidgetItem* childItem = m_customtreeWidget->topLevelItem(0)->child(i);

        qInfo()<<"attributes"<<childItem->text(0);

        //左边属性大循环，查找相同名字 0代表attributes
        for(int j=0;j<treeTestViewWidget->currentItem()->child(0)->childCount();j++)
        {
            if(childItem->text(0)==treeTestViewWidget->currentItem()->child(0)->child(j)->text(0))
            {
                //把值赋值
                treeTestViewWidget->currentItem()->child(0)->child(j)->setText(1,childItem->text(1));
            }
        }
    }
}

void ATEMainWindow::populateTreeFromXml(QTreeWidget* treeWidget, const QString& filePath)
{
    // 清除现有内容
    treeWidget->clear();

    // 打开XML文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
        return;
    }

    // 解析XML
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        qDebug() << "Invalid XML";
        return;
    }
    file.close();

    // 隐藏表头
    //treeWidget->setHeaderHidden(true);

    // 配置表头允许自由调整大小
    treeWidget->header()->setSectionsMovable(false);
    treeWidget->header()->setSectionResizeMode(QHeaderView::Interactive); // 允许手动调整
    treeWidget->header()->setStretchLastSection(true); // 最后一列自动拉伸

    // 获取根元素
    QDomElement root = doc.documentElement();
    if (root.isNull()) return;

    // 创建根节点（不显示根标签）
    QTreeWidgetItem* invisibleRoot = treeWidget->invisibleRootItem();

    // 递归添加子节点
    //parseElement(root, invisibleRoot);
    processElement(invisibleRoot,root);

    treeWidget->expandAll();
}

void ATEMainWindow::parseElement(const QDomElement& element, QTreeWidgetItem* parentItem)
{
    // 创建当前元素节点
    QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
    QString displayText = element.tagName();

    // 添加属性（如果有）
    if (element.hasAttributes())
    {
        QDomNamedNodeMap attrs = element.attributes();
        for (int i = 0; i < attrs.count(); ++i)
        {
            QDomAttr attr = attrs.item(i).toAttr();
            displayText += " " + attr.name() + "=\"" + attr.value() + "\"";
        }
    }

    item->setText(0, displayText);

    // 处理子节点
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        if (child.isElement())
        {
            // 递归处理元素节点
            parseElement(child.toElement(), item);
        } else if (child.isText())
        {
            // 添加文本内容
            QTreeWidgetItem* textItem = new QTreeWidgetItem(item);
            textItem->setText(0, child.toText().data().trimmed());
        }
        child = child.nextSibling();
    }
}

void ATEMainWindow::processElement(QTreeWidgetItem *parent, const QDomElement &element)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
       //item->setIcon(0, QIcon(":/icons/element.png"));
       item->setText(0, element.tagName());

       // 处理属性 - 创建分组节点
       //if (element.hasAttributes())
       {
           QTreeWidgetItem *attrGroup = new QTreeWidgetItem(item);
           //attrGroup->setIcon(0, QIcon(":/icons/attributes.png"));
           attrGroup->setText(0, "Attributes");

           QDomNamedNodeMap attrMap = element.attributes();
           for (int i = 0; i < attrMap.count(); ++i) {
               QDomAttr attr = attrMap.item(i).toAttr();
               QTreeWidgetItem *attrItem = new QTreeWidgetItem(attrGroup);
               //attrItem->setIcon(0, QIcon(":/icons/attribute.png"));
               attrItem->setText(0, attr.name());
               attrItem->setText(1, attr.value());
           }
       }

       // 处理子节点
       QDomNode child = element.firstChild();
       while (!child.isNull())
       {
           if (child.isElement())
           {
               // 递归处理元素节点
               processElement(item,child.toElement());
           } else if (child.isText())
           {
               QTreeWidgetItem *attrVaule = new QTreeWidgetItem(item);
               attrVaule->setText(0, "Value");
               // 添加文本内容
               QTreeWidgetItem* textItem = new QTreeWidgetItem(attrVaule);
               textItem->setText(0, child.toText().data().trimmed());
           }
           child = child.nextSibling();
       }

}

bool ATEMainWindow::saveTreeToXml(QTreeWidget* treeWidget, const QString& filePath) {
    QDomDocument doc;

    // 创建根元素（使用固定名称或自定义）
    QDomElement root = doc.createElement("Root");
    doc.appendChild(root);

    // 递归添加子节点
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        serializeItem(treeWidget->topLevelItem(i), root, doc);
    }

    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString(4);  // 缩进4空格
    file.close();
    return true;
}

void ATEMainWindow::serializeItem(QTreeWidgetItem* item, QDomElement& parent, QDomDocument& doc) {
    // 解析节点文本（标签名+属性）
    QStringList parts = item->text(0).split(' ');
    QString tagName = parts[0];

    // 创建元素
    QDomElement elem = doc.createElement(tagName);
    parent.appendChild(elem);

    // 添加属性
    for (int i = 1; i < parts.size(); ++i) {
        if (parts[i].contains('=')) {
            QStringList attr = parts[i].split('=');
            QString name = attr[0];
            QString value = attr[1].mid(1, attr[1].length() - 2); // 去除引号
            elem.setAttribute(name, value);
        }
    }

    // 递归处理子节点
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem* child = item->child(i);
        serializeItem(child, elem, doc);
    }
}

void ATEMainWindow::onTreeItemClicked(QTreeWidgetItem *item,int column )
{
    //
    //Q_UNUSED(column);

    // 更新属性表格
    m_customtreeWidget->clear();

    bool elementflag=false;

    // 创建键值对容器
    QMap<QString, QString> attributes;

    //创建值
    QString   strvalue;
    QString   strname;

    // 获取节点属性数据
    for (int i = 0; i < item->childCount(); ++i)
    {
           QTreeWidgetItem *child = item->child(i);
           QString childType = child->text(0);

           // 只关心属性节点和文本节点
           if (childType == "Value" || childType == "Attributes")
           {
               elementflag =true;
           }

           //把属性所有的值都拿出来
           if(childType == "Attributes")
           {
               for (int i = 0; i < child->childCount(); ++i)
               {
                   QTreeWidgetItem *childitem = child->child(i);
                   attributes[childitem->text(0)] =childitem->text(1);
               }
           }

           //把Value的值拿出来
           if(childType == "Value")
           {
               strvalue = child->child(0)->text(0);
           }
     }

    if(false==elementflag)
    {
        return;
    }

    //取值name的值
    strname = item->text(0);


    QTreeWidgetItem *rootItem1 = new QTreeWidgetItem(m_customtreeWidget);
    rootItem1->setText(0, "Attributes");

    QTreeWidgetItem *rootItem2 = new QTreeWidgetItem(m_customtreeWidget);
    rootItem2->setText(0, "Key");

    for (auto it = attributes.begin(); it != attributes.end(); ++it)
    {
        qDebug() << it.key() << "=>" << it.value();

         QTreeWidgetItem *child = new QTreeWidgetItem(rootItem1);
         child->setFlags(child->flags() | Qt::ItemIsEditable); // 必须允许编辑
         child->setText(0, it.key());
         child->setText(1,it.value());

    }

    //Name
    QTreeWidgetItem *child = new QTreeWidgetItem(rootItem2);
    child->setFlags(child->flags() | Qt::ItemIsEditable); // 必须允许编辑
    child->setText(0, "Name");
    child->setText(1, strname);

    //Vaule
    child = new QTreeWidgetItem(rootItem2);
    child->setFlags(child->flags() | Qt::ItemIsEditable); // 必须允许编辑
    child->setText(0, "Value");
    child->setText(1, strvalue);


//    child2->setText(1,pairs[0]);


//         QTreeWidgetItem *child11 = new QTreeWidgetItem(rootItem1);
//         child11->setFlags(child11->flags() | Qt::ItemIsEditable); // 必须允许编辑
//         child11->setText(0, it.key());
//         child11->setText(1,it.value());

    //展开
    m_customtreeWidget->expandAll();

}

QMap<QString, QString> ATEMainWindow::parseAttributeJson(const QByteArray &jsonData)
{
    QMap<QString, QString> attributes;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        for (auto it = obj.begin(); it != obj.end(); ++it)
        {
            attributes[it.key()] = it.value().toString();
        }
    }
    return attributes;
}
