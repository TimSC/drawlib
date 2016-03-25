#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <stdarg.h>
#include "drawlib.h"
using namespace std;

ShapeProperties::ShapeProperties() 
{r=1.0; g=1.0; b=1.0; a=1.0;}

ShapeProperties::ShapeProperties(double r, double g, double b): r(r), g(g), b(b), a(1.0) 
{}

ShapeProperties::ShapeProperties(const ShapeProperties &arg) 
{r=arg.r; g=arg.g; b=arg.b; a=arg.a;}

ShapeProperties::~ShapeProperties() 
{}

bool ShapeProperties::operator <(const ShapeProperties& rhs) const
{
	if(r < rhs.r) return true;
	if(r > rhs.r) return false;
	if(g < rhs.g) return true;
	if(g > rhs.g) return false;
	if(b < rhs.b) return true;
	if(b > rhs.b) return false;
	if(a < rhs.a) return true;
	if(a > rhs.a) return false;
	return false;
}

// *********************************************

LineProperties::LineProperties() 
{r=1.0; g=1.0; b=1.0; a=1.0; lineWidth=1.0; closedLoop = false;}

LineProperties::LineProperties(double r, double g, double b, double lineWidth): r(r), g(g), b(b), a(1.0),
	lineWidth(lineWidth), closedLoop(false),
	lineJoin("miter"), lineCap("butt")
{}

LineProperties::LineProperties(const LineProperties &arg) : r(arg.r), g(arg.g), b(arg.b), a(arg.a),
	lineWidth(arg.lineWidth), closedLoop(arg.closedLoop),
	lineJoin(arg.lineJoin), lineCap(arg.lineCap)
{}

LineProperties::~LineProperties()
{}

bool LineProperties::operator <(const LineProperties& rhs) const
{
	if(r < rhs.r) return true;
	if(r > rhs.r) return false;
	if(g < rhs.g) return true;
	if(g > rhs.g) return false;
	if(b < rhs.b) return true;
	if(b > rhs.b) return false;
	if(a < rhs.a) return true;
	if(a > rhs.a) return false;
	if(lineWidth < rhs.lineWidth) return true;
	if(lineWidth > rhs.lineWidth) return false;
	if(closedLoop < rhs.closedLoop) return true;
	if(closedLoop > rhs.closedLoop) return false;
	if(lineJoin < rhs.lineJoin) return true;
	if(lineJoin > rhs.lineJoin) return false;
	if(lineCap < rhs.lineCap) return true;
	if(lineCap > rhs.lineCap) return false;
	return false;
}

// *********************************************

TextProperties::TextProperties()
{r=1.0; g=1.0; b=1.0; a=1.0; fontSize=10.0; font="Sans"; outline = false; lineWidth = 1.0;
}

TextProperties::TextProperties(double r, double g, double b): r(r), g(g), b(b), a(1.0),
	fontSize(10.0), font("Sans"), outline(false), lineWidth(1.0)
{}

TextProperties::TextProperties(const TextProperties &arg)
{
	r=arg.r; g=arg.g; b=arg.b; a=arg.a; fontSize=arg.fontSize; font=arg.font; 
	outline=arg.outline; lineWidth=arg.lineWidth;
}

TextProperties::~TextProperties()
{}

bool TextProperties::operator <(const TextProperties& rhs) const
{
	if(r < rhs.r) return true;
	if(r > rhs.r) return false;
	if(g < rhs.g) return true;
	if(g > rhs.g) return false;
	if(b < rhs.b) return true;
	if(b > rhs.b) return false;
	if(a < rhs.a) return true;
	if(a > rhs.a) return false;
	if(font < rhs.font) return true;
	if(font > rhs.font) return false;
	if(fontSize < rhs.fontSize) return true;
	if(fontSize > rhs.fontSize) return false;
	if(outline < rhs.outline) return true;
	if(outline > rhs.outline) return false;
	if(lineWidth < rhs.lineWidth) return true;
	if(lineWidth > rhs.lineWidth) return false;

	return false;
}

// *************************************

TextLabel::TextLabel() : x(0.0), y(0.0), ang(0.0)
{}

TextLabel::TextLabel(std::string &text, double x, double y, double ang): text(text), x(x), y(y), ang(ang)
{}

TextLabel::TextLabel(const char *text, double x, double y, double ang): text(text), x(x), y(y), ang(ang)
{}

TextLabel::TextLabel(const TextLabel &arg):
	x(arg.x), y(arg.y), text(arg.text), ang(arg.ang)
{}

TextLabel::~TextLabel()
{}

// *************************************

TwistedTextLabel::TwistedTextLabel()
{}

TwistedTextLabel::TwistedTextLabel(std::string &text, const std::vector<TwistedCurveCmd> &path): text(text), path(path)
{}

TwistedTextLabel::TwistedTextLabel(const char *text, const std::vector<TwistedCurveCmd> &path): text(text), path(path)
{}

TwistedTextLabel::TwistedTextLabel(const TwistedTextLabel &arg):
	text(arg.text), path(arg.path)
{}

TwistedTextLabel::~TwistedTextLabel()
{}

// *************************************

BaseCmd::BaseCmd(CmdTypes type): type(type)
{}

BaseCmd::BaseCmd(const BaseCmd &arg): type(arg.type)
{}

BaseCmd::~BaseCmd()
{}

BaseCmd *BaseCmd::Clone()
{return new class BaseCmd(*this);}

DrawPolygonsCmd::DrawPolygonsCmd(const std::vector<Polygon> &polygons, const class ShapeProperties &properties) : 
	BaseCmd(CMD_POLYGONS), polygons(polygons), properties(properties)
{}

DrawPolygonsCmd::DrawPolygonsCmd(const DrawPolygonsCmd &arg) : BaseCmd(CMD_POLYGONS), polygons(arg.polygons), properties(arg.properties) 
{}

DrawPolygonsCmd::~DrawPolygonsCmd() 
{}

BaseCmd *DrawPolygonsCmd::Clone()
{return new class DrawPolygonsCmd(*this);}

DrawLinesCmd::DrawLinesCmd(const Contours &lines, const class LineProperties &properties) : BaseCmd(CMD_LINES), 
	lines(lines), properties(properties) 
{}

DrawLinesCmd::DrawLinesCmd(const DrawLinesCmd &arg) : BaseCmd(CMD_LINES), lines(arg.lines), properties(arg.properties)
{}

DrawLinesCmd::~DrawLinesCmd()
{}

BaseCmd *DrawLinesCmd::Clone()
{return new class DrawLinesCmd(*this);}

DrawTextCmd::DrawTextCmd(const std::vector<class TextLabel> &textStrs, const class TextProperties &properties) : BaseCmd(CMD_TEXT), 
	textStrs(textStrs), properties(properties) 
{}

DrawTextCmd::DrawTextCmd(const DrawTextCmd &arg) : BaseCmd(CMD_TEXT), textStrs(arg.textStrs), properties(arg.properties)
{}

DrawTextCmd::~DrawTextCmd()
{}

BaseCmd *DrawTextCmd::Clone()
{return new class DrawTextCmd(*this);}

DrawTwistedTextCmd::DrawTwistedTextCmd(const std::vector<class TwistedTextLabel> &textStrs, const class TextProperties &properties) : 
	BaseCmd(CMD_TWISTED_TEXT), textStrs(textStrs), properties(properties) 
{}

DrawTwistedTextCmd::DrawTwistedTextCmd(const DrawTwistedTextCmd &arg):
	BaseCmd(CMD_TWISTED_TEXT), textStrs(arg.textStrs), properties(arg.properties)
{}

DrawTwistedTextCmd::~DrawTwistedTextCmd()
{}

BaseCmd *DrawTwistedTextCmd::Clone()
{return new class DrawTwistedTextCmd(*this);}

// *************************************

LocalStore::LocalStore() : IDrawLib()
{

}

LocalStore::~LocalStore()
{
	ClearDrawingCmds();
}

void LocalStore::ClearDrawingCmds()
{
	for(size_t i=0;i < cmds.size(); i++)
		delete cmds[i];
	cmds.clear();
}


void LocalStore::AddCmd(class BaseCmd *cmd)
{
	cmds.push_back(cmd->Clone());
}

void LocalStore::AddDrawPolygonsCmd(const std::vector<Polygon> &polygons, const class ShapeProperties &properties)
{
	class DrawPolygonsCmd cmd(polygons, properties);
	this->AddCmd(&cmd);
}

void LocalStore::AddDrawLinesCmd(const Contours &lines, const class LineProperties &properties)
{
	class DrawLinesCmd cmd(lines, properties);
	this->AddCmd(&cmd);
}

void LocalStore::AddDrawTextCmd(const std::vector<class TextLabel> &textStrs, const class TextProperties &properties)
{
	class DrawTextCmd cmd(textStrs, properties);
	this->AddCmd(&cmd);
}

void LocalStore::AddDrawTwistedTextCmd(const std::vector<class TwistedTextLabel> &textStrs, const class TextProperties &properties)
{
	class DrawTwistedTextCmd cmd(textStrs, properties);
	this->AddCmd(&cmd);
}

int LocalStore::GetTextExtents(const char *textStr, const class TextProperties &properties, 
		double &width, double &height)
{
	width = -1.0;
	height = -1.0;
	return -1;
}

// ****************************************

///Convenience factory to create a curve command
TwistedCurveCmd NewTwistedCurveCmd(TwistedCurveCmdType ty, int n_args, ...)
{

	switch(ty)
	{
	case CurveTo:
	case RelCurveTo:
		if(n_args != 6) throw std::invalid_argument("Incorrect number of arguments");
		break;
	default:
		if(n_args != 2) throw std::invalid_argument("Incorrect number of arguments");		
		break;
	}

	va_list ap;
	va_start(ap, n_args);
	std::vector<double> vals;
	for(int i=0;i < n_args;i++)
		vals.push_back(va_arg(ap, double));
	va_end(ap);
	return TwistedCurveCmd(ty, vals);
}

