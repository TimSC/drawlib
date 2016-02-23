#include "drawlibcairo.h"
#include <stdlib.h>
#include <pango/pangocairo.h>
#include <cmath>
#include <iostream>
using namespace std;

DrawLibCairo::DrawLibCairo(cairo_surface_t *surface): LocalStore(),
	surface(surface)
{
	this->cr = cairo_create(surface);
}

DrawLibCairo::~DrawLibCairo()
{
	cairo_destroy(this->cr);
}

void DrawLibCairo::Draw()
{
	for(size_t i=0;i < cmds.size(); i++) {
		class BaseCmd *baseCmd = cmds[i];
		switch(baseCmd->type)
		{
		case CMD_POLYGONS:
			DrawCmdPolygons(*(class DrawPolygonsCmd *)baseCmd);
			break;
		case CMD_LINES:
			DrawCmdLines(*(class DrawLinesCmd *)baseCmd);
			break;
		case CMD_TEXT:
			DrawCmdText(*(class DrawTextCmd *)baseCmd);
			break;
		}

	}
}

void DrawLibCairo::DrawCmdPolygons(class DrawPolygonsCmd &polygonsCmd)
{
	cairo_save (this->cr);
	const class ShapeProperties &properties = polygonsCmd.properties;
	cairo_set_source_rgb(cr, properties.r, properties.g, properties.b);

	const std::vector<Polygon> &polygons = polygonsCmd.polygons;
	for(size_t i=0;i < polygons.size();i++)
	{
		const Polygon &polygon = polygons[i];
		const Contour &outer = polygon.first;
		if(outer.size() > 0)
			cairo_move_to(cr, outer[0].first, outer[0].second);
		for(size_t pt=1;pt < outer.size();pt++)
		{
			cairo_line_to(cr, outer[pt].first, outer[pt].second);
		}
		cairo_fill (cr);
	}
	cairo_restore(this->cr);
}

void DrawLibCairo::DrawCmdLines(class DrawLinesCmd &linesCmd)
{
	cairo_save (this->cr);
	const class LineProperties &properties = linesCmd.properties;
	cairo_set_source_rgb(cr, properties.r, properties.g, properties.b);
	cairo_set_line_width (cr, properties.lineWidth);

	const Contours &lines = linesCmd.lines;
	for(size_t i=0;i < lines.size();i++)
	{
		const Contour &contour = lines[i];
		if(contour.size() > 0)
			cairo_move_to(cr, contour[0].first, contour[0].second);
		for(size_t pt=1;pt < contour.size();pt++)
		{
			cairo_line_to(cr, contour[pt].first, contour[pt].second);
		}
		if(properties.closedLoop)
			cairo_close_path (cr);

		cairo_stroke (cr);
	}
	cairo_restore(this->cr);
}

void DrawLibCairo::DrawCmdText(class DrawTextCmd &textCmd)
{
	cairo_save (this->cr);
	const class TextProperties &properties = textCmd.properties;
	cairo_set_source_rgb(cr, properties.r, properties.g, properties.b);
	cairo_set_font_size(cr, properties.fontSize);
	cairo_select_font_face(cr, properties.font.c_str(), CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);

	const std::vector<class TextLabel> &textStrs = textCmd.textStrs;
	for(size_t i=0;i < textStrs.size();i++)
	{
		cairo_text_extents_t extents;
		cairo_text_extents (cr,
                    textStrs[i].text.c_str(),
                    &extents);

		cairo_move_to(cr, textStrs[i].x, textStrs[i].y + extents.height);
		cairo_show_text(cr, textStrs[i].text.c_str());
	}
	cairo_restore(this->cr);
}

// *****************************************************

DrawLibCairoPango::DrawLibCairoPango(cairo_surface_t *surface) : DrawLibCairo(surface)
{
	
}

DrawLibCairoPango::~DrawLibCairoPango()
{

}

void DrawLibCairoPango::DrawCmdText(class DrawTextCmd &textCmd)
{
	cairo_save (this->cr);
	const class TextProperties &properties = textCmd.properties;
	cairo_set_source_rgb(cr, properties.r, properties.g, properties.b);

	PangoFontDescription *desc = pango_font_description_from_string (properties.font.c_str());
	pango_font_description_set_size (desc, round(properties.fontSize * PANGO_SCALE));

	const std::vector<class TextLabel> &textStrs = textCmd.textStrs;
	for(size_t i=0;i < textStrs.size();i++)
	{
		PangoLayout *layout = pango_cairo_create_layout (cr);

		pango_layout_set_text (layout, textStrs[i].text.c_str(), -1);
		pango_layout_set_font_description (layout, desc);

		PangoRectangle ink_rect;
		PangoRectangle logical_rect;
		pango_layout_get_pixel_extents (layout,
                                &ink_rect,
                                &logical_rect);

		cairo_move_to(cr, textStrs[i].x - logical_rect.x, textStrs[i].y - logical_rect.y);
		pango_cairo_show_layout (cr, layout);

		g_object_unref (layout);
	}

	pango_font_description_free (desc);

	cairo_restore(this->cr);
}

