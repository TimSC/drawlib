#ifndef _DRAWLIB_H
#define _DRAWLIB_H

#include <vector>
#include <utility>
#include <string>

typedef std::pair<double, double> Point;
typedef std::vector<Point> Contour;
typedef std::vector<Contour> Contours;
typedef std::pair<Contour, Contours> Polygon;
typedef std::vector<std::vector<Point> > TwistedTriangles;

///Enumeration of allowed command types
enum CmdTypes
{
	CMD_BASE,
	CMD_POLYGONS,
	CMD_LINES,
	CMD_TEXT,
	CMD_TWISTED_TEXT,
};

enum TwistedCurveCmdType
{
	MoveTo,
	LineTo,
	RelLineTo,
	CurveTo,
	RelCurveTo
};

typedef std::pair<TwistedCurveCmdType, std::vector<double> > TwistedCurveCmd;
TwistedCurveCmd NewTwistedCurveCmd(TwistedCurveCmdType ty, int n_args, ...);

///Drawing properties of shapes that are filled
class ShapeProperties
{
public:
	double r, g, b, a;

	ShapeProperties();
	ShapeProperties(double r, double g, double b);
	ShapeProperties(const ShapeProperties &arg);
	virtual ~ShapeProperties();

	bool operator <(const ShapeProperties& rhs) const;
};

///Drawing properties of lines/strokes
class LineProperties
{
public:
	double r, g, b, a;
	double lineWidth;
	bool closedLoop;
	std::string lineJoin, lineCap;

	LineProperties();
	LineProperties(double r, double g, double b, double lineWidth=1.0);
	LineProperties(const LineProperties &arg);
	virtual ~LineProperties();

	bool operator <(const LineProperties& rhs) const;
};

///Drawing properties of text drawing
class TextProperties
{
public:
	double lr, lg, lb, la; //Line colour
	double fr, fg, fb, fa; //Fill colour
	std::string font;
	double fontSize;
	bool outline;
	bool fill;
	double lineWidth;

	TextProperties();
	TextProperties(double fr, double fg, double fb);
	TextProperties(const TextProperties &arg);
	virtual ~TextProperties();

	bool operator <(const TextProperties& rhs) const;
};

///Defines a single label and positioning
class TextLabel
{
public:
	std::string text;
	double x, y; //Top left corner (unlike cairo)
	double ang; //ang in radians, clockwise

	TextLabel();
	TextLabel(std::string &text, double x, double y, double ang=0.0);
	TextLabel(const char *text, double x, double y, double ang=0.0);
	TextLabel(const TextLabel &arg);
	virtual ~TextLabel();
};

///Defines a single twisted label that follows a Bezier path
class TwistedTextLabel
{
public:
	std::string text;
	std::vector<TwistedCurveCmd> path; //Path of bottom edge

	TwistedTextLabel();
	TwistedTextLabel(std::string &text, const std::vector<TwistedCurveCmd> &path);
	TwistedTextLabel(const char *text, const std::vector<TwistedCurveCmd> &path);
	TwistedTextLabel(const TwistedTextLabel &arg);
	virtual ~TwistedTextLabel();
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
	const std::vector<class TextLabel> textStrs;
	const class TextProperties properties;

	DrawTextCmd(const std::vector<class TextLabel> &textStrs, const class TextProperties &properties);
	DrawTextCmd(const DrawTextCmd &arg);
	virtual ~DrawTextCmd();
	virtual BaseCmd *Clone();
};

///Draw twisted text command
class DrawTwistedTextCmd : public BaseCmd
{
public:
	const std::vector<class TwistedTextLabel> textStrs;
	const class TextProperties properties;

	DrawTwistedTextCmd(const std::vector<class TwistedTextLabel> &textStrs, const class TextProperties &properties);
	DrawTwistedTextCmd(const DrawTwistedTextCmd &arg);
	virtual ~DrawTwistedTextCmd();
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
	virtual void AddDrawTextCmd(const std::vector<class TextLabel> &textStrs, const class TextProperties &properties) = 0;
	virtual void AddDrawTwistedTextCmd(const std::vector<class TwistedTextLabel> &textStrs, const class TextProperties &properties) = 0;
	virtual int GetTextExtents(const char *textStr, const class TextProperties &properties, 
		double &width, double &height) = 0;
	virtual int GetDrawableExtents(double &x1,
		double &y1,
		double &x2,
		double &y2){x1=0.0; y1=0.0; x2 = 0.0; y2 = 0.0; return -1;}

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
	void AddDrawTextCmd(const std::vector<class TextLabel> &textStrs, const class TextProperties &properties);
	void AddDrawTwistedTextCmd(const std::vector<class TwistedTextLabel> &textStrs, const class TextProperties &properties);
	int GetTextExtents(const char *textStr, const class TextProperties &properties, 
		double &width, double &height);
};

#endif //_DRAWLIB_H

