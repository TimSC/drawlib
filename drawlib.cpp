#include <stdlib.h>
#include <iostream>
#include "drawlib.h"
using namespace std;

LocalStore::LocalStore() : IDrawLib()
{

}

LocalStore::~LocalStore()
{
	ClearDrawingCmds();
}

void LocalStore::ClearDrawingCmds()
{
	for(size_t i=0;i < cmds.size(); i++)
		delete cmds[i];
	cmds.clear();
}


void LocalStore::AddCmd(class BaseCmd *cmd)
{
	cmds.push_back(cmd->Clone());
}

// **********************************

DrawLibCairo::DrawLibCairo(cairo_surface_t *surface): LocalStore(),
	surface(surface)
{
	cr = cairo_create(surface);
}

DrawLibCairo::~DrawLibCairo()
{
	cairo_destroy(cr);
}

void DrawLibCairo::test()
{
	cairo_set_source_rgb(cr, 255, 0, 0);
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 30.0);

	cairo_move_to(cr, 20.0, 60.0);
	cairo_show_text(cr, "Foobar.");
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
			break;
		}

	}
}

void DrawLibCairo::DrawCmdPolygons(class DrawPolygonsCmd &polygonsCmd)
{
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
}

