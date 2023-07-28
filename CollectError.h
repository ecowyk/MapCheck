/***********************************************************************/
/*                                                                     */
/* "CollectError.h"                                                    */
/* Description : This file is used to describe the error collection    */
/* class.                                                              */
/* Started by zhunan, 2023/7/23                                        */
/* Updated by zhunan, 2023/7/24                                        */
/*                                                                     */
/***********************************************************************/
#ifndef __CollectError_H__
#define __CollectError_H__

#include "MapCheckSuites.h"
#include "Structs.h"
#include <vector>
#include <map>
#include <iostream>

const ai::int32 ErrorNumMax = 100;
const ai::int32 ErrorNumPerPage = 10;
const ai::int32 ErrorTypeNum = 14;

using namespace std;
class CollectError
{
public:
	//构造函数
	CollectError()
	{
		ErrorNum = 0;
		CurPage = 0;
		PageNum = 0;
		ErrorTypeList.insert(make_pair(1,"面积小于25平方毫米，改点状"));
		ErrorTypeList.insert(make_pair(2,"至少配置两个符号"));
		ErrorTypeList.insert(make_pair(3,"小于4平方厘米的不加性质注记"));
		ErrorTypeList.insert(make_pair(4,"绿色多边形压在路上"));
		ErrorTypeList.insert(make_pair(5,"电力线半个符号应舍弃"));
		ErrorTypeList.insert(make_pair(6,"水压在电线上"));
		ErrorTypeList.insert(make_pair(7,"等高线遇双线河应断开"));
		ErrorTypeList.insert(make_pair(8,"等高线与河流套合"));
		ErrorTypeList.insert(make_pair(9,"道路压盖关系不正确-1"));
		ErrorTypeList.insert(make_pair(10,"道路压盖关系不正确-2"));
		ErrorTypeList.insert(make_pair(11,"河压在路上是否有桥梁-1"));
		ErrorTypeList.insert(make_pair(12,"河压在路上是否有桥梁-2"));
		ErrorTypeList.insert(make_pair(13,"重版"));
		ErrorTypeList.insert(make_pair(14,"等高线间隔应大于0.2mm"));
	}
	//析构函数
	~CollectError(){};
	//添加错误
	void AddError(ai::int32 errorType,ai::int32 layerOrdinalNum,ai::int32 artOrdinalNum)
	{
		/*
		面积小于25平方毫米，改点状      1
		至少配置两个符号                2
		小于4平方厘米的不加性质注记     3
		绿色多边形压在路上              4
		电力线半个符号应舍弃            5
		水压在电线上                    6
		等高线遇双线河应断开            7
		等高线与河流套合                8
		道路压盖关系不正确-1            9
		道路压盖关系不正确-2            10
		河压在路上是否有桥梁-1          11
		河压在路上是否有桥梁-2          12
		重版                           13
     		等高线间隔                     14
		*/
		ErrorNum++;
		ErrorInfo errorInfo = {errorType,layerOrdinalNum,artOrdinalNum};
		this->errors.push_back(errorInfo);
	}
	//打印错误
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
	//显示错误
	void SetErrorRed()
    {
	    ASErr error = kNoErr;
 
	    ai::int32 layerListCount;//图层列表数
	    ai::int32 layerCount;//一个图层列表中的图层数

	    AILayerList list;//图层列表
	    AILayerHandle layer;//图层

	    AIArtHandle path;//路径
	    AIPathStyle style;//路径样式
	    ai::int32 curLayerNum = 0;
	    //测试各种api
	    error = sAILayerList->Count(&layerListCount);//获取图层列表数量
	    error = sAILayerList->GetFirst(&list);//获取第一个图层列表
	    error = sAILayerList->CountLayers(list,&layerCount);//获取该图层列表有多少个图层
	    error = sAILayerList->GetFirstLayer(list,&layer);//获取第一个图层

	    while(layer)
	    {
		    error = sAIArt->GetFirstArtOfLayer(layer,&path);//获取该图层的第一个图元
		    error = sAIArt->GetArtFirstChild(path,&path);//获取第一条路径
	    	ai::int32 curArtNum = 0;
	    	while(path)
	    	{
		    	for(ai::int32 i=0;i<this->ErrorNum;i++)
		    	{
		    		if(this->errors[i].layerOrdinalNum==curLayerNum&&this->errors[i].artOrdinalNum==curArtNum)
			    	{
				    	error = sAIPathStyle->GetPathStyle(path,&style);
						style.strokePaint = true;
						style.stroke.color.kind =kFourColor;
						style.stroke.color.c.f.cyan = 0;
						style.stroke.color.c.f.magenta = 100;
						style.stroke.color.c.f.yellow = 100;
						style.stroke.color.c.f.black = 0;
					
					    error = sAIPathStyle->SetPathStyle(path,&style);
					    this->errors.erase(this->errors.begin()+i);
					    i--;
					    this->ErrorNum--;
				    }
		    	}
		    	curArtNum++;
		    	error = sAIArt->GetArtSibling(path,&path);
		    }
	    	curLayerNum++;
	    	error = sAILayerList->GetNextLayer(list,layer,&layer);
     	}

    }
public:
	vector<ErrorInfo> errors;//错误数组
	map<ai::int32,string> ErrorTypeList;
	ai::int32 ErrorNum;//错误数量
	ai::int32 CurPage;
	ai::int32 PageNum;
};
#endif
