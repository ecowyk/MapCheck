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

//ͼ����Ļ���
class Layer
{
public:
	//���캯��
	Layer(){};
	//��������
	virtual ~Layer(){};
	//��Ӻ���
	virtual AIBoolean Add(ai::int16 type,MyPath myPath) = 0;
	//��ȡͼ������Ϣ
	virtual ASErr GetPathOfLayer(AIArtHandle path) = 0;
};
class BlackLayer;
class BrownLayer;
//��ɫͼ��
class GreenLayer:public Layer
{
public:
	//���캯��
	GreenLayer(){};
	//��������
	~GreenLayer(){};
	//��Ӻ���
	AIBoolean Add(ai::int16 type,MyPath myPath);
	//��ȡ��ɫͼ�����Ϣ
	ASErr GetPathOfLayer(AIArtHandle path);
	//��ȡ��ɫ����δ��е�·��
	MyPath GetGreenPolyBigPath(ai::int32 index);
	//��ȡ��ɫ�����С�е�·��
	MyPath GetGreenPolySmallPath(ai::int32 index);
	//��ȡ��ɫ����ι�С��·��
	MyPath GetGreenPolyTooSmallPath(ai::int32 index);
	//��ȡ��ɫԲ���е�·��
	MyPath GetGreenCirclePath(ai::int32 index);
	//��ȡ��ɫ�����е�·��
	MyPath GetOtherWhitePath(ai::int32 index);
	//��ȡ��ɫ����δ�Ĵ�С
	ai::int32 GetGreenPolyBigNum();
	//��ȡ��ɫ�����С�Ĵ�С
	ai::int32 GetGreenPolySmallNum();
	//��ȡ��ɫ����ι�С�Ĵ�С
	ai::int32 GetGreenPolyTooSmallNum();
	//��ȡ��ɫԲ�εĴ�С
	ai::int32 GetGreenCircleNum();
	//��ȡ��ɫ�����Ĵ�С
	ai::int32 GetOtherWhiteNum();
	//����Ϊ������ĺ���
	ASErr CheckError1(CollectError& collectError);//
	ASErr CheckError2(CollectError& collectError);//��ɫ���������Ҫ���������÷���
	ASErr CheckError3(BlackLayer blackLayer,CollectError &collectError);//���С�ڹ涨ֵ��Ӧ������ע��
private:
	vector<MyPath> greenPolyBig;//�ص�Ϊ��stroke����fill����ɫΪ�Զ�����ɫ��tint��0.85����ɫ���Ϊ1��0Ϊ��ɫ,������ڹ涨ֵ��Ӧ������ע��
	vector<MyPath> greenPolySmall;//��Ӧ������ע��
	vector<MyPath> greenPolyTooSmall;//Ӧ��Ϊ��״
	vector<MyPath> greenCircle;//�ص�Ϊ��stroke����fill��stroke��ɫΪ��ͼ�̣��߿�Ϊ0.34
	vector<MyPath> otherWhite;//�ص�Ϊ��stroke����fill��tintΪ1

};
//��ɫͼ��
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
//��ɫͼ��
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

	ASErr CheckError7(BlueLayer blueLayer,CollectError& collectError);//���ȸ�����˫�ߺ��Ƿ�Ͽ�
	ASErr CheckError8(BlueLayer blueLayer,CollectError& collectError);//���ȸ����Ƿ�������׺�
private:
	vector<MyPath> darkBrownRoad;//����ɫ��·   shiftDist=0.2
	vector<MyPath> lightBrownRoad;//ǳ��ɫ��·   shiftDist = 0.6
	vector<MyPath> contourLine;//�ȸ���    shiftDist = 0
	vector<MyPath> otherWhite;
};
//��ɫͼ��
class BlackLayer:public Layer
{
public:
	BlackLayer(){};
	~BlackLayer(){};
	AIBoolean Add(ai::int16 type,MyPath myPath);
	//��������������Ϊ��ȡ��������
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
	vector<MyPath> blackText;//fillΪ��ɫ����stroke
	vector<MyPath> blackPole;//fillΪ��ɫ����stroke��ֱ��Ϊ0.4mm
	vector<MyPath> blackDigital;//fillΪ��ɫ��strokeҲΪ��ɫ
	vector<MyPath> blackMainRoad;//��fill��strokeΪ��ɫ���߿�0.34
	vector<MyPath> blackByPassRoad;//��fill��strokeΪ��ɫ���߿�0.28
	vector<MyPath> blackSingleLineRoad;//��fill��strokeΪ��ɫ���߿�0.5669
	vector<MyPath> wireDirecction;//
	vector<MyPath> wire;//
	vector<MyPath> bridge_1;//����
	vector<MyPath> bridge_2;
	vector<MyPath> stone;//ʯͷ��
	vector<MyPath> otherWhite;
};
#endif