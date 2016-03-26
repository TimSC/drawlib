#ifndef _CAIRO_TWISTED_H
#define _CAIRO_TWISTED_H

#include <vector>
#include <utility>
#include <string>

void draw_formatted_twisted_text (cairo_t *cr, const std::string &text, const std::vector<TwistedCurveCmd> &cmds,
	const class TextProperties &properties);
void get_bounding_triangles_twisted_text (cairo_t *cr, const std::string &text, const std::vector<TwistedCurveCmd> &cmds,
	const class TextProperties &properties, TwistedTriangles &trianglesOut);
void fancy_cairo_draw_triangles(cairo_t *cr, TwistedTriangles &triangles);

#endif //_CAIRO_TWISTED_H

