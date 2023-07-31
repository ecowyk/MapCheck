/***********************************************************************/
/*                                                                     */
/* "CollectError.h"                                                    */
/* Description : This file is used to describe the error collection    */
/* class.                                                              */
/* Started by zhunan, 2023/7/23                                        */
/* Updated by zhunan, 2023/7/31                                        */
/*                                                                     */
/***********************************************************************/
#ifndef __CollectError_H__
#define __CollectError_H__

#include "MapCheckSuites.h"
#include "Structs.h"
#include "Functions.h"
#include <vector>
#include <map>
#include <iostream>

const ai::int32 ErrorNumMax = 100;
const ai::int32 ErrorTypeNum = 14;

using namespace std;
class CollectError
{
public:
	//���캯��
	CollectError()
	{
		ErrorNum = 0;
		ErrorTypeList.insert(make_pair(1,"���С��25ƽ�����ף��ĵ�״"));
		ErrorTypeList.insert(make_pair(2,"����������������"));
		ErrorTypeList.insert(make_pair(3,"С��4ƽ�����׵Ĳ�������ע��"));
		ErrorTypeList.insert(make_pair(4,"��ɫ�����ѹ��·��"));
		ErrorTypeList.insert(make_pair(5,"�����߰������Ӧ����"));
		ErrorTypeList.insert(make_pair(6,"ˮѹ�ڵ�����"));
		ErrorTypeList.insert(make_pair(7,"�ȸ�����˫�ߺ�Ӧ�Ͽ�"));
		ErrorTypeList.insert(make_pair(8,"�ȸ���������׺�"));
		ErrorTypeList.insert(make_pair(9,"��·ѹ�ǹ�ϵ����ȷ-1"));
		ErrorTypeList.insert(make_pair(10,"��·ѹ�ǹ�ϵ����ȷ-2"));
		ErrorTypeList.insert(make_pair(11,"��ѹ��·���Ƿ�������-1"));
		ErrorTypeList.insert(make_pair(12,"��ѹ��·���Ƿ�������-2"));
		ErrorTypeList.insert(make_pair(13,"�ذ�"));
		ErrorTypeList.insert(make_pair(14,"�ȸ��߼��Ӧ����0.2mm"));
	}
	//��������
	~CollectError(){};
	//��Ӵ���
	void AddError(ai::int32 errorType,ai::int32 layerOrdinalNum,ai::int32 artOrdinalNum)
	{
		/*
		���С��25ƽ�����ף��ĵ�״      1
		����������������                2
		С��4ƽ�����׵Ĳ�������ע��     3
		��ɫ�����ѹ��·��              4
		�����߰������Ӧ����            5
		ˮѹ�ڵ�����                    6
		�ȸ�����˫�ߺ�Ӧ�Ͽ�            7
		�ȸ���������׺�                8
		��·ѹ�ǹ�ϵ����ȷ-1            9
		��·ѹ�ǹ�ϵ����ȷ-2            10
		��ѹ��·���Ƿ�������-1          11
		��ѹ��·���Ƿ�������-2          12
		�ذ�                            13
		�ȸ��߼��                      14

		*/
		ErrorNum++;
		ErrorInfo errorInfo = {errorType,layerOrdinalNum,artOrdinalNum};
		this->errors.push_back(errorInfo);
	}
	//��ӡ����
	void PrintErrorInfo()
	{
		ai::UnicodeString allInfo=ai::UnicodeString("");
		for(ai::int32 i = 0;i<ErrorNum;i++)
		{
			ai::UnicodeString a;
			ai::UnicodeString b;
			ai::UnicodeString c;
			sAIUser->AIRealToString(errors[i].errorType,0,a);
			sAIUser->AIRealToString(errors[i].layerOrdinalNum,0,b);
			sAIUser->AIRealToString(errors[i].artOrdinalNum,0,c);
			allInfo.append(a);
			allInfo.append(ai::UnicodeString(","));
			allInfo.append(b);
			allInfo.append(ai::UnicodeString(","));
			allInfo.append(c);
			allInfo.append(ai::UnicodeString("\n"));
		}
		sAIUser->MessageAlert(allInfo);
	}
	//��ʾ����
	ASErr SetErrorRedAndCenter(ai::int32 errorIndex)
    {
	    ASErr error = kNoErr;

	    AILayerList list;//ͼ���б�
	    AILayerHandle layer;//ͼ��
	    AIArtHandle path;//·��
	    AIPathStyle style;//·����ʽ

	    error = sAILayerList->GetFirst(&list);//��ȡ��һ��ͼ���б�
	    error = sAILayerList->GetFirstLayer(list,&layer);//��ȡ��һ��ͼ��

		ai::int32 errorLayerIndex = this->errors[errorIndex].layerOrdinalNum;
		ai::int32 errorArtIndex = this->errors[errorIndex].artOrdinalNum;
		for(ai::int32 i = 0;i<errorLayerIndex;i++)//��λͼ��
		{
			error = sAILayerList->GetNextLayer(list,layer,&layer);
		}
		error = sAIArt->GetFirstArtOfLayer(layer,&path);//��ȡ��ͼ��ĵ�һ��ͼԪ
		error = sAIArt->GetArtFirstChild(path,&path);//��ȡ��һ��·��
		for(ai::int32 i = 0;i<errorArtIndex;i++)//��λ����λ��
		{
			error = sAIArt->GetArtSibling(path,&path);
		}
		//��ɫ������ʾ
		error = sAIPathStyle->GetPathStyle(path,&style);
		style.strokePaint = true;
		style.stroke.color.kind =kFourColor;
		style.stroke.color.c.f.cyan = 0;
		style.stroke.color.c.f.magenta = 100;
		style.stroke.color.c.f.yellow = 100;
		style.stroke.color.c.f.black = 0;
        error = sAIPathStyle->SetPathStyle(path,&style);
		AIRealPoint center;
		GetPathCenter(path,center);
		error = sAIDocumentView->SetDocumentViewCenter(NULL,&center);
		return error;
    }

public:
	vector<ErrorInfo> errors;//��������
	map<ai::int32,string> ErrorTypeList;
	ai::int32 ErrorNum;//��������
};
#endif