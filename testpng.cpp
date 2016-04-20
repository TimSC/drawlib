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
	drawLib->AddDrawPolygonsCmd(polygons, prop);

	std::vector<Polygon> polygons2;
	polygons2.push_back(polygon2);
	class ShapeProperties prop2(0.5, 0.0, 0.0);
	drawLib->AddDrawPolygonsCmd(polygons2, prop2);

	Contour line1;
	line1.push_back(Point(100, 100));
	line1.push_back(Point(160, 110));
	line1.push_back(Point(60, 50));
	class LineProperties lineProp1(0.0, 0.9, 0.0, 1.0);
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
	class LineProperties lineProp2(0.0, 0.0, 1.0, 1.0);
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
	double pathLen = -1.0, textLen = -1.0;
	drawLib->GetTriangleBoundsTwistedText(TwistedTextLabel(arabic, pathCmds),
		properties2, 
		triangles, pathLen, textLen);

	Contour line3;
	line3.push_back(Point(320.0, 150.0));
	line3.push_back(Point(380.0, 210.0));
	line3.push_back(Point(440.0, 150.0));
	line3.push_back(Point(460.0, 150.0));
	line3.push_back(Point(460.0, 220.0));
	line3.push_back(Point(480.0, 240.0));
	line3.push_back(Point(550.0, 240.0));
	line3.push_back(Point(550.0, 200.0));
	line3.push_back(Point(560.0, 150.0));
	line3.push_back(Point(600.0, 170.0));
	class LineProperties lineProp3(0.0, 0.9, 0.0, 1.0);
	Contours lines3;
	lines3.push_back(line3);
	drawLib->AddDrawLinesCmd(lines3, lineProp3);

	//Test fitting Bezier to set of points
	const char *rooms = "All the rooms renumbered. All the rooms renumbered.";
	std::vector<TwistedCurveCmd> pathCmds2;
	FixBezierToPoints(line3, pathCmds2);
	class TextProperties properties3;
	properties3.fontSize = 15.0;
	properties3.valign = 0.8;
	std::vector<class TwistedTextLabel> twistedTextStrs2;
	twistedTextStrs2.push_back(TwistedTextLabel(rooms, pathCmds2));
	drawLib->AddDrawTwistedTextCmd(twistedTextStrs2, properties3);

	//Load image resources
	std::map<std::string, std::string> loadIdToFilenameMapping;
	loadIdToFilenameMapping["letterR"] = "tests/letterr.png";
	std::vector<std::string> unloadIds;
	drawLib->AddLoadImageResourcesCmd(loadIdToFilenameMapping, unloadIds);

	//Draw image to canvas
	class ShapeProperties prop3(1.0, 1.0, 1.0);
	prop3.imageId = "letterR";
	prop3.texx=-340.0;
	prop3.texy=-200.0;
	Contour outer3;
	outer3.push_back(Point(340, 200));
	outer3.push_back(Point(404, 200));
	outer3.push_back(Point(404, 264));
	outer3.push_back(Point(340, 264));
	Contour inner3;
	inner3.push_back(Point(360, 220));
	inner3.push_back(Point(384, 220));
	inner3.push_back(Point(384, 244));
	inner3.push_back(Point(360, 244));
	Contours inners3;
	inners3.push_back(inner3);
	Polygon polygon3(outer3, inners3);
	std::vector<Polygon> polygons3;
	polygons3.push_back(polygon3);
	drawLib->AddDrawPolygonsCmd(polygons3, prop3);

	drawLib->Draw();

}

int main(void)
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 640, 480);
	class DrawLibCairoPango drawlib(surface);
	//class DrawLibCairo drawlib(surface);
	
	DrawTestPatterns(&drawlib);

	cairo_surface_write_to_png(surface, "image.png");	
	cairo_surface_destroy(surface);
	return 0;
}

