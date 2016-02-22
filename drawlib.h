#ifndef _DRAWLIB_H
#define _DRAWLIB_H

#include <cairo/cairo.h>
#include <vector>
#include <utility>

typedef std::pair<double, double> Point;
typedef std::vector<Point> Contour;
typedef std::vector<Contour> Contours;
typedef std::pair<Contour, Contours> Polygon;

enum CmdTypes
{
	CMD_BASE,
	CMD_POLYGONS,
	CMD_LINES
};

class ShapeProperties
{
public:
	double r, g, b;

	ShapeProperties() {r=255.0; g=255.0; b=255.0;}
	ShapeProperties(double r, double g, double b): r(r), g(g), b(b) {}
	ShapeProperties(const ShapeProperties &arg) {r=arg.r; g=arg.g; b=arg.b;}
	virtual ~ShapeProperties() {};
};

class BaseCmd
{
public:
	const CmdTypes type;
	BaseCmd(CmdTypes type = CMD_BASE): type(type) {};
	BaseCmd(const BaseCmd &arg): type(arg.type) {};
	virtual ~BaseCmd() {};
	virtual BaseCmd *Clone() {return new class BaseCmd(*this);};
};

class DrawPolygonsCmd : public BaseCmd
{
public:
	const std::vector<Polygon> polygons;
	const class ShapeProperties properties;

	DrawPolygonsCmd(const std::vector<Polygon> &polygons, const class ShapeProperties &properties) : 
		BaseCmd(CMD_POLYGONS), polygons(polygons), properties(properties) {};
	DrawPolygonsCmd(const DrawPolygonsCmd &arg) : BaseCmd(CMD_POLYGONS), polygons(arg.polygons), properties(arg.properties) {};
	virtual ~DrawPolygonsCmd() {};
	virtual BaseCmd *Clone() {return new class DrawPolygonsCmd(*this);};
};

class DrawLinesCmd : public BaseCmd
{
public:
	const Contours lines;

	DrawLinesCmd(const Contours &lines) : BaseCmd(CMD_LINES), lines(lines) {};
	DrawLinesCmd(const DrawLinesCmd &arg) : BaseCmd(CMD_LINES), lines(arg.lines) {};
	virtual ~DrawLinesCmd() {};
	virtual BaseCmd *Clone() {return new class DrawLinesCmd(*this);};
};

class IDrawLib
{
public:
	IDrawLib() {};
	virtual ~IDrawLib() {};

	virtual void ClearDrawingCmds() = 0;
	virtual void AddCmd(class BaseCmd *cmd) = 0;


	virtual void Draw() {};
};

class LocalStore : public IDrawLib
{
protected:
	std::vector<class BaseCmd *> cmds;
public:
	LocalStore();
	virtual ~LocalStore();

	void ClearDrawingCmds();
	void AddCmd(class BaseCmd *cmd);
};

class DrawLibCairo : public LocalStore
{
protected:
	cairo_t *cr;
	cairo_surface_t *surface;

	void DrawCmdPolygons(class DrawPolygonsCmd &polygons);
public:
	DrawLibCairo(cairo_surface_t *surface);
	virtual ~DrawLibCairo();
	void test();

	void Draw();
	
};

#endif //_DRAWLIB_H

