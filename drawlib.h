#ifndef _DRAWLIB_H
#define _DRAWLIB_H

#include <vector>
#include <utility>
#include <string>

typedef std::pair<double, double> Point;
typedef std::vector<Point> Contour;
typedef std::vector<Contour> Contours;
typedef std::pair<Contour, Contours> Polygon;

///Enumeration of allowed command types
enum CmdTypes
{
	CMD_BASE,
	CMD_POLYGONS,
	CMD_LINES,
	CMD_TEXT,
};

///Drawing properties of shapes that are filled
class ShapeProperties
{
public:
	double r, g, b;

	ShapeProperties();
	ShapeProperties(double r, double g, double b);
	ShapeProperties(const ShapeProperties &arg);
	virtual ~ShapeProperties();
};

///Drawing properties of lines/strokes
class LineProperties
{
public:
	double r, g, b;
	double lineWidth;
	bool closedLoop;

	LineProperties();
	LineProperties(double r, double g, double b, double lineWidth=1.0);
	LineProperties(const LineProperties &arg);
	virtual ~LineProperties();
};

///Drawing properties of lines/strokes
class TextProperties
{
public:
	double r, g, b;
	std::string font;

	TextProperties();
	TextProperties(double r, double g, double b);
	TextProperties(const TextProperties &arg);
	virtual ~TextProperties();
};

///Base class of all command classes
class BaseCmd
{
public:
	const CmdTypes type;
	BaseCmd(CmdTypes type = CMD_BASE);
	BaseCmd(const BaseCmd &arg);
	virtual ~BaseCmd();
	virtual BaseCmd *Clone();
};

///Draw polygons command
class DrawPolygonsCmd : public BaseCmd
{
public:
	const std::vector<Polygon> polygons;
	const class ShapeProperties properties;

	DrawPolygonsCmd(const std::vector<Polygon> &polygons, const class ShapeProperties &properties);
	DrawPolygonsCmd(const DrawPolygonsCmd &arg);
	virtual ~DrawPolygonsCmd();
	virtual BaseCmd *Clone();
};

///Draw lines command
class DrawLinesCmd : public BaseCmd
{
public:
	const Contours lines;
	const class LineProperties properties;

	DrawLinesCmd(const Contours &lines, const class LineProperties &properties);
	DrawLinesCmd(const DrawLinesCmd &arg);
	virtual ~DrawLinesCmd();
	virtual BaseCmd *Clone();
};

///Draw text command
class DrawTextCmd : public BaseCmd
{
public:
	const std::vector<std::string> textStrs;
	const class TextProperties properties;

	DrawTextCmd(const std::vector<std::string> &textStrs, const class TextProperties &properties);
	DrawTextCmd(const DrawTextCmd &arg);
	virtual ~DrawTextCmd();
	virtual BaseCmd *Clone();
};

///Abstract base class of drawing library
class IDrawLib
{
public:
	IDrawLib() {};
	virtual ~IDrawLib() {};

	virtual void ClearDrawingCmds() = 0;
	virtual void AddCmd(class BaseCmd *cmd) = 0;
	virtual void AddDrawPolygonsCmd(const std::vector<Polygon> &polygons, const class ShapeProperties &properties) = 0;
	virtual void AddDrawLinesCmd(const Contours &lines, const class LineProperties &properties) = 0;
	virtual void AddDrawTextCmd(const std::vector<std::string> &textStrs, const class TextProperties &properties) = 0;

	virtual void Draw() {};
};

///Store all drawing commands in a memory buffer
class LocalStore : public IDrawLib
{
protected:
	std::vector<class BaseCmd *> cmds;
public:
	LocalStore();
	virtual ~LocalStore();

	void ClearDrawingCmds();
	void AddCmd(class BaseCmd *cmd);
	void AddDrawPolygonsCmd(const std::vector<Polygon> &polygons, const class ShapeProperties &properties);
	void AddDrawLinesCmd(const Contours &lines, const class LineProperties &properties);
	void AddDrawTextCmd(const std::vector<std::string> &textStrs, const class TextProperties &properties);
};

#endif //_DRAWLIB_H

