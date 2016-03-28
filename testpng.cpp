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

	//Text labels
	std::vector<class TextLabel> textStrs;
	TwistedTriangles triangles;
	const char *hello = "Hello";
	const char *arabic = "السلام عليكم";
	class TextLabel label(hello, 60.0, 50.0);
	class TextLabel interlLabel(arabic, 50.0, 200.0);

	class TextProperties properties;
	properties.fontSize = 30.0;
	drawLib->GetTriangleBoundsText(label, properties, 
		triangles);
	drawLib->GetTriangleBoundsText(interlLabel, properties, 
		triangles);
	textStrs.push_back(label);
	textStrs.push_back(interlLabel);
	drawLib->AddDrawTextCmd(textStrs, properties);

	//Draw origin point
	
	Contour cross1;
	cross1.push_back(Point(55, 50));
	cross1.push_back(Point(65, 50));
	Contour cross2;
	cross2.push_back(Point(60, 45));
	cross2.push_back(Point(60, 55));
	class LineProperties lineProp2(0.0, 0.0, 1.0, 2.0);
	Contours lines2;
	lines2.push_back(cross1);
	lines2.push_back(cross2);
	drawLib->AddDrawLinesCmd(lines2, lineProp2);

	//More label tests

	textStrs.clear();
	class TextLabel label2("world", 170.0, 50.0);
	class TextProperties properties2;
	properties2.fontSize = 25.0;
	properties2.outline = true;
	properties2.fill = false;
	properties2.lr = 1.0;
	properties2.lg = 0.5;
	properties2.lb = 0.1;
	textStrs.push_back(label2);
	drawLib->AddDrawTextCmd(textStrs, properties2);

	textStrs.clear();
	class TextLabel label3("rotated", 60.0, 100.0, 0.5);
	textStrs.push_back(label3);
	drawLib->AddDrawTextCmd(textStrs, properties2);

	textStrs.clear();
	class TextLabel label4("rotated", 60.0, 100.0, 0.6);
	class TextLabel label5("foo", 180.0, 100.0, 0.0);
	properties.fa = 0.5;
	textStrs.push_back(label4);
	textStrs.push_back(label5);
	drawLib->AddDrawTextCmd(textStrs, properties);

	drawLib->GetTriangleBoundsText(label4, properties, 
		triangles);

	//Twisted text
	std::vector<class TwistedTextLabel> twistedTextStrs;
	std::vector<TwistedCurveCmd> pathCmds;
	pathCmds.push_back(NewTwistedCurveCmd(MoveTo, 2, 320.0, 100.0));
	pathCmds.push_back(NewTwistedCurveCmd(RelCurveTo, 6, 50.0, -50.0, 150.0, -50.0, 200.0, 0.0));
	twistedTextStrs.push_back(TwistedTextLabel(arabic, pathCmds));
	properties2.valign = 0.5;
	drawLib->AddDrawTwistedTextCmd(twistedTextStrs, properties2);

	//Test triangles for twisted text
	drawLib->GetTriangleBoundsTwistedText(TwistedTextLabel(arabic, pathCmds),
		properties2, 
		triangles);

	drawLib->Draw();

}

int main(void)
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 640, 240);
	class DrawLibCairoPango drawlib(surface);
	//class DrawLibCairo drawlib(surface);
	
	DrawTestPatterns(&drawlib);

	cairo_surface_write_to_png(surface, "image.png");	
	cairo_surface_destroy(surface);
	return 0;
}

