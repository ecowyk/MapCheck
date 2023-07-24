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
//·���ṹ
typedef struct
{
	AIPathSegment segment[segmentNumMAX];//ê��
	AIReal lineWidth;//�߿�
	AIReal area;//���
	ai::int32 layerOrdinalNum;//ͼ�����
	ai::int32 artOrdinalNum;//ͼԪ���
	ai::int16 segmentNum;//ê����
} MyPath;

//�߶�
typedef struct
{
	AIReal x1,y1,x2,y2;
} Line;

//������Ϣ
typedef struct
{
	ai::int32 errorType;//��������
	ai::int32 layerOrdinalNum;//�ڼ���ͼ��
	ai::int32 artOrdinalNum;//��ͼ��ĵڼ���ͼԪ
} ErrorInfo;

#endif