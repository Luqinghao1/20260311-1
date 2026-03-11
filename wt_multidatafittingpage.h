/*
 * 文件名: wt_multidatafittingpage.h
 * 文件作用: 多数据试井拟合多标签页管理类头文件
 * 功能描述:
 * 1. 提供多数据分析的多标签(Tab)管理功能。
 * 2. 统筹实现“新建分析”、“重命名”、“删除当前页”的交互逻辑。
 * 3. 负责向各个独立的多数据拟合子窗口分发项目数据和模型管理器。
 */

#ifndef WT_MULTIDATAFITTINGPAGE_H
#define WT_MULTIDATAFITTINGPAGE_H

#include <QWidget>
#include <QMap>
#include <QStandardItemModel>
#include "modelmanager.h"

namespace Ui {
class WT_MultidataFittingPage;
}

class WT_MultidataFittingWidget;

class WT_MultidataFittingPage : public QWidget
{
    Q_OBJECT

public:
    explicit WT_MultidataFittingPage(QWidget *parent = nullptr);
    ~WT_MultidataFittingPage();

    // 设置全局模型管理器，并向所有子标签分发
    void setModelManager(ModelManager* m);

    // 设置项目数据池，并向所有子标签分发
    void setProjectDataModels(const QMap<QString, QStandardItemModel*>& models);

private slots:
    // 新建分析页
    void on_btnNew_clicked();
    // 重命名当前分析页
    void on_btnRename_clicked();
    // 删除当前分析页
    void on_btnDelete_clicked();

private:
    Ui::WT_MultidataFittingPage *ui;
    ModelManager* m_modelManager;
    QMap<QString, QStandardItemModel*> m_dataMap;

    WT_MultidataFittingWidget* currentWidget();
};

#endif // WT_MULTIDATAFITTINGPAGE_H
