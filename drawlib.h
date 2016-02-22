#ifndef _DRAWLIB_H
#define _DRAWLIB_H

#include <cairo/cairo.h>
#include <vector>
#include <utility>

typedef std::pair<double, double> Point;
typedef std::vector<Point> Contour;
typedef std::vector<Contour> Contours;
typedef std::pair<Contour, Contours> Polygon;

class IDrawLib
{
public:
	IDrawLib() {};
	virtual ~IDrawLib() {};

	virtual void ClearDrawingBuffer() = 0;
	virtual void SetColour(double red, double green, double blue) = 0;
	virtual void SetAlpha(double alpha) = 0;	
	virtual void SetLineWidth(double lineWidth) = 0;
	virtual void AddLines(const Contours &lines) = 0;	
	virtual void AddPolygons(const std::vector<Polygon> &polygons) = 0;

};

class LocalStore : public IDrawLib
{
protected:

public:
	LocalStore();
	virtual ~LocalStore();

	void ClearDrawingBuffer();
	void SetColour(double red, double green, double blue);
	void SetAlpha(double alpha);	
	void SetLineWidth(double lineWidth);
	void AddLines(const Contours &lines);	
	void AddPolygons(const std::vector<Polygon> &polygons);
};

class DrawLibCairo : public LocalStore
{
protected:
	cairo_t *cr;
	cairo_surface_t *surface;
public:
	DrawLibCairo(cairo_surface_t *surface);
	virtual ~DrawLibCairo();
	void test();

	void Draw();
};

#endif //_DRAWLIB_H

