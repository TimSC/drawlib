//2D Line-line intersection using determinants

#include <iostream>
#include <cmath>
#include <assert.h>
#include "LineLineIntersect.h"
using namespace std;

/** Calculate determinant of matrix:
	[a b]
	[c d]
*/
inline double Mat2x2Det(double a, double b, double c, double d)
{
	return a*d - b*c;
}

///Calculate intersection of two lines.
///\return true if found, false if not found or error
bool LineLineIntersect(double x1, double y1, //Line 1 start
	double x2, double y2, //Line 1 end
	double x3, double y3, //Line 2 start
	double x4, double y4, //Line 2 end
	double &ixOut, double &iyOut) //Output 
{
	//http://mathworld.wolfram.com/Line-LineIntersection.html

	double detL1 = Mat2x2Det(x1, y1, x2, y2);
	double detL2 = Mat2x2Det(x3, y3, x4, y4);
	double x1mx2 = x1 - x2;
	double x3mx4 = x3 - x4;
	double y1my2 = y1 - y2;
	double y3my4 = y3 - y4;

	double xnom = Mat2x2Det(detL1, x1mx2, detL2, x3mx4);
	double ynom = Mat2x2Det(detL1, y1my2, detL2, y3my4);
	double denom = Mat2x2Det(x1mx2, y1my2, x3mx4, y3my4);
	if(denom == 0.0)//Lines don't seem to cross
	{
		ixOut = NAN;
		iyOut = NAN;
		return false;
	}

	ixOut = xnom / denom;	
	iyOut = ynom / denom;
	if(!isfinite(ixOut) || !isfinite(iyOut)) //Probably a numerical issue
		return false;

	return true; //All OK
}

void LineLineIntersectTests()
{
	// **Simple crossing diagonal lines**

	//Line 1
	double x1=4.0, y1=0.0;
	double x2=6.0, y2=10.0;
	
	//Line 2
	double x3=0.0, y3=3.0;
	double x4=10.0, y4=7.0;

	double ix = -1.0, iy = -1.0;
	bool result = LineLineIntersect(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy);
	cout << "result " <<  result << "," << ix << "," << iy << endl;

	double eps = 1e-6;
	assert(result == true);
	assert(fabs(ix - 5.0) < eps);
	assert(fabs(iy - 5.0) < eps);

	// ** Parallel lines **
	
	x1=0.0; y1=3.0;
	x2=10.0; y2=3.0;
	x3=0.0; y3=7.0;
	x4=10.0; y4=7.0;

	ix = -1.0; iy = -1.0;
	result = LineLineIntersect(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy);
	cout << "result " <<  result << "," << ix << "," << iy << endl;
	assert(result == false);
	
	x1=0.0; y1=0.0;
	x2=10.0; y2=10.0;
	x3=0.0; y3=1.0;
	x4=10.0; y4=11.0;

	ix = -1.0; iy = -1.0;
	result = LineLineIntersect(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy);
	cout << "result " <<  result << "," << ix << "," << iy << endl;
	assert(result == false);

	// ** One horizontal line, one diagonal **

	x1=0.0; y1=3.0;
	x2=10.0; y2=3.0;
	x3=0.0; y3=0.0;
	x4=10.0; y4=10.0;

	ix = -1.0; iy = -1.0;
	result = LineLineIntersect(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy);
	cout << "result " <<  result << "," << ix << "," << iy << endl;
	assert(result == true);
	assert(fabs(ix - 3.0) < eps);
	assert(fabs(iy - 3.0) < eps);

	// ** One vertical line, one diagonal **

	x1=6.0; y1=-5.0;
	x2=6.0; y2=5.0;
	x3=0.0; y3=0.0;
	x4=10.0; y4=10.0;

	ix = -1.0; iy = -1.0;
	result = LineLineIntersect(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy);
	cout << "result " <<  result << "," << ix << "," << iy << endl;
	assert(result == true);
	assert(fabs(ix - 6.0) < eps);
	assert(fabs(iy - 6.0) < eps);

	// ** Two vertical lines **

	x1=6.0; y1=-5.0;
	x2=6.0; y2=5.0;
	x3=-4.0; y3=0.0;
	x4=-4.0; y4=10.0;

	ix = -1.0; iy = -1.0;
	result = LineLineIntersect(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy);
	cout << "result " <<  result << "," << ix << "," << iy << endl;
	assert(result == false);

	// ** Degenerate line **

	x1=6.0; y1=6.0;
	x2=x1; y2=y1;
	x3=-4.0; y3=0.0;
	x4=5.0; y4=10.0;

	ix = -1.0; iy = -1.0;
	result = LineLineIntersect(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy);
	cout << "result " <<  result << "," << ix << "," << iy << endl;
	assert(result == false);

}

