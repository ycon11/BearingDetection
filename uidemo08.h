#ifndef UIDEMO08_H
#define UIDEMO08_H

#include <QWidget>
#include "bearing.h"
#include "bearingUI.h"
class QToolButton;

namespace Ui {
class UIDemo08;
}

class UIDemo08 : public QWidget
{
    Q_OBJECT

public:
    explicit UIDemo08(QWidget *parent = 0);
    ~UIDemo08();

private:
    Ui::UIDemo08 *ui;    

    QList<int> pixCharMain;
    QList<QToolButton *> btnsMain;

    QList<int> pixCharConfig;
    QList<QToolButton *> btnsConfig;

    QVector<Bearing> bearingsVec;

private slots:
    void initForm();
    void buttonClick();
private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
};

#endif // UIDEMO08_H
