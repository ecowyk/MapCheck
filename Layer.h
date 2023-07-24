/***********************************************************************/
/*                                                                     */
/* "Layer.h"                                                           */
/* Description : This file describes layer classes, which have two     */
/* functions: get layer data and check for errors.                     */
/* Started by zhunan, 2023/7/23                                        */
/* Updated by zhunan, 2023/7/24                                        */
/*                                                                     */
/***********************************************************************/
#ifndef __Layer_H__
#define __Layer_H__

#include "CollectError.h"
#include "Functions.h"

//图层类的基类
class Layer
{
public:
	//构造函数
	Layer(){};
	//析构函数
	virtual ~Layer(){};
	//添加函数
	virtual AIBoolean Add(ai::int16 type,MyPath myPath) = 0;
	//获取图层内信息
	virtual ASErr GetPathOfLayer(AIArtHandle path) = 0;
};
class BlackLayer;
class BrownLayer;
//绿色图层
class GreenLayer:public Layer
{
public:
	//构造函数
	GreenLayer(){};
	//析构函数
	~GreenLayer(){};
	//添加函数
	AIBoolean Add(ai::int16 type,MyPath myPath);
	//获取绿色图层的信息
	ASErr GetPathOfLayer(AIArtHandle path);
	//获取绿色多边形大中的路径
	MyPath GetGreenPolyBigPath(ai::int32 index);
	//获取绿色多边形小中的路径
	MyPath GetGreenPolySmallPath(ai::int32 index);
	//获取绿色多边形过小的路径
	MyPath GetGreenPolyTooSmallPath(ai::int32 index);
	//获取绿色圆形中的路径
	MyPath GetGreenCirclePath(ai::int32 index);
	//获取白色其他中的路径
	MyPath GetOtherWhitePath(ai::int32 index);
	//获取绿色多边形大的大小
	ai::int32 GetGreenPolyBigNum();
	//获取绿色多边形小的大小
	ai::int32 GetGreenPolySmallNum();
	//获取绿色多边形过小的大小
	ai::int32 GetGreenPolyTooSmallNum();
	//获取绿色圆形的大小
	ai::int32 GetGreenCircleNum();
	//获取白色其他的大小
	ai::int32 GetOtherWhiteNum();
	//下面为检查错误的函数
	ASErr CheckError1(CollectError& collectError);//
	ASErr CheckError2(CollectError& collectError);//绿色多边形至少要有两个配置符号
	ASErr CheckError3(BlackLayer blackLayer,CollectError &collectError);//面积小于规定值不应有文字注记
private:
	vector<MyPath> greenPolyBig;//特点为无stroke，有fill，颜色为自定义颜色，tint≈0.85，白色填充为1，0为黑色,面积大于规定值，应有文字注记
	vector<MyPath> greenPolySmall;//不应有文字注记
	vector<MyPath> greenPolyTooSmall;//应改为点状
	vector<MyPath> greenCircle;//特点为有stroke，无fill，stroke颜色为地图绿，线宽为0.34
	vector<MyPath> otherWhite;//特点为无stroke，有fill，tint为1

};
//蓝色图层
class BlueLayer:public Layer
{
public:
	BlueLayer(){};
	~BlueLayer(){};
	AIBoolean Add(ai::int16 type,MyPath myPath);
	ASErr GetPathOfLayer(AIArtHandle path);

	MyPath GetBluePolyPath(ai::int32 index);
	MyPath GetBluePolyOutLinePath(ai::int32 index);
	MyPath GetSingleLineRiverPath(ai::int32 index);
	MyPath GetOtherWhitePath(ai::int32 index);
	ai::int32 GetBluePolyNum();
	ai::int32 GetBluePolyOutLineNum();
	ai::int32 GetSingleLineRiverNum();
	ai::int32 GetOtherWhiteNum();

	ASErr CheckError11(BlackLayer blackLayer,BrownLayer brownLayer,CollectError& collectError);
	ASErr CheckError12(BlackLayer blackLayer,CollectError& collectError);
private:
	vector<MyPath> bluePoly;
	vector<MyPath> bluePolyOutline;
	vector<MyPath> singleLineRiver;
	vector<MyPath> otherWhite;
};
//棕色图层
class BrownLayer:public Layer
{
public:
	BrownLayer(){};
	~BrownLayer(){};
	AIBoolean Add(ai::int16 type,MyPath myPath);
	ASErr GetPathOfLayer(AIArtHandle path);

	MyPath GetDarkBrownRoadPath(ai::int32 index);
	MyPath GetLightBrownRoadPath(ai::int32 index);
	MyPath GetContourLinePath(ai::int32 index);
	MyPath GetOtherWhitePath(ai::int32 index);
	ai::int32 GetDarkBrownRoadNum();
	ai::int32 GetLightBrownRoadNum();
	ai::int32 GetContourLineNum();
	ai::int32 GetOtherWhiteNum();

	ASErr CheckError7(BlueLayer blueLayer,CollectError& collectError);//检测等高线遇双线河是否断开
	ASErr CheckError8(BlueLayer blueLayer,CollectError& collectError);//检查等高线是否与河流套合
private:
	vector<MyPath> darkBrownRoad;//深棕色道路   shiftDist=0.2
	vector<MyPath> lightBrownRoad;//浅棕色道路   shiftDist = 0.6
	vector<MyPath> contourLine;//等高线    shiftDist = 0
	vector<MyPath> otherWhite;
};
//黑色图层
class BlackLayer:public Layer
{
public:
	BlackLayer(){};
	~BlackLayer(){};
	AIBoolean Add(ai::int16 type,MyPath myPath);
	//下面三个函数均为获取函数服务
	ASErr GetPathOfLayer(AIArtHandle path);
	void Classify(vector<MyPath> paths);
	AIBoolean JudgeLineIntersectCircle(MyPath path,MyPath pole);

	MyPath GetBlackTextPath(ai::int32 index);
	MyPath GetBlackPolePath(ai::int32 index);
	MyPath GetBlackDigitalPath(ai::int32 index);
	MyPath GetBlackMainRoadPath(ai::int32 index);
	MyPath GetBlackByPassRoadPath(ai::int32 index);
	MyPath GetBlackSingleLineRoadPath(ai::int32 index);
	MyPath GetWireDirecctionPath(ai::int32 index);
	MyPath GetWirePath(ai::int32 index);
	MyPath GetBridge_1Path(ai::int32 index);
	MyPath GetBridge_2Path(ai::int32 index);
	MyPath GetStonePath(ai::int32 index);
	MyPath GetOtherWhitePath(ai::int32 index);
	ai::int32 GetBlackTextNum();
	ai::int32 GetBlackPoleNum();
	ai::int32 GetBlackDigitalNum();
	ai::int32 GetBlackMainRoadNum();
	ai::int32 GetBlackByPassRoadNum();
	ai::int32 GetBlackSingleLineRoadNum();
	ai::int32 GetWireDirecctionNum();
	ai::int32 GetWireNum();
	ai::int32 GetBridge_1Num();
	ai::int32 GetBridge_2Num();
	ai::int32 GetStoneNum();
	ai::int32 GetOtherWhiteNum();

	ASErr CheckError4(GreenLayer greenLayer,CollectError& collectError);
	ASErr CheckError5(CollectError& collectError);
	ASErr CheckError6(BlueLayer blueLayer,CollectError& collectError);
	ASErr CheckError9(CollectError& collectError);
	ASErr CheckError10(CollectError& collectError);
	ASErr CheckError13(CollectError& collectError);
private:
	vector<MyPath> blackText;//fill为黑色，无stroke
	vector<MyPath> blackPole;//fill为黑色，无stroke，直径为0.4mm
	vector<MyPath> blackDigital;//fill为黑色，stroke也为黑色
	vector<MyPath> blackMainRoad;//无fill，stroke为黑色，线宽0.34
	vector<MyPath> blackByPassRoad;//无fill，stroke为黑色，线宽0.28
	vector<MyPath> blackSingleLineRoad;//无fill，stroke为黑色，线宽0.5669
	vector<MyPath> wireDirecction;//
	vector<MyPath> wire;//
	vector<MyPath> bridge_1;//桥梁
	vector<MyPath> bridge_2;
	vector<MyPath> stone;//石头？
	vector<MyPath> otherWhite;
};
#endif