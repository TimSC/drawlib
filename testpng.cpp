#include "drawlibcairo.h"
#include <iostream>
using namespace std;

void DrawTestPatterns(class IDrawLib *drawLib)
{
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
	drawLib->AddDrawPolygonsCmd(polygons, prop);

	std::vector<Polygon> polygons2;
	polygons2.push_back(polygon2);
	class ShapeProperties prop2(0.5, 0.0, 0.0);
	drawLib->AddDrawPolygonsCmd(polygons2, prop2);

	Contour line1;
	line1.push_back(Point(100, 100));
	line1.push_back(Point(160, 110));
	line1.push_back(Point(60, 50));
	class LineProperties lineProp1(0.0, 0.9, 0.0, 3.0);
	lineProp1.closedLoop = true;
	Contours lines1;
	lines1.push_back(line1);
	drawLib->AddDrawLinesCmd(lines1, lineProp1);

	//Text extents
	double width = 0.0, height = 0.0;
	class TextLabel label("Spam", 60.0, 50.0);
	class TextProperties properties;
	properties.fontSize = 30.0;
	drawLib->GetTextExtents("Spam", properties, 
		width, height);
	cout << "text w: "<< width << ", h: " << height << endl;

	std::vector<class TextLabel> textStrs;
	textStrs.push_back(label);
	drawLib->AddDrawTextCmd(textStrs, properties);
	drawLib->Draw();

}

int main(void)
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 320, 240);
	class DrawLibCairoPango drawlib(surface);
	//class DrawLibCairo drawlib(surface);
	
	DrawTestPatterns(&drawlib);

	cairo_surface_write_to_png(surface, "image.png");	
	cairo_surface_destroy(surface);
	return 0;
}

