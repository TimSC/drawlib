#include "drawlibcairo.h"
#include <stdlib.h>
#include <pango/pangocairo.h>
#include <cmath>
#include <stdexcept>
#include <iostream>
using namespace std;

DrawLibCairo::DrawLibCairo(cairo_surface_t *surface): LocalStore(),
	surface(surface)
{
	this->cr = cairo_create(surface);
	this->maskSurface = NULL;
}

DrawLibCairo::~DrawLibCairo()
{
	cairo_destroy(this->cr);
	if(this->maskSurface != NULL)
		cairo_surface_destroy(maskSurface);
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

void DrawLibCairo::CreateMaskSurface(double width, double height)
{
	if(this->maskSurface != NULL) 
		cairo_surface_destroy(maskSurface);

	//Create mask surface
	this->maskSurface = cairo_image_surface_create (CAIRO_FORMAT_A8,
		                round(width),
		                round(height));
	if(cairo_surface_status(this->maskSurface)!=CAIRO_STATUS_SUCCESS)
	{
		cairo_restore(this->cr);
		throw runtime_error("Creating cairo surface failed");
	}
}

void DrawLibCairo::DrawCmdPolygons(class DrawPolygonsCmd &polygonsCmd)
{
	cairo_save (this->cr);
	const class ShapeProperties &properties = polygonsCmd.properties;

	//Draw outer polygons
	const std::vector<Polygon> &polygons = polygonsCmd.polygons;
	for(size_t i=0;i < polygons.size();i++)
	{
		const Polygon &polygon = polygons[i];
		const Contour &outer = polygon.first;
		const Contours &inners = polygon.second;

		double x1=0.0, x2=0.0, y1=0.0, y2=0.0;

		if(inners.size() > 0)
		{
			this->GetDrawableExtents(x1, y1, x2, y2);
			double width = x2 - x1;
			double height = y2 - y1;

			if(maskSurface == NULL)
				this->CreateMaskSurface(width, height);
			else
			{
				//Check if mask surface can be reused
				int msw = cairo_image_surface_get_width (maskSurface);
				int msh = cairo_image_surface_get_height (maskSurface);
				if(round(width) != msw || round(height) != msh)
				{
					//Surface must be recreated
					this->CreateMaskSurface(width, height);
				}
			}

			cairo_t *maskCr = cairo_create (maskSurface);
			cairo_set_operator (maskCr, CAIRO_OPERATOR_SOURCE);

			//Clear mask surface content
			cairo_set_source_rgba(maskCr, 0.0, 0.0, 0.0, 0.0);
			cairo_move_to(maskCr, 0.0, 0.0);
			cairo_line_to(maskCr, width, 0.0);
			cairo_line_to(maskCr, width, height);
			cairo_line_to(maskCr, 0.0, height);
			cairo_fill (maskCr);

			//Draw outer polygon to mask surface
			cairo_set_source_rgba(maskCr, 1.0, 1.0, 1.0, 1.0);
			if(outer.size() > 0) {
				cairo_move_to(maskCr, outer[0].first-x1, outer[0].second-y1);
				for(size_t pt=1;pt < outer.size();pt++)
					cairo_line_to(maskCr, outer[pt].first-x1, outer[pt].second-y1);
				cairo_fill (maskCr);
			}

			//Draw inner polygons to mask surface
			cairo_set_source_rgba(maskCr, 0.0, 0.0, 0.0, 0.0);
			for(size_t j=0; j < inners.size(); j++)
			{
				const Contour &inner = inners[j];
				if(inner.size() > 0) {
					cairo_move_to(maskCr, inner[0].first-x1, inner[0].second-y1);
					for(size_t pt=1;pt < inner.size();pt++)
						cairo_line_to(maskCr, inner[pt].first-x1, inner[pt].second-y1);
					cairo_fill (maskCr);
				}
			}

			cairo_surface_flush(maskSurface);
			cairo_destroy(maskCr);

			//Fill using mask surface
			cairo_set_source_rgba(cr, properties.r, properties.g, properties.b, properties.a);
			cairo_mask_surface(cr, maskSurface, x1, y1);
			cairo_fill (cr);
			
		}
		else
		{
			cairo_set_source_rgba(cr, properties.r, properties.g, properties.b, properties.a);

			//Draw outer polygon
			if(outer.size() > 0) {
				cairo_move_to(cr, outer[0].first, outer[0].second);
				for(size_t pt=1;pt < outer.size();pt++)
					cairo_line_to(cr, outer[pt].first, outer[pt].second);
				cairo_fill (cr);
			}
		}

	}
	cairo_restore(this->cr);
}

void DrawLibCairo::DrawCmdLines(class DrawLinesCmd &linesCmd)
{
	cairo_save (this->cr);
	const class LineProperties &properties = linesCmd.properties;
	cairo_set_source_rgba(cr, properties.r, properties.g, properties.b, properties.a);
	cairo_set_line_width (cr, properties.lineWidth);

	if(properties.lineCap == "butt") //cairo default
		cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
	if(properties.lineCap == "sqaure")
		cairo_set_line_cap (cr, CAIRO_LINE_CAP_SQUARE);
	if(properties.lineCap == "round")
		cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

	if(properties.lineJoin == "miter") //cairo default
		cairo_set_line_join (cr, CAIRO_LINE_JOIN_MITER);
	if(properties.lineJoin == "round") //cairo default
		cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);
	if(properties.lineJoin == "bevel") //cairo default
		cairo_set_line_join (cr, CAIRO_LINE_JOIN_BEVEL);

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
	cairo_set_source_rgba(cr, properties.r, properties.g, properties.b, properties.a);
	cairo_set_font_size(cr, properties.fontSize);
	cairo_select_font_face(cr, properties.font.c_str(), CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);

	if(!properties.outline){
		throw std::runtime_error("Not implemented");
	}

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

int DrawLibCairo::GetTextExtents(const char *textStr, const class TextProperties &properties, 
		double &width, double &height)
{
	cairo_save (this->cr);
	cairo_set_font_size(cr, properties.fontSize);
	cairo_select_font_face(cr, properties.font.c_str(), CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);

	cairo_text_extents_t extents;
	cairo_text_extents (cr,
                textStr,
                &extents);
	width = extents.width;
	height = extents.height;
	cairo_restore(this->cr);
	return 0;
}

int DrawLibCairo::GetDrawableExtents(double &x1,
	double &y1,
	double &x2,
	double &y2)
{
	cairo_clip_extents (this->cr, &x1, &y1, &x2, &y2);
	return 0; //Zero means ok, non-zero is not ok
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
	cairo_set_source_rgba(cr, properties.r, properties.g, properties.b, properties.a);

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

		if(!properties.outline){
			pango_cairo_show_layout (cr, layout);
		}
		else
		{
			cairo_set_line_width(cr, properties.lineWidth);
			pango_cairo_layout_path(cr, layout);
			cairo_stroke(cr);
		}

		g_object_unref (layout);
	}

	pango_font_description_free (desc);

	cairo_restore(this->cr);
}

int DrawLibCairoPango::GetTextExtents(const char *textStr, const class TextProperties &properties, 
		double &width, double &height)
{
	PangoFontDescription *desc = pango_font_description_from_string (properties.font.c_str());
	pango_font_description_set_size (desc, round(properties.fontSize * PANGO_SCALE));

	PangoLayout *layout = pango_cairo_create_layout (this->cr);

	pango_layout_set_text (layout, textStr, -1);
	pango_layout_set_font_description (layout, desc);

	PangoRectangle ink_rect;
	PangoRectangle logical_rect;
	pango_layout_get_pixel_extents (layout,
                            &ink_rect,
                            &logical_rect);

	width = logical_rect.width;
	height = logical_rect.height;

	g_object_unref (layout);

	pango_font_description_free (desc);
	return 0;
}

