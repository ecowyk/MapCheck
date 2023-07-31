#include "IllustratorSDK.h"
#include "MapCheckSuites.h"
#include "Functions.h"

AIBoolean JudgePath1InsidePath2(MyPath path1,MyPath path2)
{
	
	for(ai::int32 j = 0;j<path1.segmentNum;j++)
	{
		ai::int32 count = 0;
		for(ai::int32 i = 0;i<path2.segmentNum;i++)
	    {
		    ai::int32 k = i+1;
		    if(i==path2.segmentNum-1)
		    	k = 0;
			if(path1.segment[j].p.v <path2.segment[i].p.v&&path1.segment[j].p.v <path2.segment[k].p.v)
		    	continue;
	    	if(path1.segment[j].p.v >=path2.segment[i].p.v&&path1.segment[j].p.v >=path2.segment[k].p.v)
			    continue;
	        if(path2.segment[i].p.v == path2.segment[k].p.v)
		    	continue;
	     	AIReal x = (path1.segment[j].p.v - path2.segment[i].p.v) * (path2.segment[k].p.h - path2.segment[i].p.h) / (path2.segment[k].p.v - path2.segment[i].p.v) + path2.segment[i].p.h;
			if (x >path1.segment[j].p.h)
		        count++;
	    }
    	if(count%2==0)
			return false;
	}
	return true;
}

AIBoolean JudgeLineIntersectLine(Line line1, Line line2)
{
	 //快速排斥实验
    if ((line1.x1 > line1.x2 ? line1.x1 : line1.x2) < (line2.x1 < line2.x2 ? line2.x1 : line2.x2) ||
        (line1.y1 > line1.y2 ? line1.y1 : line1.y2) < (line2.y1 < line2.y2 ? line2.y1 : line2.y2) ||
        (line2.x1 > line2.x2 ? line2.x1 : line2.x2) < (line1.x1 < line1.x2 ? line1.x1 : line1.x2) ||
        (line2.y1 > line2.y2 ? line2.y1 : line2.y2) < (line1.y1 < line1.y2 ? line1.y1 : line1.y2))
    {
        return false;
    }
    //跨立实验
    if ((((line1.x1 - line2.x1)*(line2.y2 - line2.y1) - (line1.y1 - line2.y1)*(line2.x2 - line2.x1))*
        ((line1.x2 - line2.x1)*(line2.y2 - line2.y1) - (line1.y2 - line2.y1)*(line2.x2 - line2.x1))) >= 0 ||
        (((line2.x1 - line1.x1)*(line1.y2 - line1.y1) - (line2.y1 - line1.y1)*(line1.x2 - line1.x1))*
        ((line2.x2 - line1.x1)*(line1.y2 - line1.y1) - (line2.y2 - line1.y1)*(line1.x2 - line1.x1))) >= 0)
    {
        return false;
    }
    return true;
}

AIBoolean JudgeLineOthogonal(Line line1,Line line2)
{
	AIReal dx1 = line1.x1 - line1.x2;
	AIReal dy1 = line1.y1 - line1.y2;
	AIReal dx2 = line2.x1 - line2.x2;
	AIReal dy2 = line2.y1 - line2.y2;
	if(dx1 * dx2 + dy1 * dy2 == 0)
	{
		return true;
	}
	return false;
}

AIBoolean JudgePathIntersectPath(MyPath path1,MyPath path2)
{
	Line line1;
	Line line2;
	for(ai::int32 i=0;i<path1.segmentNum-1;i++)
	{
		line1.x1 = path1.segment[i].p.h;
		line1.y1 = path1.segment[i].p.v;
		line1.x2 = path1.segment[i+1].p.h;
		line1.y2 = path1.segment[i+1].p.v;
		for(ai::int32 j = 0;j<path2.segmentNum-1;j++)
		{
			line2.x1 = path2.segment[j].p.h;
	    	line2.y1 = path2.segment[j].p.v;
	    	line2.x2 = path2.segment[j+1].p.h;
		    line2.y2 = path2.segment[j+1].p.v;
			if(JudgeLineIntersectLine(line1,line2))
				return true;
		}
	}
	return false;
}

AIBoolean JudgePathIntersectPath2(MyPath path1,MyPath path2)
{
	//因为path2为多边形，故先判断path1中是否有锚点在path2中，若有则在，否则进入下一判断
	for(ai::int32 i = 0;i<path1.segmentNum;i++)
	{
		if(JudgeSegmentInPath(path1.segment[i],path2))
		{
			return true;
		}
	}
	if(JudgePathIntersectPath(path1,path2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

AIBoolean JudgePathOthogonal(MyPath path1,MyPath path2)
{
	Line line1;
	Line line2;
	for(ai::int32 i=0;i<path1.segmentNum-1;i++)
	{
		line1.x1 = path1.segment[i].p.h;
		line1.y1 = path1.segment[i].p.v;
		line1.x2 = path1.segment[i+1].p.h;
		line1.y2 = path1.segment[i+1].p.v;
		for(ai::int32 j = 0;j<path2.segmentNum-1;j++)
		{
			line2.x1 = path2.segment[j].p.h;
	    	line2.y1 = path2.segment[j].p.v;
	    	line2.x2 = path2.segment[j+1].p.h;
		    line2.y2 = path2.segment[j+1].p.v;
			if(JudgeLineIntersectLine(line1,line2))
			{
				if(!JudgeLineOthogonal(line1,line2))
				{
					return false;
				}
			}
		}
	}
	return true;
}

AIBoolean JudgePathIntersectPath(MyPath path1,MyPath path2,ai::int32& count)
{
	Line line1;
	Line line2;
	for(ai::int32 i=0;i<path1.segmentNum-1;i++)
	{
		line1.x1 = path1.segment[i].p.h;
		line1.y1 = path1.segment[i].p.v;
		line1.x2 = path1.segment[i+1].p.h;
		line1.y2 = path1.segment[i+1].p.v;
		for(ai::int32 j = 0;j<path2.segmentNum-1;j++)
		{
			line2.x1 = path2.segment[j].p.h;
	    	line2.y1 = path2.segment[j].p.v;
	    	line2.x2 = path2.segment[j+1].p.h;
		    line2.y2 = path2.segment[j+1].p.v;
			if(JudgeLineIntersectLine(line1,line2))
				count++;
		}
	}
	return count==0?false:true;
}

AIBoolean OtherWhiteMatching(MyPath path1,MyPath path2)
{
	if(path1.segmentNum!=path2.segmentNum)
		return false;
	if(path1.area!=path2.area)
		return false;

	//此处仅做简单判断，精确判断可以用坐标去对应。
	return true;
}

AIBoolean OtherWhiteMatching2(MyPath path1,MyPath path2)
{
	if(path2.segmentNum!=2)
		return false;
	if((SegmentEqual(path1.segment[0],path2.segment[0])&&SegmentEqual(path1.segment[1],path2.segment[1]))||
		(SegmentEqual(path1.segment[0],path2.segment[1])&&SegmentEqual(path1.segment[1],path2.segment[0])))
	{
		return true;
	}
	return false;
}

AIBoolean SegmentEqual(AIPathSegment segment1,AIPathSegment segment2)
{
	if(segment1.p.h == segment2.p.h&&segment1.p.v == segment2.p.v)
		return true;
	return false;
}

AIBoolean JudgeSegmentInPath(AIPathSegment segment , MyPath path)
{
	ai::int32 count = 0;
	for(ai::int32 i = 0;i<path.segmentNum;i++)
    {
	    ai::int32 k = i+1;
	    if(i==path.segmentNum-1)
	    	k = 0;
		if(segment.p.v <path.segment[i].p.v&&segment.p.v <path.segment[k].p.v)
	    	continue;
    	if(segment.p.v >=path.segment[i].p.v&&segment.p.v >=path.segment[k].p.v)
		    continue;
        if(path.segment[i].p.v == path.segment[k].p.v)
	    	continue;
     	AIReal x = (segment.p.v - path.segment[i].p.v) * (path.segment[k].p.h - path.segment[i].p.h) / (path.segment[k].p.v - path.segment[i].p.v) + path.segment[i].p.h;
		if (x >segment.p.h)
	        count++;
    }
   	if(count%2==1)
		return true;
	return false;
}

AIBoolean JudgeSegmentInCircle(AIPathSegment segment,MyPath circle)
{
	AIReal x = (circle.segment[0].p.h+circle.segment[1].p.h+circle.segment[2].p.h+circle.segment[3].p.h)/4;
	AIReal y = (circle.segment[0].p.v+circle.segment[1].p.v+circle.segment[2].p.v+circle.segment[3].p.v)/4;
	AIReal r = fabs(circle.segment[0].p.h-circle.segment[1].p.h);

	AIReal dist = sqrt(pow(segment.p.h - x,2)+pow(segment.p.v - y,2));
	if(dist<r)
		return true;
	return false;
}

AIReal round(AIReal r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

AIReal segmentLength(Line l)
{
	double dx = l.x1 - l.x2;
    double dy = l.y1 - l.y2;
    return std::sqrt(dx * dx + dy * dy);
}

AIReal pointToSegmentDistance(AIReal x,AIReal y,Line l)
{
	double len=segmentLength(l);
	double t =((x-l.x1)*(l.x2-l.x1)+(y-l.y1)*(l.y2-l.y1))/len*len;
	if(t<0.0)
		return std::sqrt((x-l.x1)*(x-l.x1)+(y-l.y1)*(y-l.y1));
	if(t>1.0)
		return std::sqrt((x-l.x2)*(x-l.x2)+(y-l.y2)*(y-l.y2));

	AIReal m=l.x1+t*(l.x2-l.x1);
	AIReal n=l.y1+t*(l.y2-l.y1);
	return std::sqrt((x-m)*(x-m)+(y-n)*(y-n));
}

AIReal shortestDistanceBetweenPolylines(MyPath polyline1,MyPath polyline2)
{
	double shortestDist = 0xfff;
	// 遍历第一条折线上的所有线段
	for (size_t i = 0; i < polyline1.segmentNum - 1; ++i)
	{
		AIReal p1x=polyline1.segment[i].p.h;
		AIReal p1y=polyline1.segment[i].p.v;
		AIReal p2x=polyline1.segment[i+1].p.h;
		AIReal p2y=polyline1.segment[i+1].p.v;
		Line L1={p1x,p1y,p2x,p2y};
		// 遍历第二条折线上的所有线段
		for (size_t j = 0; j < polyline2.segmentNum - 1; ++j)
		{
			AIReal q1x=polyline1.segment[i].p.h;
			AIReal q1y=polyline1.segment[i].p.v;
			AIReal q2x=polyline1.segment[i+1].p.h;
			AIReal q2y=polyline1.segment[i+1].p.v;
			Line L2={q1x,q1y,q2x,q2y};

			// 计算线段 p1p2 和 q1q2 之间的距离
			double dist=pointToSegmentDistance(p1x,p1y,L2);
			dist = dist<pointToSegmentDistance(p2x,p2y,L2) ? dist : pointToSegmentDistance(p2x,p2y,L2);
			dist = dist<pointToSegmentDistance(q1x,q1y,L1) ? dist : pointToSegmentDistance(q1x,q1y,L1);
			dist = dist<pointToSegmentDistance(q2x,q2y,L1) ? dist : pointToSegmentDistance(q2x,q2y,L1);

			// 更新最短距离
            if (dist < shortestDist) 
			{
                shortestDist = dist;
            }
		}
	}
	return shortestDist;
}

ASErr GetPathCenter(AIArtHandle path, AIRealPoint& center)
{
	ASErr error = kNoErr;
	AIPathSegment segment[segmentNumMAX];
	ai::int16 segmentNum;
	sAIPath->GetPathSegmentCount(path,&segmentNum);
	sAIPath->GetPathSegments(path,0,segmentNum,segment);
	for(ai::int32 i = 0;i<segmentNum;i++)
	{
		center.h += segment[i].p.h;
		center.v += segment[i].p.v;
	}
	center.h = center.h / segmentNum;
	center.v = center.v / segmentNum;
	return error;
}