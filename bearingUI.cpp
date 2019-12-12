#include "BearingUI.h"
#include "ui_bearingui.h"
template<typename T>bool checkNumberType(T num,QString &&type){
    if(typeid(T).name()==type){
        return true;
    }
    return false;
}
BearingUI::BearingUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BearingUI)
{
    ui->setupUi(this);
    //UI设置
    initForm();
    initLeftBtnConfig();
    initLineEditConfig();
    initComboBoxConfig();
    //参数显示
    xmlParser.readBearingData(QString("E:\\Qt\\uidemo08\\轴承型号及参数.xml"));
    bearingsVec = xmlParser.getData();
    addBrandComboxItem();
    addTypeComboxItem();
    addNumberComboxItem();
    showBearingParam();
    enableLineEdit(false);
}
void BearingUI::initForm(){
    setWindowFlags(Qt::FramelessWindowHint);
    ui->widgetLeftConfig->setProperty("flag", "left");
}
void BearingUI::initLeftBtnConfig()
{
    pixCharConfig << 0xf031 << 0xf036  << 0xf055<< 0xf249;
    btnsConfig <<ui->tbtnConfig1 << ui->tbtnConfig2 << ui->tbtnConfig3<< ui->tbtnConfig4  ;

    int count = btnsConfig.count();
    for (int i = 0; i < count; i++) {
        btnsConfig.at(i)->setCheckable(true);
        btnsConfig.at(i)->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(btnsConfig.at(i), SIGNAL(clicked(bool)), this, SLOT(leftBtnClick()));
    }

    IconHelper::Instance()->setStyle(ui->widgetLeftConfig, btnsConfig, pixCharConfig, 10, 20, 15, "left", 5);
    ui->tbtnConfig1->click();
}
void BearingUI::initLineEditConfig(){
    lineEdits<< ui->accuracyLevelLineEdit<<ui->axialLoadLineEdit<<ui->ballDiameterLineEdit<<ui->ballNumberLineEdit
             <<ui->heatRateLineEdit<<ui->inCurvatureCoeffLineEdit<<ui->inRingSpeedLineEdit<<ui->inDiameterLineEdit
             <<ui->lifeLineEdit<<ui->outCurvatureCoeffLineEdit<<ui->outDiameterLineEdit<<ui->presetContactAngleLineEdit
             <<ui->widthLineEdit<<ui->stressLevelLineEdit<<ui->temperatureLineEdit<<ui->radialLoadLineEdit<<ui->rotaryRatioLineEdit
             <<ui->actualAngleLineEdit;
    for (int i = 0; i < lineEdits.size(); i++) {
        connect(lineEdits.at(i), SIGNAL(textChanged(QString)), this, SLOT(lineEditsChange(const QString &)));
    }
    lineEditsBlockSignals(true);
}
void BearingUI::initComboBoxConfig(){
    comboBoxs<<ui->brandComboBox<<ui->typeComboBox<<ui->numberComboBox;
    for(int i = 0;i<comboBoxs.size();i++){
        connect(comboBoxs.at(i),SIGNAL(currentTextChanged(QString)),this,SLOT(comboBoxTextChanged(QString)));
    }
}
void BearingUI::leftBtnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->text();
    int count = btnsConfig.count();
    for (int i = 0; i < count; i++) {
        if (btnsConfig.at(i) == btn) {
            btnsConfig.at(i)->setChecked(true);
            btnsConfig.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsConfig.at(i), false)));
        } else {
            btnsConfig.at(i)->setChecked(false);
            btnsConfig.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsConfig.at(i), true)));
        }
    }
    if(name == "轴承参数"){
        enableLineEdit(false);
        enableComboBox(true);
        lineEditsBlockSignals(true);
         ui->numberComboBox->setEditable(false);
        showBearingParam();
    }else if(name == "修改参数"){
        enableLineEdit(true);
        enableComboBox(false);
        lineEditsBlockSignals(false);
        state = CHANGING;
    }else if(name =="添加轴承"){
        enableLineEdit(true);
        enableComboBox(true);
        lineEditsClear();
        lineEditsBlockSignals(false);
        state = ADDING;
        //设置numberComboBox为添加型号状态
        ui->numberComboBox->setEditable(true);

        ui->brandComboBox->blockSignals(true);
        ui->brandComboBox->setCurrentIndex(-1);
        ui->brandComboBox->blockSignals(false);
        ui->typeComboBox ->blockSignals(true);
        ui->typeComboBox->setCurrentIndex(-1);
        ui->typeComboBox->blockSignals(false);
        ui->numberComboBox ->blockSignals(true);
        ui->numberComboBox->setCurrentIndex(-1);
        ui->numberComboBox->blockSignals(false);
        ui->numberComboBox->setStyleSheet("QComboBox{font:black}");

    }else if(name == "保存参数"){
        if(state==CHANGING && changeBearingParamBuffer.number!=""){
            saveBearingParam(changeBearingParamBuffer);
            changeBearingsVec(changeBearingParamBuffer);
            changeBearingParamBuffer.clear();
        }else if(state==ADDING){
            addBearingParamBuffer.number = ui->numberComboBox->currentText();
            //saveBearingParam(addBearingParamBuffer);
            changeBearingsVec(addBearingParamBuffer);
            currBrand = addBearingParamBuffer.bearingBrand;
            currType = addBearingParamBuffer.bearingType;
            currNumber = addBearingParamBuffer.number;
            if(ui->numberComboBox->findText(addBearingParamBuffer.number)==-1){
                ui->numberComboBox->blockSignals(true);
                ui->numberComboBox->addItem(addBearingParamBuffer.number);
                ui->numberComboBox->blockSignals(false);
            }
            addBearingParamBuffer.clear();
        }
        if(state != SAVED){
            enableLineEdit(false);
            enableComboBox(false);
            lineEditsBlockSignals(true);
            ui->numberComboBox->setEditable(false);
        }
        state = SAVED;
        showBearingParam();
    }
}
void BearingUI::changeBearingsVec(const Bearing &bearing){
    for(int i = 0;i<bearingsVec.size();i++){
        if(bearingsVec[i].number == bearing.number){
            bearingsVec[i] =  bearing;
        }
    }
}
void BearingUI::lineEditsBlockSignals(bool isBlock){
    for(auto &lineEdit:lineEdits){
        lineEdit->blockSignals(isBlock);
    }
}
void BearingUI::lineEditsChange(const QString &text){
    QLineEdit* lineEdit =(QLineEdit *)sender();
    QString name  = lineEdit->objectName();
    if(state == CHANGING){
        for(int i = 0;i<bearingsVec.size();i++){
            if(currNumber == bearingsVec[i].number){
                changeBearingParamBuffer = bearingsVec[i];
            }
        }
    }
    if(name == "accuracyLevelLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BPP.accuracyLevel = lineEdit->text();
        }else if(state == ADDING){
            addBearingParamBuffer.BPP.accuracyLevel = lineEdit->text();
        }
    }else if(name == "axialLoadLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BOP.axialLoad = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BOP.axialLoad = lineEdit->text().toFloat();
        }
    }else if(name == "ballDiameterLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.ballDiameter = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.ballDiameter = lineEdit->text().toFloat();
        }
    }else if(name == "ballNumberLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.ballNumber = lineEdit->text().toInt();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.ballNumber = lineEdit->text().toInt();
        }
    }else if(name == "heatRateLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BPP.heatRate = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BPP.heatRate = lineEdit->text().toFloat();
        }
    }else if(name == "inCurvatureCoeffLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.inCurvatureCoeff = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.inCurvatureCoeff = lineEdit->text().toFloat();
        }
    }else if(name == "inRingSpeedLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BOP.inneRingSpeed = lineEdit->text().toInt();
            qDebug()<<changeBearingParamBuffer.BOP.inneRingSpeed<<" ";
        }else if(state == ADDING){
            addBearingParamBuffer.BOP.inneRingSpeed = lineEdit->text().toInt();
        }
    }else if(name == "inDiameterLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.insideDiameter = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.insideDiameter = lineEdit->text().toFloat();
        }
    }else if(name == "lifeLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BPP.life = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BPP.life = lineEdit->text().toFloat();
        }
    }else if(name == "outCurvatureCoeffLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.outCurvatureCoeff = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.outCurvatureCoeff = lineEdit->text().toFloat();
        }
    }else if(name == "outDiameterLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.outsideDiameter = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.outsideDiameter = lineEdit->text().toFloat();
        }
    }else if(name == "presetContactAngleLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.presetContactAngle = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.presetContactAngle = lineEdit->text().toFloat();
        }
    }else if(name == "widthLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.width = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.width = lineEdit->text().toFloat();
        }
    }else if(name == "stressLevelLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BPP.stressLevel = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BPP.stressLevel = lineEdit->text().toFloat();
        }
    }else if(name == "temperatureLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BOP.temperature = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BOP.temperature = lineEdit->text().toFloat();
        }
    }else if(name == "radialLoadLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BOP.radialLoad = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BOP.radialLoad = lineEdit->text().toFloat();
        }
    }else if(name == "rotaryRatioLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BPP.rotaryRatio = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BPP.rotaryRatio = lineEdit->text().toFloat();
        }
    }else if(name == "actualAngleLineEdit"){
        if(state == CHANGING){
            changeBearingParamBuffer.BSP.actualContactAngle = lineEdit->text().toFloat();
        }else if(state == ADDING){
            addBearingParamBuffer.BSP.actualContactAngle = lineEdit->text().toFloat();
        }
    }else{
        return ;
    }

}

void BearingUI::lineEditsClear(){
    for(auto &lineEdit:lineEdits){
        lineEdit->clear();
    }
}
void BearingUI::enableLineEdit(bool isEnable)
{
    for(auto &lineEdit:lineEdits){
        lineEdit->setEnabled(isEnable);
    }
}
void BearingUI::enableComboBox(bool isEnable){
    ui->brandComboBox->setEnabled(isEnable);
    ui->typeComboBox->setEnabled(isEnable);
    ui->numberComboBox->setEnabled(isEnable);
}
void BearingUI::saveBearingParam(const Bearing &bearing){
    xmlParser.writeBearingData(QString("E:\\Qt\\uidemo08\\轴承型号及参数.xml"),bearing);
}
//轴承参数显示、修改、添加、保存
void BearingUI::showBearingParam(){
     ui->brandComboBox->setCurrentText(currBrand);
     ui->brandComboBox->setStyleSheet("color:white;");
     ui->typeComboBox->setCurrentText(currType);
     ui->typeComboBox->setStyleSheet("color:white;");
     ui->numberComboBox->setCurrentText(currNumber);
     ui->numberComboBox->setStyleSheet("color:white;");
    //=======================================读取轴承的类型号和参数=====================================================
    if(!bearingsVec.empty())
    {
        Bearing bearing;
        for(QString::size_type i = 0;i<bearingsVec.size();i++)
        {
            if(bearingsVec[i].number == currNumber)
            {
                bearing = bearingsVec[i];
            }
        }
        //=========================显示轴承样图=========================================

        //===========================显示轴承参数========================================
        for(auto &lineEdit:lineEdits){
            lineEdit->clear();
        }
        ui->inDiameterLineEdit->setText(QString("%1").arg(bearing.BSP.insideDiameter));
        ui->inDiameterLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->outDiameterLineEdit->setText(QString("%1").arg(bearing.BSP.outsideDiameter));
        ui->outDiameterLineEdit->setStyleSheet("color:white;");

        //-------------------------------------------------------------------------------
        ui->widthLineEdit->setText(QString("%1").arg(bearing.BSP.width));
        ui->widthLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->ballNumberLineEdit->setText(QString("%1").arg(bearing.BSP.ballNumber));
        ui->ballNumberLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->ballDiameterLineEdit->setText(QString("%1").arg(bearing.BSP.ballDiameter));
        ui->ballDiameterLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->outCurvatureCoeffLineEdit->setText(QString("%1").arg(bearing.BSP.outCurvatureCoeff));
        ui->outCurvatureCoeffLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->inCurvatureCoeffLineEdit->setText(QString("%1").arg(bearing.BSP.inCurvatureCoeff));
        ui->inCurvatureCoeffLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->presetContactAngleLineEdit->setText(QString("%1").arg(bearing.BSP.presetContactAngle));
        ui->presetContactAngleLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->accuracyLevelLineEdit->setText(QString("%1").arg(bearing.BSP.actualContactAngle));
        ui->accuracyLevelLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->accuracyLevelLineEdit->setText(bearing.BPP.accuracyLevel);
        ui->accuracyLevelLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->lifeLineEdit->setText(QString("%1").arg(bearing.BPP.life));
        ui->lifeLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->rotaryRatioLineEdit->setText(QString("%1").arg(bearing.BPP.rotaryRatio));
        ui->rotaryRatioLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->stressLevelLineEdit->setText(QString("%1").arg(bearing.BPP.stressLevel));
        ui->stressLevelLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->heatRateLineEdit->setText(QString("%1").arg(bearing.BPP.heatRate));
        ui->heatRateLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->inRingSpeedLineEdit->setText(QString("%1").arg(bearing.BOP.inneRingSpeed));
        ui->inRingSpeedLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->axialLoadLineEdit->setText(QString("%1").arg(bearing.BOP.axialLoad));
        ui->axialLoadLineEdit->setStyleSheet("color:white;");
        //-------------------------------------------------------------------------------
        ui->radialLoadLineEdit->setText(QString("%1").arg(bearing.BOP.radialLoad));
        ui->radialLoadLineEdit->setStyleSheet("color:white;");
        //------------------------------------------------------------------------------
        ui->temperatureLineEdit->setText(QString("%1\260C").arg(bearing.BOP.temperature));
        ui->temperatureLineEdit->setStyleSheet("color:white;");
    }
}
void BearingUI::addBrandComboxItem()
{
    ui->brandComboBox->clear();
    for(int i = 0;i<bearingBrand.size();i++){
        if (ui->brandComboBox->findText(bearingBrand[i])==-1){
            ui->brandComboBox->blockSignals(true);
            ui->brandComboBox->addItem(bearingBrand[i]);
            ui->brandComboBox->blockSignals(false);
        }
    }
    currBrand = bearingBrand[0];
}
void BearingUI::addTypeComboxItem()
{
    //添加轴承的种类信息
    for(std::size_t i = 0;i<bearingType.size();i++){
        if(ui->typeComboBox->findText(bearingType[i])==-1){
            ui->typeComboBox->blockSignals(true);
            ui->typeComboBox->addItem(bearingType[i]);
            ui->typeComboBox->blockSignals(false);
        }
    }
    currType = bearingType[0];
}
//增加属于当前类型与品牌下轴承型号
void BearingUI::addNumberComboxItem()
{
    QVector<Bearing> bearing;
    for(std::size_t i = 0;i<bearingsVec.size();i++)
    {
        if(bearingsVec[i].bearingType == currType &&
                bearingsVec[i].bearingBrand== currBrand )
        {
            bearing.push_back(bearingsVec[i]);
        }
    }
    for(QString::size_type i = 0;i<bearing.size();i++){
        if(ui->numberComboBox->findText(bearing[i].number)==-1){
            ui->numberComboBox->blockSignals(true);
            ui->numberComboBox->addItem(bearing[i].number);
            ui->numberComboBox->blockSignals(false);
        }
        if(i == 0){
            ui->numberComboBox->setCurrentText(bearing[0].number);
            currNumber = bearing[0].number;
        }
    }
}

void BearingUI::comboBoxTextChanged(const QString &arg1){
    QComboBox *comboBox = (QComboBox *)sender();
    QString objectName = comboBox->objectName();
    if(objectName == "brandComboBox"){
        if(state == SAVED){
            currBrand = arg1;
            if(!bearingsVec.empty()){
                addNumberComboxItem();
                showBearingParam();
            }
        }else if(state == ADDING){
            addBearingParamBuffer.bearingBrand = arg1;
            //qDebug()<<"brandComboBox:  "<<arg1;
        }
    }else if (objectName == "typeComboBox"){
        if(state == SAVED){
            currType = arg1;
            ui->numberComboBox->clear();
            if(!bearingsVec.empty()){
                addNumberComboxItem();
                showBearingParam();
            }
        }else if(state == ADDING){
            addBearingParamBuffer.bearingType = arg1;
            //qDebug()<<"typeComboBox:  "<<arg1;
        }
    }else if(objectName == "numberComboBox"){
        if(state == SAVED){
            currNumber = arg1;
            ui->typeComboBox->setCurrentText(currNumber);
            showBearingParam();
        }else if(state == ADDING){
            addBearingParamBuffer.bearingType = arg1;
            //qDebug()<<"numberComboBox:  "<<arg1;
        }
    }else {
        return ;
    }
}
BearingUI::~BearingUI()
{
    delete ui;
}
