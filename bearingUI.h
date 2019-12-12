#ifndef BEARINGUI_H
#define BEARINGUI_H

#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <typeinfo>
#include "bearing.h"
#include "iconhelper.h"
typedef enum {SAVED,CHANGING,ADDING} UIState;
namespace Ui {
class BearingUI;
}

class BearingUI : public QWidget
{
    Q_OBJECT

public:
    explicit BearingUI(QWidget *parent = 0);
    //界面设置函数
    void initForm();
    void initLeftBtnConfig();

    //轴承参数相关函数
    void showBearingParam();
    void addNumberComboxItem();
    void addTypeComboxItem();
    void addBrandComboxItem();
    void enableLineEdit(bool);
    void enableComboBox(bool);
    void changeBearingParam();
    void addBearingItem();
    void saveBearingParam(const Bearing &bearing);
    void initLineEditConfig();
    void initComboBoxConfig();
    void lineEditsBlockSignals(bool isBlock);
    void changeBearingsVec(const Bearing &bearing);
    void lineEditsClear();
    ~BearingUI();

private slots:

     void leftBtnClick();

     void lineEditsChange(const QString &text);

     void comboBoxTextChanged(const QString &arg1);

private:
    Ui::BearingUI *ui;

    QList<int> pixCharConfig;
    QList<QToolButton *> btnsConfig;
    QList<QLineEdit *> lineEdits;
    QList<QComboBox *> comboBoxs;

    QString currBrand ;
    QString currType;
    QString currNumber;
    XMLParser xmlParser;
    UIState state = SAVED ; //三种状态 0：显示轴承参数  1：修改轴承参数并未保存 2：增加新的轴承参数并且未保存
    Bearing changeBearingParamBuffer;
    Bearing addBearingParamBuffer;
    QVector<Bearing> bearingsVec;

};

#endif // BEARINGUI_H
