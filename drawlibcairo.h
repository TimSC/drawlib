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

	virtual void DrawCmdPolygons(class DrawPolygonsCmd &polygons);
	virtual void DrawCmdLines(class DrawLinesCmd &linesCmd);
	virtual void DrawCmdText(class DrawTextCmd &textCmd);
public:
	DrawLibCairo(cairo_surface_t *surface);
	virtual ~DrawLibCairo();

	void Draw();
	
};

class DrawLibCairoPango : public DrawLibCairo
{
public:
	DrawLibCairoPango(cairo_surface_t *surface);
	virtual ~DrawLibCairoPango();

	void DrawCmdText(class DrawTextCmd &textCmd);
};

#endif //_DRAW_LIB_CAIRO_H
