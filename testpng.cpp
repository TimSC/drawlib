#include <cairo/cairo.h>

int main(void)
{
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 320, 240);
	cr = cairo_create(surface);

	cairo_set_source_rgb(cr, 255, 0, 0);
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 30.0);

	cairo_move_to(cr, 20.0, 60.0);
	cairo_show_text(cr, "Foobar.");

	cairo_surface_write_to_png(surface, "image.png");

	cairo_destroy(cr);
	cairo_surface_destroy(surface);

	return 0;
}

