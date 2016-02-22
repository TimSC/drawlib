#include "drawlib.h"

int main(void)
{
	cairo_surface_t *surface;


	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 320, 240);

	class DrawLibCairo drawlib(surface);
	drawlib.test();

	cairo_surface_write_to_png(surface, "image.png");	
	cairo_surface_destroy(surface);

	return 0;
}

