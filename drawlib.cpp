
#include "drawlib.h"

LocalStore::LocalStore() : IDrawLib()
{

}

LocalStore::~LocalStore()
{

}

void LocalStore::test()
{

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

