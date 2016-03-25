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
	cairo_surface_t *maskSurface;

	virtual void DrawCmdPolygons(class DrawPolygonsCmd &polygons);
	virtual void DrawCmdLines(class DrawLinesCmd &linesCmd);
	virtual void DrawCmdText(class DrawTextCmd &textCmd);
	virtual void DrawCmdTwistedText(class DrawTwistedTextCmd &textCmd);

	void CreateMaskSurface(double width, double height);
public:
	DrawLibCairo(cairo_surface_t *surface);
	virtual ~DrawLibCairo();

	void Draw();
	int GetTextExtents(const char *textStr, const class TextProperties &properties, 
		double &width, double &height);
	int GetDrawableExtents(double &x1,
		double &y1,
		double &x2,
		double &y2);
};

class DrawLibCairoPango : public DrawLibCairo
{
protected:
	void DrawCmdText(class DrawTextCmd &textCmd);
	void DrawCmdTwistedText(class DrawTwistedTextCmd &textCmd);

public:
	DrawLibCairoPango(cairo_surface_t *surface);
	virtual ~DrawLibCairoPango();

	int GetTextExtents(const char *textStr, const class TextProperties &properties, 
		double &width, double &height);
};

#endif //_DRAW_LIB_CAIRO_H
