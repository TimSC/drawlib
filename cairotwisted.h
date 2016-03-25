#ifndef _CAIRO_TWISTED_H
#define _CAIRO_TWISTED_H

#include <vector>
#include <utility>
#include <string>

void draw_pow (cairo_t *cr, const std::string &text, const std::vector<TwistedCurveCmd> &cmds);

#endif //_CAIRO_TWISTED_H

