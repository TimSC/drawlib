#include <stdlib.h>
#include <iostream>
#include "drawlib.h"
using namespace std;

ShapeProperties::ShapeProperties() 
{r=1.0; g=1.0; b=1.0;}

ShapeProperties::ShapeProperties(double r, double g, double b): r(r), g(g), b(b) 
{}

ShapeProperties::ShapeProperties(const ShapeProperties &arg) 
{r=arg.r; g=arg.g; b=arg.b;}

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
	return false;
}

// *********************************************

LineProperties::LineProperties() 
{r=1.0; g=1.0; b=1.0; lineWidth=1.0; closedLoop = false;}

LineProperties::LineProperties(double r, double g, double b, double lineWidth): r(r), g(g), b(b), 
	lineWidth(lineWidth), closedLoop(false),
	lineJoin("miter"), lineCap("butt")
{}

LineProperties::LineProperties(const LineProperties &arg) : r(arg.r), g(arg.g), b(arg.b),
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
{r=1.0; g=1.0; b=1.0; fontSize=10.0; font="Sans";}

TextProperties::TextProperties(double r, double g, double b): r(r), g(g), b(b),
	fontSize(10.0), font("Sans")
{}

TextProperties::TextProperties(const TextProperties &arg)
{r=arg.r; g=arg.g; b=arg.b; fontSize=arg.fontSize; font=arg.font;}

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
	if(font < rhs.font) return true;
	if(font > rhs.font) return false;
	if(fontSize < rhs.fontSize) return true;
	if(fontSize > rhs.fontSize) return false;
	return false;
}

// *************************************

TextLabel::TextLabel() : x(0.0), y(0.0)
{}

TextLabel::TextLabel(std::string &text, double x, double y): text(text), x(x), y(y)
{}

TextLabel::TextLabel(const char *text, double x, double y): text(text), x(x), y(y)
{}

TextLabel::TextLabel(const TextLabel &arg):
	x(arg.x), y(arg.y), text(arg.text)
{}

TextLabel::~TextLabel()
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

int LocalStore::GetTextExtents(class TextLabel &textStr, const class TextProperties &properties, 
		double &width, double &height)
{
	width = -1.0;
	height = -1.0;
	return -1;
}

