/***********************************************************************/
/*                                                                     */
/* "Functions.h"                                                       */
/* Description : This file is used to describe the functions used for  */
/* the project.                                                        */
/* Started by zhunan, 2023/7/23                                        */
/* Updated by zhunan, 2023/7/24                                        */
/*                                                                     */
/***********************************************************************/
#ifndef __Functions_h__
#define __Functions_h__

#include "Structs.h"
AIBoolean JudgePath1InsidePath2(MyPath path1,MyPath path2);//一条路径在另一条里面,path均为多边形

AIBoolean JudgeLineIntersectLine(Line line1, Line line2);//线段相交

AIBoolean JudgeLineOthogonal(Line line1,Line line2);//判断线段正交

AIBoolean JudgePathIntersectPath(MyPath path1,MyPath path2);//路径和路径相交

AIBoolean JudgePathIntersectPath2(MyPath path1,MyPath path2);//判断路径相交，path2为闭合路径

AIBoolean JudgePathOthogonal(MyPath path1,MyPath path2);//判断路径正交

AIBoolean JudgePathIntersectPath(MyPath path1,MyPath path2,ai::int32& count);//判断路径相交并给出交点数

AIBoolean OtherWhiteMatching(MyPath path1,MyPath path2);//匹配函数

AIBoolean OtherWhiteMatching2(MyPath path1,MyPath path2);//用于bridge_2的匹配

AIBoolean SegmentEqual(AIPathSegment segment1,AIPathSegment segment2);//锚点相等判断

AIBoolean JudgeSegmentInPath(AIPathSegment segment , MyPath path);//判断锚点是否在路径(多边形)内

AIBoolean JudgeSegmentInCircle(AIPathSegment segment,MyPath circle);//判断锚点是否在圆内

AIReal round(AIReal r);//四舍五入


#endif