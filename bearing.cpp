#include<bearing.h>
QVector<QString> bearingBrand={"SKF轴承","NSK轴承","FAG轴承","NTN轴承","IKO轴承","TIMKEN轴承","ZWZ轴承","HRB轴承","LYC轴承"};
QVector<QString> bearingType={"深沟球轴承","角接触球轴承","圆柱滚子轴承","推力球轴承"};
QStringList bearingParam={"number","类型","品牌","内径","外径","宽度","球数量","球径","外沟曲率系数","内沟曲率系数","预设接触角",
                          "精度等级","寿命","旋滚比","发热率","内圈转速","轴向载荷","径向载荷","轴承温度"};


//轴承尺寸参数
BearingSizeParam::BearingSizeParam():insideDiameter(0.0),outsideDiameter(0.0),width(0.0),ballNumber(0),ballDiameter(0.0)
  ,inCurvatureCoeff(0.0),outCurvatureCoeff(0.0), presetContactAngle(0.0) ,actualContactAngle(0.0){
}
BearingSizeParam & BearingSizeParam::operator=(const BearingSizeParam &bsp){
    if(this!= &bsp){
        insideDiameter = bsp.insideDiameter;
        outsideDiameter = bsp.outsideDiameter;
        width = bsp.width;
        ballNumber = bsp.ballNumber;
        ballDiameter = bsp.ballDiameter;
        inCurvatureCoeff = bsp.inCurvatureCoeff;
        outCurvatureCoeff = bsp.outCurvatureCoeff;
        presetContactAngle = bsp.presetContactAngle;
        actualContactAngle = bsp.actualContactAngle;
    }
    return *this;
}
void BearingSizeParam::clear(){
    insideDiameter = 0.0;
    outsideDiameter = 0.0;
    width = 0.0;
    ballNumber = 0;
    ballDiameter = 0.0;
    outCurvatureCoeff = 0.0;
    inCurvatureCoeff = 0.0;
    presetContactAngle = 0.0;
}
//
BearingPerformParam::BearingPerformParam():accuracyLevel(""),life(0.0),rotaryRatio(0.0),stressLevel(0.0),heatRate(0.0){

}
BearingPerformParam &BearingPerformParam::operator=(const BearingPerformParam &bpp){
    if(this != &bpp){
        accuracyLevel = "";
        life = bpp.life;
        rotaryRatio = bpp.rotaryRatio;
        stressLevel = bpp.stressLevel;
        heatRate = bpp.heatRate;
    }
    return *this;
}

void BearingPerformParam::clear(){
    accuracyLevel.clear();
    life = 0;
    rotaryRatio =0.0;
    heatRate = 0.0;
}

//轴承参数
BearingOperatingParam::BearingOperatingParam():inneRingSpeed(0),axialLoad(0.0),radialLoad(0.0),temperature(0.0){


}
BearingOperatingParam &BearingOperatingParam::operator=(const BearingOperatingParam &bop){
    if(this != &bop){
        inneRingSpeed = bop.inneRingSpeed;
        axialLoad = bop.axialLoad;
        radialLoad = bop.radialLoad;
        temperature = bop.temperature;
    }
    return *this;
}
void BearingOperatingParam::clear(){
    inneRingSpeed = 0;
    axialLoad = 0;
    radialLoad = 0;
    temperature = 0;
}


//轴承工况参数
Bearing::Bearing():number(""),bearingBrand(""),bearingType(""),BSP(BearingSizeParam()),BOP(BearingOperatingParam())
  ,BPP(BearingPerformParam()){

}

Bearing::Bearing(const QString &num):number(num),bearingBrand(""),bearingType(""),BSP(BearingSizeParam())
     ,BOP(BearingOperatingParam())
      ,BPP(BearingPerformParam()){

}
Bearing& Bearing::operator=(const Bearing &bearing){
    if(this != &bearing){
        number = bearing.number;
        bearingBrand = bearing.bearingBrand;
        bearingType = bearing.bearingType;
        BSP = bearing.BSP;
        BOP = bearing.BOP;
        BPP = bearing.BPP;
    }
    return *this;
}
void Bearing::clear(){
    number.clear();
    bearingBrand.clear();
    bearingType.clear();

    BSP.clear();
    BOP.clear();
    BPP.clear();
}



//XML解析
bool XMLParser::readBearingData(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)){

         return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "bearings")
    {
        return false;
    }
    parserBearingsMembers(root);
    file.close();
    return true;
}

void XMLParser::parserBearingsMembers(const QDomElement &element){
    QDomNode child = element.firstChild();

    while (!child.isNull())
    {
        if (child.toElement().tagName() == "bearing")
        {
            QString number = child.attributes().item(0).nodeValue();
            bearingsVec.push_back(Bearing(number));
            parserBearing(child.toElement());
        }
        child = child.nextSibling();
    }
}
void XMLParser::parserBearing(const QDomElement &element){
    //element = bearing
    auto iter = bearingsVec.end()-1;
    auto children = element.childNodes();
    iter->bearingType = children.at(0).toElement().text();
    iter->bearingBrand = children.at(1).toElement().text();
    iter->BSP.insideDiameter = children.at(2).toElement().text().toFloat();
    iter->BSP.outsideDiameter = children.at(3).toElement().text().toFloat();
    iter->BSP.width = children.at(4).toElement().text().toFloat();
    iter->BSP.ballNumber = children.at(5).toElement().text().toInt();
    iter->BSP.ballDiameter = children.at(6).toElement().text().toFloat();
    iter->BSP.outCurvatureCoeff = children.at(7).toElement().text().toFloat();
    iter->BSP.inCurvatureCoeff = children.at(8).toElement().text().toFloat();
    iter->BSP.presetContactAngle = children.at(9).toElement().text().toFloat();
    iter->BPP.accuracyLevel = children.at(10).toElement().text();
    iter->BPP.life = children.at(11).toElement().text().toFloat();
    iter->BPP.rotaryRatio = children.at(12).toElement().text().toFloat();
    iter->BPP.heatRate = children.at(13).toElement().text().toFloat();
    iter->BOP.inneRingSpeed = children.at(14).toElement().text().toInt();
    iter->BOP.axialLoad = children.at(15).toElement().text().toFloat();
    iter->BOP.radialLoad = children.at(16).toElement().text().toFloat();
    iter->BOP.temperature = children.at(17).toElement().text().toFloat();
}
bool XMLParser::writeBearingData(const QString &fileName,const Bearing  &bearing){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QDomDocument doc;
    QString errorMsg;
    int errorRow;
    int errorCol;
    if (!doc.setContent(&file,true,&errorMsg,&errorRow,&errorCol)){
         qDebug()<<errorMsg<<"   \n"<<"line: "<<errorRow<<"\ncol"<<errorCol;
         return false;
     }
    file.close();
    QDomElement root = doc.documentElement();
    if (root.tagName() != "bearings")
    {
        return false;
    }
    //该轴承种类是否存在如果存在则为修改已存在的轴承，否则为增加不存在的轴承
    QDomNode child = root.firstChild();
    while (!child.isNull()){
        if (child.toElement().tagName() == "bearing"){

            QString number = child.attributes().item(0).nodeValue();
            if(number == bearing.number){
                auto oldParamNodes = child.toElement().childNodes();
                if(bearing.bearingType != oldParamNodes.at(0).toElement().text()){
                    oldParamNodes.at(0).firstChild().setNodeValue(bearing.bearingType);
                }
                if(bearing.bearingBrand != oldParamNodes.at(1).toElement().text()){
                    oldParamNodes.at(1).firstChild().setNodeValue(bearing.bearingBrand);
                }
                if(bearing.BSP.insideDiameter != oldParamNodes.at(2).toElement().text().toFloat()){
                    oldParamNodes.at(2).firstChild().setNodeValue(QString("%1").arg(bearing.BSP.insideDiameter));
                }
                if(bearing.BSP.outsideDiameter != oldParamNodes.at(3).toElement().text().toFloat()){
                     oldParamNodes.at(3).firstChild().setNodeValue(QString("%1").arg(bearing.BSP.outsideDiameter));
                }
                if(bearing.BSP.width != oldParamNodes.at(4).toElement().text().toFloat()){
                     oldParamNodes.at(4).firstChild().setNodeValue(QString("%1").arg(bearing.BSP.width));
                }
                if(bearing.BSP.ballNumber != oldParamNodes.at(5).toElement().text().toInt()){
                     oldParamNodes.at(5).firstChild().setNodeValue(QString("%1").arg(bearing.BSP.ballNumber));
                }
                if(bearing.BSP.ballDiameter != oldParamNodes.at(6).toElement().text().toFloat()){
                     oldParamNodes.at(6).firstChild().setNodeValue(QString("%1").arg(bearing.BSP.ballDiameter));
                }
                if(bearing.BSP.outCurvatureCoeff != oldParamNodes.at(7).toElement().text().toFloat()){
                     oldParamNodes.at(7).firstChild().setNodeValue(QString("%1").arg(bearing.BSP.outCurvatureCoeff));
                }
                if(bearing.BSP.inCurvatureCoeff != oldParamNodes.at(8).toElement().text().toFloat()){
                     oldParamNodes.at(8).firstChild().setNodeValue(QString("%1").arg(bearing.BSP.inCurvatureCoeff));
                }
                if(bearing.BSP.presetContactAngle != oldParamNodes.at(9).toElement().text().toFloat()){
                     oldParamNodes.at(9).firstChild().setNodeValue(QString("%1").arg(bearing.BSP.presetContactAngle));
                }
                if(bearing.BPP.accuracyLevel != oldParamNodes.at(10).toElement().text()){
                     oldParamNodes.at(10).firstChild().setNodeValue(bearing.BPP.accuracyLevel);
                }
                if(bearing.BPP.life != oldParamNodes.at(11).toElement().text().toFloat()){
                     oldParamNodes.at(11).firstChild().setNodeValue(QString("%1").arg(bearing.BPP.life));
                }
                if(bearing.BPP.rotaryRatio != oldParamNodes.at(12).toElement().text().toFloat()){
                     oldParamNodes.at(12).firstChild().setNodeValue(QString("%1").arg(bearing.BPP.rotaryRatio));
                }
                if(bearing.BPP.heatRate != oldParamNodes.at(13).toElement().text().toFloat()){
                     oldParamNodes.at(13).firstChild().setNodeValue(QString("%1").arg(bearing.BPP.heatRate));
                }
                if(bearing.BOP.inneRingSpeed != oldParamNodes.at(14).toElement().text().toInt()){
                     oldParamNodes.at(14).firstChild().setNodeValue(QString("%1").arg(bearing.BOP.inneRingSpeed));
                }
                if(bearing.BOP.axialLoad != oldParamNodes.at(15).toElement().text().toFloat()){
                     oldParamNodes.at(15).firstChild().setNodeValue(QString("%1").arg(bearing.BOP.axialLoad));
                }
                if(bearing.BOP.radialLoad != oldParamNodes.at(16).toElement().text().toFloat()){
                     oldParamNodes.at(16).firstChild().setNodeValue(QString("%1").arg(bearing.BOP.radialLoad));
                }
                if(bearing.BOP.temperature != oldParamNodes.at(17).toElement().text().toFloat()){
                     oldParamNodes.at(17).firstChild().setNodeValue(QString("%1").arg(bearing.BOP.temperature));
                }
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                    return false;
                }
                QTextStream out(&file);
                out.setCodec("UTF-8");
                doc.save(out, 4);
                file.close();
                return true;
            }
        }
        child = child.nextSibling();
    }
    //若不存在则增加新的轴承参数
    QDomElement bearingElement = doc.createElement("bearing");
    root.appendChild(bearingElement);
    QString strName = "number";
    QString strValue = bearing.number;
    bearingElement.setAttribute(strName, strValue);
    //添加类型
    QDomElement paramElement = doc.createElement("类型");
    QDomText text = doc.createTextNode(bearing.bearingType);
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加品牌
    paramElement = doc.createElement("品牌");
    text = doc.createTextNode(bearing.bearingBrand);
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加内径
    paramElement = doc.createElement("内径");
    text = doc.createTextNode(QString("%1").arg(bearing.BSP.insideDiameter));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加宽度
    paramElement = doc.createElement("宽度");
    text = doc.createTextNode(QString("%1").arg(bearing.BSP.width));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加球数量
    paramElement = doc.createElement("球数量");
    text = doc.createTextNode(QString("%1").arg(bearing.BSP.ballNumber));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加球径
    paramElement = doc.createElement("球径");
    text = doc.createTextNode(QString("%1").arg(bearing.BSP.ballDiameter));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加外沟曲率系数
    paramElement = doc.createElement("外沟曲率系数");
    text = doc.createTextNode(QString("%1").arg(bearing.BSP.outCurvatureCoeff));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加内沟曲率系数
    paramElement = doc.createElement("内沟曲率系数");
    text = doc.createTextNode(QString("%1").arg(bearing.BSP.inCurvatureCoeff));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加预设接触角
    paramElement = doc.createElement("预设接触角");
    text = doc.createTextNode(QString("%1").arg(bearing.BSP.presetContactAngle));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加精度等级
    paramElement = doc.createElement("精度等级");
    text = doc.createTextNode(bearing.BPP.accuracyLevel);
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加寿命
    paramElement = doc.createElement("寿命");
    text = doc.createTextNode(QString("%1").arg(bearing.BPP.life));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加旋滚比
    paramElement = doc.createElement("旋滚比");
    text = doc.createTextNode(QString("%1").arg(bearing.BPP.rotaryRatio));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加发热率
    paramElement = doc.createElement("发热率");
    text = doc.createTextNode(QString("%1").arg(bearing.BPP.heatRate));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加内圈转速
    paramElement = doc.createElement("内圈转速");
    text = doc.createTextNode(QString("%1").arg(bearing.BOP.inneRingSpeed));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加轴向载荷
    paramElement = doc.createElement("轴向载荷");
    text = doc.createTextNode(QString("%1").arg(bearing.BOP.axialLoad));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加径向载荷
    paramElement = doc.createElement("径向载荷");
    text = doc.createTextNode(QString("%1").arg(bearing.BOP.radialLoad));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);
    //添加轴承温度
    paramElement = doc.createElement("轴承温度");
    text = doc.createTextNode(QString("%1").arg(bearing.BOP.temperature));
    paramElement.appendChild(text);
    bearingElement.appendChild(paramElement);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return false;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out, 4);
    file.close();
    return true;
}
QVector<Bearing> XMLParser::getData(){
    return bearingsVec;
}
