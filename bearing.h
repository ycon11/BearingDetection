#ifndef BEARING_H
#define BEARING_H
#include<QString>
#include<QVector>
#include<QXmlStreamReader>
#include<QXmlStreamWriter>
#include<QDomElement>
#include<QDomNode>
#include<QStringList>
#include<QFile>
#include<typeinfo>
#include<QDebug>

//#define  NUMBER  0
//#define  TYPE    1
//#define  BRAND   2
//#define  INSIDE_DIAMETER      3
//#define  OUTSIDE_DIAMETER     4
//#define  WIDTH                5
//#define  BALL_NUMBER          6
//#define  BALL_DIAMETER        7
//#define  OUT_CURVATURE_COEFF  8
//#define  IN_CURVATURE_COEFF   9
//#define  PRESET_CONTACT_ANGLE 10
//#define  ACCURACY_LEVEL       11
//#define  LIFE                 12
//#define  ROTARY_RATIO         13
//#define  HEAT_RATE            14
//#define  INNERING_SPEED       15
//#define  AXIAL_LOAD           16
//#define  RADIAL_LOAD          17
//#define  TEMPERATURE          18

class  BearingSizeParam{
public:
    BearingSizeParam();
    BearingSizeParam &operator=(const BearingSizeParam &bsp);
    void clear();
public:
   float insideDiameter;                //轴承内径  单位：mm
   float outsideDiameter;               //轴承外径  单位：mm
   float width;                         //轴承宽度  单位：mm
   int ballNumber;                      //轴承球的数量
   float ballDiameter;                  //球径     单位：mm
   float inCurvatureCoeff;              //内沟曲率系数
   float outCurvatureCoeff;             //外沟曲率系数
   float presetContactAngle;            //预设接触角
   float actualContactAngle;            //实际接触角
};
class BearingPerformParam{
public:
    BearingPerformParam();
    BearingPerformParam &operator=(const BearingPerformParam &bsp);
     void clear();
public:
    QString accuracyLevel;                        //精度等级
    float  life;                                 //使用寿命  单位：h
    float rotaryRatio;                           //旋滚比
    float stressLevel;                           //应力水平  单位:pa
    float heatRate;                              //发热率    单位：W
};
class BearingOperatingParam{
public:
    BearingOperatingParam();
    BearingOperatingParam &operator=(const BearingOperatingParam &bop);
     void clear();
public:
    int inneRingSpeed;                           //内圈参数   单位：r/min
    float axialLoad;                             //轴向载荷   单位：N
    float radialLoad;                            //径向载荷   单位：N
    float temperature;                           //轴承温度   单位：C
                                                 //润滑油
};
class Bearing{
public:
    Bearing();
    Bearing(const QString &num);
    void clear();
    Bearing& operator=(const Bearing &bearing);
public:
    QString bearingBrand;
    QString bearingType;
    QString number;
    BearingSizeParam BSP;
    BearingPerformParam BPP;
    BearingOperatingParam BOP;
};


extern QVector<QString> bearingBrand;
extern QVector<QString> bearingType;



class XMLParser{
public:
    XMLParser() =default;
    bool readBearingData(const QString &fileName);
    bool writeBearingData(const QString &fileName,const Bearing  &bearing);
    QVector<Bearing> getData();
private:
    void parserBearingsMembers(const QDomElement &element);
    void parserBearing(const QDomElement &element);
    QVector<Bearing> bearingsVec;
};

#endif // BEARING_H

