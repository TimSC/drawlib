#ifndef _DRAW_LIB_CAIRO_H
#define _DRAW_LIB_CAIRO_H

#include <cairo/cairo.h>
#include "drawlib.h"

///Drawing with a cairo back end
class DrawLibCairo : public LocalStore
{
protected:
	cairo_t *cr;
	cairo_surface_t *surface;

	void DrawCmdPolygons(class DrawPolygonsCmd &polygons);
	void DrawCmdLines(class DrawLinesCmd &linesCmd);
public:
	DrawLibCairo(cairo_surface_t *surface);
	virtual ~DrawLibCairo();
	void test();

	void Draw();
	
};

#endif //_DRAW_LIB_CAIRO_H
