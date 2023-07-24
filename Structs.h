/***********************************************************************/
/*                                                                     */
/* "Structs.h"                                                         */
/* Description : This file is used to describe the structure and       */
/* constants used for the project.                                     */
/* Started by zhunan, 2023/7/23                                        */
/* Updated by zhunan, 2023/7/24                                        */
/*                                                                     */
/***********************************************************************/
#ifndef __Structs_H__
#define __Structs_H__

#include "Plugin.hpp"
#include "AIPath.h"

const ai::int32 segmentNumMAX = 100;
const AIReal unitConversation = 72/25.4;
const AIReal areaCheckToPoint = 25*unitConversation*unitConversation;
const AIReal areaCheckIfAnnotation = 400*unitConversation*unitConversation;
//路径结构
typedef struct
{
	AIPathSegment segment[segmentNumMAX];//锚点
	AIReal lineWidth;//线宽
	AIReal area;//面积
	ai::int32 layerOrdinalNum;//图层序号
	ai::int32 artOrdinalNum;//图元序号
	ai::int16 segmentNum;//锚点数
} MyPath;

//线段
typedef struct
{
	AIReal x1,y1,x2,y2;
} Line;

//错误信息
typedef struct
{
	ai::int32 errorType;//错误类型
	ai::int32 layerOrdinalNum;//第几个图层
	ai::int32 artOrdinalNum;//该图层的第几个图元
} ErrorInfo;

#endif