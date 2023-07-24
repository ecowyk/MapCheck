#include "IllustratorSDK.h"
#include "MapCheckSuites.h"
#include "Layer.h"
AIBoolean GreenLayer::Add(ai::int16 type,MyPath myPath)
{
	//这是一条测试注释
	switch(type)
	{
	case 1:this->greenPolyBig.push_back(myPath);return true;break;
	case 2:this->greenPolySmall.push_back(myPath);return true;break;
	case 3:this->greenPolyTooSmall.push_back(myPath);return true;break;
	case 4:this->greenCircle.push_back(myPath);return true;break;
	case 5:this->otherWhite.push_back(myPath);return true;break;
	default:return false;
	}
}
ASErr GreenLayer::GetPathOfLayer(AIArtHandle path)
{
	ASErr error = kNoErr;

	ai::int32 curArtNum = 0;
	ai::int32 curLayerNum = 0;

	AIReal area;
	MyPath myPath;
	myPath.layerOrdinalNum = curLayerNum;
	AIPathStyle style;
	while(path)
	{
		if(curArtNum<8)
		{
			sAIArt->GetArtSibling(path,&path);
		    curArtNum++;
			continue;
		}
		myPath.artOrdinalNum = curArtNum;
		error = sAIPathStyle->GetPathStyle(path,&style);//获取style
		sAIPath->GetPathSegmentCount(path,&myPath.segmentNum);
		sAIPath->GetPathSegments(path,0,myPath.segmentNum,myPath.segment);
		sAIPath->GetPathArea(path,&myPath.area);
		if(style.fillPaint==1&&style.strokePaint==0)//判断是否为多边形
		{
			if(style.fill.color.c.c.tint==1)//区分grennPoly和otherWhite
			{
				this->Add(5,myPath);
			}
			else
			{
				error = sAIPath->GetPathArea(path,&area);//获取绿色多边形面积
			    area = fabs(area);
				if(area<areaCheckToPoint)//面积过小改点状
					this->Add(3,myPath);
				else if(area>=areaCheckToPoint&&area<areaCheckIfAnnotation)//需检查是否加注记（不应加）
				{
					this->Add(2,myPath);
				}
				else
				{
					this->Add(1,myPath);
				}
			}

		}
		else if(style.fillPaint==0&&style.strokePaint==1)
		{
			this->Add(4,myPath);
		}
		//获取下一个
		sAIArt->GetArtSibling(path,&path);
		curArtNum++;
	}
	return error;
}
MyPath GreenLayer::GetGreenPolyBigPath(ai::int32 index)
{
	if(index<0||index>=this->greenPolyBig.size())
		throw "GetGreenPolyBigPath";
	else
		return this->greenPolyBig[index];
}
MyPath GreenLayer::GetGreenPolySmallPath(ai::int32 index)
{
	if(index<0||index>=this->greenPolySmall.size())
		throw "GetGreenPolySmallPath";
	else
		return this->greenPolySmall[index];
}
MyPath GreenLayer::GetGreenPolyTooSmallPath(ai::int32 index)
{
	if(index<0||index>=this->greenPolyTooSmall.size())
		throw "GetGreenPolyTooSmallPath";
	else
		return this->greenPolyTooSmall[index];
}
MyPath GreenLayer::GetGreenCirclePath(ai::int32 index)
{
	if(index<0||index>=this->greenCircle.size())
		throw "GetGreenCirclePath";
	else
		return this->greenCircle[index];
}
MyPath GreenLayer::GetOtherWhitePath(ai::int32 index)
{
	if(index<0||index>=this->otherWhite.size())
		throw "GetOtherWhitePath";
	else
		return this->otherWhite[index];
}
ai::int32 GreenLayer::GetGreenPolyBigNum()
{
	return this->greenPolyBig.size();
}
ai::int32 GreenLayer::GetGreenPolySmallNum()
{
	return this->greenPolySmall.size();
}
ai::int32 GreenLayer::GetGreenPolyTooSmallNum()
{
	return this->greenPolyTooSmall.size();
}
ai::int32 GreenLayer::GetGreenCircleNum()
{
	return this->greenCircle.size();
}
ai::int32 GreenLayer::GetOtherWhiteNum()
{
	return this->otherWhite.size();
}
ASErr GreenLayer::CheckError1(CollectError& collectError)
{
	ASErr error = kNoErr;
	for(ai::int32 i = 0;i<this->greenPolyTooSmall.size();i++)
	{
		collectError.AddError(1,this->greenPolyTooSmall[i].layerOrdinalNum,this->greenPolyTooSmall[i].artOrdinalNum);
	}
	return error;
}
ASErr GreenLayer::CheckError2(CollectError &collectError)
{
	ASErr error = kNoErr;
	for(ai::int32 i=0;i<this->greenPolyBig.size();i++)
	{
		ai::int32 symbolNum = 0;
		for(ai::int32 j=0;j<this->greenCircle.size();j++)
		{
			if(JudgePath1InsidePath2(this->greenCircle[j],this->greenPolyBig[i]))
				symbolNum++;
		}
		if(symbolNum<2)
			collectError.AddError(2,this->greenPolyBig[i].layerOrdinalNum,this->greenPolyBig[i].artOrdinalNum);
	}
	for(ai::int32 i=0;i<this->greenPolySmall.size();i++)
	{
		ai::int32 symbolNum = 0;
		for(ai::int32 j=0;j<this->greenCircle.size();j++)
		{
			if(JudgePath1InsidePath2(this->greenCircle[j],this->greenPolySmall[i]))
				symbolNum++;
		}
		if(symbolNum<2)
			collectError.AddError(2,this->greenPolySmall[i].layerOrdinalNum,this->greenPolySmall[i].artOrdinalNum);
	}
	return error;
}
ASErr GreenLayer::CheckError3(BlackLayer blackLayer,CollectError &collectError)
{
	ASErr error = kNoErr;
	for(ai::int32 i = 0; i <this->greenPolySmall.size();i++)
	{
		for(ai::int32 j = 0;j<blackLayer.GetBlackTextNum();j++)
		{
			if(JudgePath1InsidePath2(blackLayer.GetBlackTextPath(j),this->greenPolySmall[i]))
			{
				collectError.AddError(3,this->greenPolySmall[i].layerOrdinalNum,this->greenPolySmall[i].artOrdinalNum);
				break;
			}
		}
	}
	return error;
}

AIBoolean BlueLayer::Add(ai::int16 type,MyPath myPath)
{
	switch(type)
	{
	case 1:this->bluePoly.push_back(myPath);return true;break;
	case 2:this->bluePolyOutline.push_back(myPath);return true;break;
	case 3:this->singleLineRiver.push_back(myPath);return true;break;
	case 4:this->otherWhite.push_back(myPath);return true;break;
	default:return false;
	}
}
ASErr BlueLayer::GetPathOfLayer(AIArtHandle path)
{
	ASErr error = kNoErr;

	ai::int32 curArtNum = 0;
	ai::int32 curLayerNum = 1;
	

	MyPath myPath;
	myPath.layerOrdinalNum = curLayerNum;
	AIPathStyle style;
	while(path)
	{
		if(curArtNum<8)
		{
			sAIArt->GetArtSibling(path,&path);
		    curArtNum++;
			continue;
		}
		myPath.artOrdinalNum = curArtNum;
		error = sAIPathStyle->GetPathStyle(path,&style);
		sAIPath->GetPathSegmentCount(path,&myPath.segmentNum);
	    sAIPath->GetPathSegments(path,0,myPath.segmentNum,myPath.segment);
		sAIPath->GetPathArea(path,&myPath.area);
		if(style.strokePaint==0&&style.fillPaint==1)
		{
			if(style.fill.color.c.c.tint==1)
				this->Add(4,myPath);
			else
				this->Add(1,myPath);
		}
		else if(style.strokePaint==1&&style.fillPaint==0&&style.stroke.color.c.p.shiftDist==0)
		{

			if(myPath.segment[0].p.h==myPath.segment[myPath.segmentNum-1].p.h&&
				myPath.segment[0].p.v==myPath.segment[myPath.segmentNum-1].p.v)
				this->Add(2,myPath);
			else
				this->Add(3,myPath);
		}
		else if(style.strokePaint==1&&style.fillPaint==0&&style.stroke.color.c.p.shiftDist==1&&myPath.segmentNum==2)
		{
			this->Add(4,myPath);
		}
		sAIArt->GetArtSibling(path,&path);
		curArtNum++;
	}
	return error;
}
MyPath BlueLayer::GetBluePolyPath(ai::int32 index)
{
	if(index<0||index>=this->bluePoly.size())
		throw "GetBluePolyPath";
	else
		return this->bluePoly[index];
}
MyPath BlueLayer::GetBluePolyOutLinePath(ai::int32 index)
{
	if(index<0||index>=this->bluePolyOutline.size())
		throw "GetBluePolyOutlinePath";
	else
		return this->bluePolyOutline[index];
}
MyPath BlueLayer::GetSingleLineRiverPath(ai::int32 index)
{
	if(index<0||index>=this->singleLineRiver.size())
		throw "GetSingleLineRiverPath";
	else
		return this->singleLineRiver[index];
}
MyPath BlueLayer::GetOtherWhitePath(ai::int32 index)
{
	if(index<0||index>=this->otherWhite.size())
		throw "GetOtherWhitePath";
	else
		return this->otherWhite[index];
}
ai::int32 BlueLayer::GetBluePolyNum()
{
	return this->bluePoly.size();
}
ai::int32 BlueLayer::GetBluePolyOutLineNum()
{
	return this->bluePolyOutline.size();
}
ai::int32 BlueLayer::GetSingleLineRiverNum()
{
	return this->singleLineRiver.size();
}
ai::int32 BlueLayer::GetOtherWhiteNum()
{
	return this->otherWhite.size();
}
ASErr BlueLayer::CheckError11(BlackLayer blackLayer,BrownLayer brownLayer,CollectError& collectError)
{
	ASErr error = kNoErr;
	//先匹配桥梁
	vector<MyPath> bridges;//bridge_1
	for(ai::int32 i = 0;i<blackLayer.GetBridge_1Num();i++)
	{
		for(ai::int32 j = 0;j<this->otherWhite.size();j++)
		{
			if(OtherWhiteMatching(blackLayer.GetBridge_1Path(i),this->otherWhite[j]))
			{
				bridges.push_back(this->otherWhite[j]);
			}
		}
	}
	//路与河相交判断并检查桥梁
	for(ai::int32 i = 0;i<brownLayer.GetDarkBrownRoadNum();i++)
	{
		for(ai::int32 j = 0;j<this->singleLineRiver.size();j++)
		{
			ai::int32 count = 0;
			if(JudgePathIntersectPath(brownLayer.GetDarkBrownRoadPath(i),this->singleLineRiver[j],count))
			{
				for(ai::int32 k = 0;k<bridges.size();k++)
				{
					if(JudgePathIntersectPath(this->singleLineRiver[j],bridges[k])&&
						this->singleLineRiver[j].artOrdinalNum>bridges[k].artOrdinalNum)
					{
						count--;
					}
				}
				if(count!=0)
				{
					collectError.AddError(11,this->singleLineRiver[j].layerOrdinalNum,this->singleLineRiver[j].artOrdinalNum);
				}
			}
		}
	}
	return error;
}
ASErr BlueLayer::CheckError12(BlackLayer blackLayer,CollectError& collectError)
{
	ASErr error = kNoErr;
	//先桥梁匹配
	vector<MyPath> bridges;
	for(ai::int32 i = 0;i<blackLayer.GetBridge_2Num();i++)
	{
		for(ai::int32 j = 0;j<this->otherWhite.size();j++)
		{
			if(OtherWhiteMatching2(blackLayer.GetBridge_2Path(i),this->otherWhite[j]))
			{
				bridges.push_back(this->otherWhite[j]);
			}
		}
	}
	//路和河的相交判断并检查桥梁
	for(ai::int32 i = 0;i<this->bluePolyOutline.size();i++)
	{
		for(ai::int32 j = 0;j<blackLayer.GetBlackByPassRoadNum();j++)
		{
			ai::int32 count = 0;
			if(JudgePathIntersectPath(this->bluePolyOutline[i],blackLayer.GetBlackByPassRoadPath(j),count))
			{
				count /=2;
				for(ai::int32 k = 0;k<bridges.size();k++)
				{
					if(JudgePathIntersectPath(this->bluePolyOutline[i],bridges[k])&&
						this->bluePolyOutline[i].artOrdinalNum>bridges[k].artOrdinalNum)
					{
						count--;
					}
				}
				if(count!=0)
				{
					collectError.AddError(12,this->bluePolyOutline[i].layerOrdinalNum,this->bluePolyOutline[i].artOrdinalNum);
				}
			}
		}
	}
	return error;
}

AIBoolean BrownLayer::Add(ai::int16 type,MyPath myPath)
{
	switch(type)
	{
	case 1:this->darkBrownRoad.push_back(myPath);return true;break;
	case 2:this->lightBrownRoad.push_back(myPath);return true;break;
	case 3:this->contourLine.push_back(myPath);return true;break;
	case 4:this->otherWhite.push_back(myPath);return true;break;
	default:return false;
	}
}
ASErr BrownLayer::GetPathOfLayer(AIArtHandle path)
{
	ASErr error = kNoErr;

	ai::int32 curArtNum = 0;
	ai::int32 curLayerNum = 2;

	MyPath myPath;
	myPath.layerOrdinalNum = curLayerNum;
	AIPathStyle style;

	while(path)
	{
		if(curArtNum<8)
		{
			sAIArt->GetArtSibling(path,&path);
		    curArtNum++;
			continue;
		}
		myPath.artOrdinalNum = curArtNum;
		error = sAIPathStyle->GetPathStyle(path,&style);
		sAIPath->GetPathSegmentCount(path,&myPath.segmentNum);
	    sAIPath->GetPathSegments(path,0,myPath.segmentNum,myPath.segment);
		sAIPath->GetPathArea(path,&myPath.area);
		if(style.strokePaint==1&&style.fillPaint == 0&&style.stroke.color.c.p.shiftDist<1)
		{
			myPath.lineWidth = style.stroke.width;
			if(style.stroke.color.c.p.shiftDist==0)//等高线
	    	{
			    this->Add(3,myPath);
		    }
			else if(style.stroke.color.c.p.shiftDist>=0.2&&style.stroke.color.c.p.shiftDist<=0.21)//深棕色道路
		    {
			    this->Add(1,myPath);
		    }
		    else if(style.stroke.color.c.p.shiftDist>=0.6&&style.stroke.color.c.p.shiftDist<=0.61)//浅棕色道路
		    {
			    this->Add(2,myPath);
		    }
		}
		else if(style.strokePaint==0&&style.fillPaint == 1)
		{
			this->Add(4,myPath);
		}
		sAIArt->GetArtSibling(path,&path);
		curArtNum++;
	}
	return error;
}
MyPath BrownLayer::GetDarkBrownRoadPath(ai::int32 index)
{
	if(index<0||index>=this->darkBrownRoad.size())
		throw "GetDarkBrownRoadPath";
	else
		return this->darkBrownRoad[index];
}
MyPath BrownLayer::GetLightBrownRoadPath(ai::int32 index)
{
	if(index<0||index>=this->lightBrownRoad.size())
		throw "GetLightBrownRoadPath";
	else
		return this->lightBrownRoad[index];
}
MyPath BrownLayer::GetContourLinePath(ai::int32 index)
{
	if(index<0||index>=this->contourLine.size())
		throw "GetContourLinePath";
	else
		return this->contourLine[index];
}
MyPath BrownLayer::GetOtherWhitePath(ai::int32 index)
{
	if(index<0||index>=this->otherWhite.size())
		throw "GetOtherWhitePath";
	else
		return this->otherWhite[index];
}
ai::int32 BrownLayer::GetDarkBrownRoadNum()
{
	return this->darkBrownRoad.size();
}
ai::int32 BrownLayer::GetLightBrownRoadNum()
{
	return this->lightBrownRoad.size();
}
ai::int32 BrownLayer::GetContourLineNum()
{
	return this->contourLine.size();
}
ai::int32 BrownLayer::GetOtherWhiteNum()
{
	return this->otherWhite.size();
}
ASErr BrownLayer::CheckError7(BlueLayer blueLayer,CollectError& collectError)
{
	ASErr error = kNoErr;
	//只要等高线与双线河相交即报错
	for(ai::int32 i=0;i<this->contourLine.size();i++)
	{
		for(ai::int32 j = 0;j<blueLayer.GetBluePolyOutLineNum();j++)
		{
			if(JudgePathIntersectPath(this->contourLine[i],blueLayer.GetBluePolyOutLinePath(j)))
			{
				collectError.AddError(7,this->contourLine[i].layerOrdinalNum,this->contourLine[i].artOrdinalNum);
				break;
			}
		}
	}
	return error;
}
ASErr BrownLayer::CheckError8(BlueLayer blueLayer,CollectError& collectError)
{
	ASErr error = kNoErr;
	for(ai::int32 i = 0;i<blueLayer.GetSingleLineRiverNum();i++)
	{
		for(ai::int32 j = 0;j<this->contourLine.size();j++)
		{
			if(JudgePathIntersectPath(blueLayer.GetSingleLineRiverPath(i),this->contourLine[j]))
			{
				if(!JudgePathOthogonal(blueLayer.GetSingleLineRiverPath(i),this->contourLine[j]))
				{
					collectError.AddError(8,this->contourLine[j].layerOrdinalNum,this->contourLine[j].artOrdinalNum);
				}
			}
		}
	}
	return error;
}

AIBoolean BlackLayer::Add(ai::int16 type,MyPath myPath)
{
	switch(type)
	{
	case 1:this->blackText.push_back(myPath);return true;break;
	case 2:this->blackPole.push_back(myPath);return true;break;
	case 3:this->blackDigital.push_back(myPath);return true;break;
	case 4:this->blackMainRoad.push_back(myPath);return true;break;
	case 5:this->blackByPassRoad.push_back(myPath);return true;break;
	case 6:this->blackSingleLineRoad.push_back(myPath);return true;break;
	case 7:this->wireDirecction.push_back(myPath);return true;break;
	case 8:this->wire.push_back(myPath);return true;break;
	case 9:this->bridge_1.push_back(myPath);return true;break;
	case 10:this->bridge_2.push_back(myPath);return true;break;
	case 11:this->stone.push_back(myPath);return true;break;
	case 12:this->otherWhite.push_back(myPath);return true;break;
	default:return false;
	}
}
	//下面三个函数均为获取函数服务
ASErr BlackLayer::GetPathOfLayer(AIArtHandle path)
{
	ASErr error = kNoErr;

	ai::int32 curArtNum = 0;
	ai::int32 curLayerNum = 3;
	
	vector<MyPath> temp;//暂时储存不能直接分类的path

	MyPath myPath;
	myPath.layerOrdinalNum = curLayerNum;
	AIPathStyle style;
	
	while(path)
	{
		
		if(curArtNum<8)
		{
			sAIArt->GetArtSibling(path,&path);
		    curArtNum++;
			continue;
		}
		myPath.artOrdinalNum = curArtNum;
		error = sAIPathStyle->GetPathStyle(path,&style);
		sAIPath->GetPathSegmentCount(path,&myPath.segmentNum);
	    sAIPath->GetPathSegments(path,0,myPath.segmentNum,myPath.segment);
		sAIPath->GetPathArea(path,&myPath.area);
		
		//获取文字
		if(style.fillPaint==1&&style.strokePaint==0&&style.fill.color.c.p.shiftDist==0&&myPath.segmentNum>4)
		{
			this->Add(1,myPath);
		}
		else if(style.fillPaint==1&&style.strokePaint==0&&style.fill.color.c.p.shiftDist==0&&myPath.segmentNum==4&&
			round(fabs(myPath.segment[0].p.v-myPath.segment[1].p.v)/unitConversation*100)/100==0.2&&
			round(fabs(myPath.segment[0].p.h-myPath.segment[1].p.h)/unitConversation*100)/100==0.2)//获取pole
		{
			this->Add(2,myPath);
		}
		else if(style.fillPaint==1&&style.strokePaint==1)
		{
			this->Add(3,myPath);
		}
		else if(style.fillPaint==0&&style.strokePaint==0&&style.stroke.width>=0.3401&&style.stroke.width<=0.3403&&myPath.segmentNum==4)
		{
			//此处判断桥梁，仅仅是简单的规律判断，可能是不准的，后续看情况调整
			sAIArt->GetArtSibling(path,&path);
			sAIPathStyle->GetPathStyle(path,&style);
			ai::int16 num;
			sAIPath->GetPathSegmentCount(path,&num);
			AIBoolean flag = 0;
			if(style.fillPaint==0&&style.strokePaint==1&&style.stroke.width>=0.3401&&style.stroke.width<=0.3403&&num==4)
			{
				sAIArt->GetArtSibling(path,&path);
			    sAIPathStyle->GetPathStyle(path,&style);
				sAIPath->GetPathSegmentCount(path,&num);
				if(style.fillPaint==1&&style.strokePaint==0&&style.fill.color.c.c.tint==1&&num==8)
				{
					curArtNum +=2;
					myPath.artOrdinalNum = curArtNum;
	            	sAIPath->GetPathSegmentCount(path,&myPath.segmentNum);
	                sAIPath->GetPathSegments(path,0,myPath.segmentNum,myPath.segment);
		            sAIPath->GetPathArea(path,&myPath.area);
					this->Add(9,myPath);
					flag = 1;
				}
				else
				{
					sAIArt->GetArtPriorSibling(path,&path);
					sAIArt->GetArtPriorSibling(path,&path);
				    sAIPathStyle->GetPathStyle(path,&style);
				}
			}
			else
			{
				sAIArt->GetArtPriorSibling(path,&path);
				sAIPathStyle->GetPathStyle(path,&style);
			}
			//若不是桥梁则是路
			if(flag == 0)
			{
				this->Add(4,myPath);
			}
		}
		else if(style.fillPaint==0&&style.strokePaint==1&&style.stroke.width>=0.3401&&style.stroke.width<=0.3403)
		{
			this->Add(4,myPath);
		}
		else if(style.fillPaint==0&&style.strokePaint==1&&style.stroke.width>=0.2834&&style.stroke.width<=0.2836)
		{
			if(myPath.segmentNum == 2)
			{
				AIReal length = sqrt(pow(myPath.segment[0].p.h-myPath.segment[1].p.h,2) + pow(myPath.segment[0].p.v-myPath.segment[1].p.v,2));
				if(length>=1.983&&length<=1.985)
					this->Add(7,myPath);
				else
					temp.push_back(myPath);
			}
			else
				temp.push_back(myPath);//在后续处理中会被分到5,8中，
		}
		else if(style.fillPaint==0&&style.strokePaint==1&&style.stroke.width>=0.5668&&style.stroke.width<=0.5670)
		{
			this->Add(6,myPath);
		}
		else if(style.fillPaint==1&&style.strokePaint==0&&style.fill.color.c.c.tint==1)
		{
			this->Add(12,myPath);
		}
		else if(style.fillPaint==0&&style.strokePaint==1&&style.stroke.color.c.p.shiftDist==1&&myPath.segmentNum==2)
		{
			AIArtHandle path1,path2,path3,path4;
			AIPathStyle style1,style2,style3,style4;
			ai::int16 num1,num2,num3,num4;

			sAIArt->GetArtSibling(path,&path1);
			sAIPath->GetPathSegmentCount(path1,&num1);
			sAIPathStyle->GetPathStyle(path1,&style1);

			sAIArt->GetArtSibling(path1,&path2);
			sAIPath->GetPathSegmentCount(path2,&num2);
			sAIPathStyle->GetPathStyle(path2,&style2);

			sAIArt->GetArtSibling(path2,&path3);
			sAIPath->GetPathSegmentCount(path3,&num3);
			sAIPathStyle->GetPathStyle(path3,&style3);

			sAIArt->GetArtSibling(path3,&path4);
			sAIPath->GetPathSegmentCount(path4,&num4);
			sAIPathStyle->GetPathStyle(path4,&style4);
			if(style1.fillPaint==0&&style1.strokePaint==1&&style1.stroke.color.c.p.shiftDist==1&&num1==2&&
				style2.fillPaint==0&&style2.strokePaint==1&&style2.stroke.color.c.p.shiftDist==0&&num2==2&&
				style3.fillPaint==0&&style3.strokePaint==1&&style3.stroke.color.c.p.shiftDist==0&&num3==2&&
				style4.fillPaint==0&&style4.strokePaint==1&&style4.stroke.color.c.p.shiftDist==1&&num4==2)
			{
				curArtNum +=4;
				myPath.artOrdinalNum = curArtNum;
				sAIArt->GetArtSibling(path3,&path);
		        error = sAIPathStyle->GetPathStyle(path,&style);
		        sAIPath->GetPathSegmentCount(path,&myPath.segmentNum);
	            sAIPath->GetPathSegments(path,0,myPath.segmentNum,myPath.segment);
		        sAIPath->GetPathArea(path,&myPath.area);
				this->Add(10,myPath);
			}
		}
		else if(style.fillPaint==1&&style.strokePaint==0&&style.fill.color.c.c.tint>0&&style.fill.color.c.c.tint<1)
		{
			this->Add(11,myPath);
		}

		
		sAIArt->GetArtSibling(path,&path);
		curArtNum++;
	}
	
	Classify(temp);
	/*
	ai::UnicodeString a=ai::UnicodeString("");
	ai::UnicodeString b;
	sAIUser->AIRealToString(this->blackText.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->blackPole.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->blackDigital.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->blackMainRoad.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->blackByPassRoad.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->blackSingleLineRoad.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->wireDirecction.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->wire.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->bridge_1.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->bridge_2.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->stone.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->AIRealToString(this->otherWhite.size(),0,b);
	a.append(b);
	a.append(ai::UnicodeString("|"));
	sAIUser->MessageAlert(a);
	*/
	return error;
}
void BlackLayer::Classify(vector<MyPath> paths)
{
	for(ai::int32 i = 0;i<paths.size();i++)
	{
		for(ai::int32 j = 0;j<this->blackPole.size();j++)
		{
			if(JudgeLineIntersectCircle(paths[i],this->blackPole[j]))
			{
				this->Add(8,paths[i]);
				paths.erase(paths.begin()+i);
				i--;
				break;
			}
		}
	}
	for(ai::int32 i = 0;i<paths.size();i++)
	{
		this->Add(5,paths[i]);
	}
	//将在道路中储存的黑石头的边线移除
	for(ai::int32 i = 0;i<this->stone.size();i++)
	{
		for(ai::int32 j = 0;j<this->blackByPassRoad.size();j++)
		{
			if(OtherWhiteMatching(this->stone[i],this->blackByPassRoad[j]))
			{
				this->blackByPassRoad.erase(this->blackByPassRoad.begin()+j);
				break;
			}
		}
	}
}
AIBoolean BlackLayer::JudgeLineIntersectCircle(MyPath path,MyPath pole)
{
	for(ai::int32 i = 0;i<path.segmentNum;i++)
	{
		if(JudgeSegmentInCircle(path.segment[i],pole))
			return true;
	}
	return false;
}
MyPath BlackLayer::GetBlackTextPath(ai::int32 index)
{
	if(index<0||index>=this->blackText.size())
		throw "GetBlackTextPath";
	else
		return this->blackText[index];
}
MyPath BlackLayer::GetBlackPolePath(ai::int32 index)
{
	if(index<0||index>=this->blackPole.size())
		throw "GetBlackPolePath";
	else
		return this->blackPole[index];
}
MyPath BlackLayer::GetBlackDigitalPath(ai::int32 index)
{
	if(index<0||index>=this->blackDigital.size())
		throw "GetBlackDigitalPath";
	else
		return this->blackDigital[index];
}
MyPath BlackLayer::GetBlackMainRoadPath(ai::int32 index)
{
	if(index<0||index>=this->blackMainRoad.size())
		throw "GetBlackMainRoadPath";
	else
		return this->blackMainRoad[index];
}
MyPath BlackLayer::GetBlackByPassRoadPath(ai::int32 index)
{
	if(index<0||index>=this->blackByPassRoad.size())
		throw "GetBlackByPassRoadPath";
	else
		return this->blackByPassRoad[index];
}
MyPath BlackLayer::GetBlackSingleLineRoadPath(ai::int32 index)
{
	if(index<0||index>=this->blackSingleLineRoad.size())
		throw "GetBlackSingleLineRoadPath";
	else
		return this->blackSingleLineRoad[index];
}
MyPath BlackLayer::GetWireDirecctionPath(ai::int32 index)
{
	if(index<0||index>=this->wireDirecction.size())
		throw "GetWireDirecctionPath";
	else
		return this->wireDirecction[index];
}
MyPath BlackLayer::GetWirePath(ai::int32 index)
{
	if(index<0||index>=this->wire.size())
		throw "GetWirePath";
	else
		return this->wire[index];
}
MyPath BlackLayer::GetBridge_1Path(ai::int32 index)
{
	if(index<0||index>=this->bridge_1.size())
		throw "GetBridge_1Path";
	else
		return this->bridge_1[index];
}
MyPath BlackLayer::GetBridge_2Path(ai::int32 index)
{
	if(index<0||index>=this->bridge_2.size())
		throw "GetBridge_2Path";
	else
		return this->bridge_2[index];
}
MyPath BlackLayer::GetStonePath(ai::int32 index)
{
	if(index<0||index>=this->stone.size())
		throw "GetStonePath";
	else
		return this->stone[index];
}
MyPath BlackLayer::GetOtherWhitePath(ai::int32 index)
{
	if(index<0||index>=this->otherWhite.size())
		throw "GetOtherWhitePath";
	else
		return this->otherWhite[index];
}
ai::int32 BlackLayer::GetBlackTextNum()
{
	return this->blackText.size();
}
ai::int32 BlackLayer::GetBlackPoleNum()
{
	return this->blackPole.size();
}
ai::int32 BlackLayer::GetBlackDigitalNum()
{
	return this->blackDigital.size();
}
ai::int32 BlackLayer::GetBlackMainRoadNum()
{
	return this->blackMainRoad.size();
}
ai::int32 BlackLayer::GetBlackByPassRoadNum()
{
	return this->blackByPassRoad.size();
}
ai::int32 BlackLayer::GetBlackSingleLineRoadNum()
{
	return this->blackSingleLineRoad.size();
}
ai::int32 BlackLayer::GetWireDirecctionNum()
{
	return this->wireDirecction.size();
}
ai::int32 BlackLayer::GetWireNum()
{
	return this->wire.size();
}
ai::int32 BlackLayer::GetBridge_1Num()
{
	return this->bridge_1.size();
}
ai::int32 BlackLayer::GetBridge_2Num()
{
	return this->bridge_2.size();
}
ai::int32 BlackLayer::GetStoneNum()
{
	return this->stone.size();
}
ai::int32 BlackLayer::GetOtherWhiteNum()
{
	return this->otherWhite.size();
}
ASErr BlackLayer::CheckError4(GreenLayer greenLayer,CollectError& collectError)
{
	ASErr error = kNoErr;

	//后续应该可以优化，此处写的很暴力，思路是先找出与路相交的绿色多边形，然后从blackLayer中的otherWhite匹配对应的绿色多边形，从而判断是否覆盖
	for(ai::int32 i = 0;i<this->blackMainRoad.size();i++)
	{
		for(ai::int32 j = 0;j<greenLayer.GetGreenPolyBigNum();j++)
		{
			if(JudgePathIntersectPath(this->blackMainRoad[i],greenLayer.GetGreenPolyBigPath(j)))
			{
				for(ai::int32 k = 0;k<this->otherWhite.size();k++)
				{
					if(OtherWhiteMatching(this->otherWhite[k],greenLayer.GetGreenPolyBigPath(j)))
					{
						if(this->otherWhite[k].artOrdinalNum<this->blackMainRoad[i].artOrdinalNum)
						{
							collectError.AddError(4,greenLayer.GetGreenPolyBigPath(j).layerOrdinalNum,greenLayer.GetGreenPolyBigPath(j).artOrdinalNum);
							break;
						}
						break;
					}
				}
			}
		}
	}
	for(ai::int32 i = 0;i<this->blackMainRoad.size();i++)
	{
		for(ai::int32 j = 0;j<greenLayer.GetGreenPolySmallNum();j++)
		{
			if(JudgePathIntersectPath(this->blackMainRoad[i],greenLayer.GetGreenPolySmallPath(j)))
			{
				for(ai::int32 k = 0;k<this->otherWhite.size();k++)
				{
					if(OtherWhiteMatching(this->otherWhite[k],greenLayer.GetGreenPolySmallPath(j)))
					{
						if(this->otherWhite[k].artOrdinalNum<this->blackMainRoad[i].artOrdinalNum)
						{
							collectError.AddError(4,greenLayer.GetGreenPolySmallPath(j).layerOrdinalNum,greenLayer.GetGreenPolySmallPath(j).artOrdinalNum);
							break;
						}
						break;
					}
				}
			}
		}
	}

	return error;
}
ASErr BlackLayer::CheckError5(CollectError& collectError)
{
	ASErr error = kNoErr;
	
	vector<MyPath> v;
	for(ai::int32 i = 0;i<this->wire.size();i++)
	{
		ai::int32 count = 0;
		for(ai::int32 j = 0;j<this->wire.size();j++)
		{
			if(j!=i)
			{
				//开始判断两条wire是否相接
				if(SegmentEqual(this->wire[i].segment[0],this->wire[j].segment[0])||SegmentEqual(this->wire[i].segment[0],this->wire[j].segment[1])||
					SegmentEqual(this->wire[i].segment[1],this->wire[j].segment[1])||SegmentEqual(this->wire[i].segment[1],this->wire[j].segment[0]))
					count++;
			}
		}
		if(count == 1)
			v.push_back(this->wire[i]);
	}
	for(ai::int32 i = 0;i<v.size();i++)
	{
		ai::int32 count = 0;
		for(ai::int32 j = 0;j<this->blackPole.size();j++)
		{
			if(JudgeSegmentInCircle(v[i].segment[0],this->blackPole[j]))
				count++;
			if(JudgeSegmentInCircle(v[i].segment[1],this->blackPole[j]))
				count++;
		}
		if(count==3)
		{
			collectError.AddError(5,v[i].layerOrdinalNum,v[i].artOrdinalNum);
		}
	}
	return error;
}
ASErr BlackLayer::CheckError6(BlueLayer blueLayer,CollectError& collectError)
{
	ASErr error = kNoErr;

	for(ai::int32 i = 0;i<this->wire.size();i++)
	{
		for(ai::int32 j = 0;j<blueLayer.GetBluePolyNum();j++)
		{
			if(JudgePathIntersectPath(this->wire[i],blueLayer.GetBluePolyPath(j)))
			{
				for(ai::int32 k = 0;k<this->otherWhite.size();k++)
				{
					if(OtherWhiteMatching(this->otherWhite[k],blueLayer.GetBluePolyPath(j)))
					{
						if(this->otherWhite[k].artOrdinalNum<this->wire[i].artOrdinalNum)
						{
							collectError.AddError(6,blueLayer.GetBluePolyPath(j).layerOrdinalNum,blueLayer.GetBluePolyPath(j).artOrdinalNum);
							break;
						}
						break;
					}
				}
			}
		}
	}

	return error;
}
ASErr BlackLayer::CheckError9(CollectError& collectError)
{
	ASErr error = kNoErr;

	for(ai::int32 i = 0;i<this->blackSingleLineRoad.size();i++)
	{
		for(ai::int32 j = 0;j<this->blackMainRoad.size();j++)
		{
			if(JudgePathIntersectPath(this->blackSingleLineRoad[i],this->blackMainRoad[j]))
			{
				collectError.AddError(9,this->blackSingleLineRoad[i].layerOrdinalNum,this->blackSingleLineRoad[i].artOrdinalNum);

			}
		}
	}
	return error;
}
ASErr BlackLayer::CheckError10(CollectError& collectError)
{
	ASErr error = kNoErr;
	//暂时有点问题，后续解决
	for(ai::int32 i = 0;i<this->blackByPassRoad.size();i++)
	{
		for(ai::int32 j = 0;j<this->blackMainRoad.size();j++)
		{
			if(JudgePathIntersectPath(this->blackByPassRoad[i],this->blackMainRoad[j]))
			{
				collectError.AddError(10,this->blackByPassRoad[i].layerOrdinalNum,this->blackByPassRoad[i].artOrdinalNum);
			}
		}
	}

	return error;
}
ASErr BlackLayer::CheckError13(CollectError& collectError)
{
	ASErr error = kNoErr;

	for(ai::int32 i = 0;i<this->stone.size();i++)
	{
		for(ai::int32 j = 0;j<this->blackText.size();j++)
		{
			if(JudgePathIntersectPath2(this->blackText[j],this->stone[i]))
			{
				collectError.AddError(13,this->stone[i].layerOrdinalNum,this->stone[i].artOrdinalNum);
				break;
			}
		}
	}

	return error;
}
