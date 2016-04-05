#ifndef _RDP_SIMPLIFY_H
#define _RDP_SIMPLIFY_H
#include "drawlib.h"

void RamerDouglasPeucker(const Contour &pointList, double epsilon, Contour &out);

#endif //_RDP_SIMPLIFY_H

