#ifndef ATEMAINWINDOW_H
#define ATEMAINWINDOW_H

#include <QMainWindow>

#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QFileDialog>

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTreeWidgetItem>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>


#include "customtreewidget.h"
#include "MyShowQTreeWidget.h"

// 节点类型枚举
enum NodeType { ElementNode, AttributeNode, TextNode };

QT_BEGIN_NAMESPACE
namespace Ui { class ATEMainWindow; }
QT_END_NAMESPACE

class ATEMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ATEMainWindow(QWidget *parent = nullptr);
    ~ATEMainWindow();

    void InitUI();

    void populateTreeFromXml(QTreeWidget* treeWidget, const QString& filePath);
    void parseElement(const QDomElement& element, QTreeWidgetItem* parentItem);
    bool saveTreeToXml(QTreeWidget* treeWidget, const QString& filePath);
    void serializeItem(QTreeWidgetItem* item, QDomElement& parent, QDomDocument& doc);

    void processElement(QTreeWidgetItem *parent, const QDomElement &element);

    // 使用自定义的 CustomTreeWidget
    CustomTreeWidget *m_customtreeWidget;
    QTableWidget     *m_ParametersTable;

    MyShowTreeWidget  *treeTestViewWidget;


    QString  m_selectedFilePath;

    QMap<QString, QString>  parseAttributeJson(const QByteArray &jsonData);

private:
    Ui::ATEMainWindow *ui;

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);

    void OpenFile();

    void SaveItem();

    void onTreeItemClicked(QTreeWidgetItem *item,int column);
};
#endif // ATEMAINWINDOW_H
