#ifndef _LINE_LINE_INTERSECT_H
#define _LINE_LINE_INTERSECT_H

///Calculate intersection of two lines.
///\return true if found, false if not found or error
bool LineLineIntersect(double x1, double y1, //Line 1 start
	double x2, double y2, //Line 1 end
	double x3, double y3, //Line 2 start
	double x4, double y4, //Line 2 end
	double &ixOut, double &iyOut); //Output 

void LineLineIntersectTests();

#endif //_LINE_LINE_INTERSECT_H

