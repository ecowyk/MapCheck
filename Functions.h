/***********************************************************************/
/*                                                                     */
/* "Functions.h"                                                       */
/* Description : This file is used to describe the functions used for  */
/* the project.                                                        */
/* Started by zhunan, 2023/7/23                                        */
/* Updated by eco, 2023/7/28                                           */
/*                                                                     */
/***********************************************************************/
#ifndef __Functions_h__
#define __Functions_h__

#include "Structs.h"
AIBoolean JudgePath1InsidePath2(MyPath path1,MyPath path2);//һ��·������һ������,path��Ϊ�����

AIBoolean JudgeLineIntersectLine(Line line1, Line line2);//�߶��ཻ

AIBoolean JudgeLineOthogonal(Line line1,Line line2);//�ж��߶�����

AIBoolean JudgePathIntersectPath(MyPath path1,MyPath path2);//·����·���ཻ

AIBoolean JudgePathIntersectPath2(MyPath path1,MyPath path2);//�ж�·���ཻ��path2Ϊ�պ�·��

AIBoolean JudgePathOthogonal(MyPath path1,MyPath path2);//�ж�·������

AIBoolean JudgePathIntersectPath(MyPath path1,MyPath path2,ai::int32& count);//�ж�·���ཻ������������

AIBoolean OtherWhiteMatching(MyPath path1,MyPath path2);//ƥ�亯��

AIBoolean OtherWhiteMatching2(MyPath path1,MyPath path2);//����bridge_2��ƥ��

AIBoolean SegmentEqual(AIPathSegment segment1,AIPathSegment segment2);//ê������ж�

AIBoolean JudgeSegmentInPath(AIPathSegment segment , MyPath path);//�ж�ê���Ƿ���·��(�����)��

AIBoolean JudgeSegmentInCircle(AIPathSegment segment,MyPath circle);//�ж�ê���Ƿ���Բ��

AIReal round(AIReal r);//��������

AIReal segmentLength(Line l);//�߶γ���

AIReal pointToSegmentDistance(AIReal x,AIReal y,Line l);//�㵽�߶ξ���

AIReal shortestDistanceBetweenPolylines(MyPath polyline1,MyPath polyline2);//���߼���̾���

ASErr GetPathCenter(AIArtHandle path, AIRealPoint& center);//��ȡ·�����ģ��Ա������ʾ
#endif