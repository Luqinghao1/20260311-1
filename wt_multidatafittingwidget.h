/*
 * 文件名: wt_multidatafittingwidget.h
 * 文件作用: 多数据试井拟合分析核心界面头文件
 * 功能描述:
 * 1. 声明多数据拟合主界面的整体布局、MDI多图表系统。
 * 2. 声明数据组的管理结构体 (DataGroup)。
 * 3. 包含了结果保存、模型拟合等交互按钮的槽函数。
 */

#ifndef WT_MULTIDATAFITTINGWIDGET_H
#define WT_MULTIDATAFITTINGWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <QJsonObject>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "modelmanager.h"
#include "fittingparameterchart.h"
#include "fittingchart1.h"
#include "fittingchart2.h"
#include "fittingchart3.h"
#include "mousezoom.h"
#include "fittingcore.h"
#include "fittingchart.h"

namespace Ui {
class WT_MultidataFittingWidget;
}

// 结构体：用于存储单个实测数据组的核心属性与序列
struct DataGroup {
    QString groupName;
    double weight;
    QColor color;
    bool showDeltaP;
    bool showDerivative;

    QVector<double> time;
    QVector<double> deltaP;
    QVector<double> derivative;

    QVector<double> origTime;
    QVector<double> origDeltaP;
    QVector<double> origDerivative;
    QVector<double> rawP;
};

class WT_MultidataFittingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WT_MultidataFittingWidget(QWidget *parent = nullptr);
    ~WT_MultidataFittingWidget();

    void setModelManager(ModelManager* m);
    void setProjectDataModels(const QMap<QString, QStandardItemModel*>& models);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void on_btnAddData_clicked();
    void on_btn_modelSelect_clicked();
    void on_btnSelectParams_clicked();
    void on_btnSampling_clicked();

    void on_btnRunFit_clicked();
    void on_btnSave_clicked(); // 新增：保存结果槽函数
    void on_btnStop_clicked();

    void on_btnImportModel_clicked();

    void onDataWeightChanged(int row, int col);
    void onTableCellClicked(int row, int col);
    void showContextMenu(const QPoint &pos);
    void on_comboFitMode_currentIndexChanged(int index);

    void onParameterChangedByWheel();
    void onParameterTableItemChanged(QTableWidgetItem *item);

    void onIterationUpdate(double err, const QMap<QString,double>& p, const QVector<double>& t, const QVector<double>& p_curve, const QVector<double>& d_curve);
    void onFitFinished();

    void onExportCurveData();
    void onSemiLogLineMoved(double k, double b);

    void layoutCharts();
    void updateModelCurve(const QMap<QString, double>* explicitParams = nullptr, bool autoScale = false, bool calcError = true);

private:
    Ui::WT_MultidataFittingWidget *ui;
    ModelManager* m_modelManager;
    FittingCore* m_core;
    FittingChart* m_chartManager;

    QMdiArea* m_mdiArea;
    FittingChart1* m_chartLogLog;
    FittingChart2* m_chartSemiLog;
    FittingChart3* m_chartCartesian;

    QMdiSubWindow* m_subWinLogLog;
    QMdiSubWindow* m_subWinSemiLog;
    QMdiSubWindow* m_subWinCartesian;

    MouseZoom* m_plotLogLog;
    MouseZoom* m_plotSemiLog;
    MouseZoom* m_plotCartesian;

    FittingParameterChart* m_paramChart;
    QMap<QString, QStandardItemModel*> m_dataMap;
    ModelManager::ModelType m_currentModelType;

    QList<DataGroup> m_dataGroups;
    bool m_isFitting;
    bool m_isUpdatingTable;
    double m_userDefinedTimeMax;

    void setupPlot();
    void initializeDefaultModel();
    void hideUnwantedParams();
    void loadProjectParams();
    void refreshDataTable();
    bool validateDataWeights();

    QVector<double> interpolate(const QVector<double>& srcX, const QVector<double>& srcY, const QVector<double>& targetX);
    QVector<double> generateCommonTimeGrid();

    QColor getColor(int index);
};

#endif // WT_MULTIDATAFITTINGWIDGET_H
