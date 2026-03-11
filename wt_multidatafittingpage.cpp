/*
 * 文件名: wt_multidatafittingpage.cpp
 * 文件作用: 多数据试井拟合多标签页管理类实现文件
 * 功能描述:
 * 1. 实现多标签 QTabWidget 的动态增删改查。
 * 2. 移除污染子窗口和弹窗的全局代码级样式表。
 * 3. 统一风格，并增加详细注释。
 */

#include "wt_multidatafittingpage.h"
#include "ui_wt_multidatafittingpage.h"
#include "wt_multidatafittingwidget.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

// 构造函数：初始化界面，打开默认页签
WT_MultidataFittingPage::WT_MultidataFittingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WT_MultidataFittingPage),
    m_modelManager(nullptr)
{
    ui->setupUi(this);

    // 设置背景色为白色，与拟合界面统一
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("background-color: white;");

    // 默认打开一个空的分析页
    on_btnNew_clicked();
}

// 析构函数：释放 UI 资源
WT_MultidataFittingPage::~WT_MultidataFittingPage()
{
    delete ui;
}

// 设置模型管理器，并向各个子页签传递
void WT_MultidataFittingPage::setModelManager(ModelManager *m)
{
    m_modelManager = m;
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        WT_MultidataFittingWidget* w = qobject_cast<WT_MultidataFittingWidget*>(ui->tabWidget->widget(i));
        if (w) w->setModelManager(m);
    }
}

// 设置项目数据模型映射
void WT_MultidataFittingPage::setProjectDataModels(const QMap<QString, QStandardItemModel *> &models)
{
    m_dataMap = models;
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        WT_MultidataFittingWidget* w = qobject_cast<WT_MultidataFittingWidget*>(ui->tabWidget->widget(i));
        if (w) w->setProjectDataModels(models);
    }
}

// 获取当前激活的多数据分析窗口对象
WT_MultidataFittingWidget* WT_MultidataFittingPage::currentWidget()
{
    if (ui->tabWidget->count() == 0) return nullptr;
    return qobject_cast<WT_MultidataFittingWidget*>(ui->tabWidget->currentWidget());
}

// 槽函数：新建多数据分析页签
void WT_MultidataFittingPage::on_btnNew_clicked()
{
    QString name = QString("多数据分析 %1").arg(ui->tabWidget->count() + 1);

    WT_MultidataFittingWidget* newWidget = new WT_MultidataFittingWidget(this);
    if (m_modelManager) newWidget->setModelManager(m_modelManager);
    if (!m_dataMap.isEmpty()) newWidget->setProjectDataModels(m_dataMap);

    int index = ui->tabWidget->addTab(newWidget, name);
    ui->tabWidget->setCurrentIndex(index);
}

// 槽函数：重命名当前页签
void WT_MultidataFittingPage::on_btnRename_clicked()
{
    int index = ui->tabWidget->currentIndex();
    if (index < 0) return;

    bool ok;
    QString oldName = ui->tabWidget->tabText(index);
    QString newName = QInputDialog::getText(this, "重命名", "请输入新的分析名称:", QLineEdit::Normal, oldName, &ok);

    if (ok && !newName.isEmpty()) {
        ui->tabWidget->setTabText(index, newName);
    }
}

// 槽函数：删除当前多数据分析页签
void WT_MultidataFittingPage::on_btnDelete_clicked()
{
    int index = ui->tabWidget->currentIndex();
    if (index < 0) return;

    // 添加最少保留一个页签的判断
    if(ui->tabWidget->count() == 1) {
        QMessageBox::warning(this, "警告", "至少需要保留一个分析页面！");
        return;
    }

    if (QMessageBox::question(this, "确认删除", "确定要删除当前多数据分析页吗？\n删除后数据不可恢复！", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QWidget* w = ui->tabWidget->widget(index);
        ui->tabWidget->removeTab(index);
        w->deleteLater();

        if (ui->tabWidget->count() == 0) {
            on_btnNew_clicked();
        }
    }
}
