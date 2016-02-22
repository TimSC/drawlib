#ifndef _DRAWLIB_H
#define _DRAWLIB_H

#include <cairo/cairo.h>

class IDrawLib
{
public:
	IDrawLib() {};
	virtual ~IDrawLib() {};
	virtual void test() = 0;
};

class LocalStore : public IDrawLib
{
protected:

public:
	LocalStore();
	virtual ~LocalStore();
	void test();
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
};

#endif //_DRAWLIB_H

