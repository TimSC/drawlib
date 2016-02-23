#include "drawlibcairo.h"

int main(void)
{
	cairo_surface_t *surface;


	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 320, 240);

	class DrawLibCairo drawlib(surface);
	
	Contour outer;
	outer.push_back(Point(50, 50));
	outer.push_back(Point(150, 50));
	outer.push_back(Point(150, 150));
	outer.push_back(Point(50, 150));
	Contours inners;
	Polygon polygon(outer, inners);

	Contour outer2;
	outer2.push_back(Point(51, 51));
	outer2.push_back(Point(149, 51));
	outer2.push_back(Point(149, 149));
	outer2.push_back(Point(51, 149));
	Contours inners2;
	Polygon polygon2(outer2, inners2);

	std::vector<Polygon> polygons;
	polygons.push_back(polygon);
	class ShapeProperties prop(1.0, 0.0, 0.0);
	class DrawPolygonsCmd cmd(polygons, prop);
	drawlib.AddDrawPolygonsCmd(polygons, prop);

	std::vector<Polygon> polygons2;
	polygons2.push_back(polygon2);
	class ShapeProperties prop2(0.5, 0.0, 0.0);
	drawlib.AddDrawPolygonsCmd(polygons2, prop2);

	Contour line1;
	line1.push_back(Point(100, 100));
	line1.push_back(Point(160, 110));
	line1.push_back(Point(200, 170));
	class LineProperties lineProp1(0.0, 0.9, 0.0, 3.0);
	lineProp1.closedLoop = true;
	Contours lines1;
	lines1.push_back(line1);
	drawlib.AddDrawLinesCmd(lines1, lineProp1);

	std::vector<std::string> textStrs;
	textStrs.push_back("spam");
	class TextProperties properties;
	drawlib.AddDrawTextCmd(textStrs, properties);

	drawlib.Draw();

	cairo_surface_write_to_png(surface, "image.png");	
	cairo_surface_destroy(surface);

	return 0;
}

