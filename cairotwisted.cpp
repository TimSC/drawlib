/* 
 * Originally written by Behdad Esfahbod, 2006..2007
 * From https://github.com/phuang/pango/blob/master/examples/cairotwisted.c
 * 
 * Adapted by Tim Sheerman-Chase. See LICENSE for license info.
 *
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pango/pangocairo.h>
#include <stdexcept>
#include "drawlib.h"
#include "cairotwisted.h"

void fancy_cairo_stroke (cairo_t *cr);
void fancy_cairo_stroke_preserve (cairo_t *cr);

/* A fancy cairo_stroke[_preserve]() that draws points and control
 * points, and connects them together.
 */
static void
_fancy_cairo_stroke (cairo_t *cr, cairo_bool_t preserve)
{
	int i;
	double line_width;
	cairo_path_t *path;
	cairo_path_data_t *data;
	const double dash[] = {10, 10};

	cairo_save (cr);
	cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);

	line_width = cairo_get_line_width (cr);
	path = cairo_copy_path (cr);
	cairo_new_path (cr);

	cairo_save (cr);
	cairo_set_line_width (cr, line_width / 3);
	cairo_set_dash (cr, dash, G_N_ELEMENTS (dash), 0);
	for (i=0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];
		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
		case CAIRO_PATH_LINE_TO:
	cairo_move_to (cr, data[1].point.x, data[1].point.y);
	break;
		case CAIRO_PATH_CURVE_TO:
	cairo_line_to (cr, data[1].point.x, data[1].point.y);
	cairo_move_to (cr, data[2].point.x, data[2].point.y);
	cairo_line_to (cr, data[3].point.x, data[3].point.y);
	break;
		case CAIRO_PATH_CLOSE_PATH:
	break;
		default:
	g_assert_not_reached ();
		}
	}
	cairo_stroke (cr);
	cairo_restore (cr);

	cairo_save (cr);
	cairo_set_line_width (cr, line_width * 4);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
	for (i=0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];
		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			cairo_move_to (cr, data[1].point.x, data[1].point.y);
			break;
		case CAIRO_PATH_LINE_TO:
			cairo_rel_line_to (cr, 0, 0);
			cairo_move_to (cr, data[1].point.x, data[1].point.y);
			break;
		case CAIRO_PATH_CURVE_TO:
			cairo_rel_line_to (cr, 0, 0);
			cairo_move_to (cr, data[1].point.x, data[1].point.y);
			cairo_rel_line_to (cr, 0, 0);
			cairo_move_to (cr, data[2].point.x, data[2].point.y);
			cairo_rel_line_to (cr, 0, 0);
			cairo_move_to (cr, data[3].point.x, data[3].point.y);
			break;
		case CAIRO_PATH_CLOSE_PATH:
			cairo_rel_line_to (cr, 0, 0);
			break;
		default:
			g_assert_not_reached ();
		}
	}
	cairo_rel_line_to (cr, 0, 0);
	cairo_stroke (cr);
	cairo_restore (cr);

	for (i=0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];
		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			cairo_move_to (cr, data[1].point.x, data[1].point.y);
			break;
		case CAIRO_PATH_LINE_TO:
			cairo_line_to (cr, data[1].point.x, data[1].point.y);
			break;
		case CAIRO_PATH_CURVE_TO:
			cairo_curve_to (cr, data[1].point.x, data[1].point.y,
							data[2].point.x, data[2].point.y,
							data[3].point.x, data[3].point.y);
			break;
		case CAIRO_PATH_CLOSE_PATH:
			cairo_close_path (cr);
			break;
		default:
			g_assert_not_reached ();
		}
	}
	cairo_stroke (cr);

	if (preserve)
		cairo_append_path (cr, path);

	cairo_path_destroy (path);

	cairo_restore (cr);
}

/* A fancy cairo_stroke() that draws points and control points, and
 * connects them together.
 */
void
fancy_cairo_stroke (cairo_t *cr)
{
	_fancy_cairo_stroke (cr, FALSE);
}

/* A fancy cairo_stroke_preserve() that draws points and control
 * points, and connects them together.
 */
void
fancy_cairo_stroke_preserve (cairo_t *cr)
{
	_fancy_cairo_stroke (cr, TRUE);
}

void fancy_cairo_draw_triangles(cairo_t *cr, TwistedTriangles &triangles)
{
	for(size_t i = 0; i < triangles.size(); i++)
	{
		std::vector<Point> &tri = triangles[i];
		if(tri.size() >= 1)
		{
			cairo_move_to (cr, tri[0].first, tri[0].second);
			for(size_t j=1; j < tri.size(); j++) {
				cairo_line_to (cr, tri[j].first, tri[j].second);
			}
			cairo_close_path (cr);
			cairo_stroke (cr);
		}
	}	
}

/* Returns Euclidean distance between two points */
static double
two_points_distance (cairo_path_data_t *a, cairo_path_data_t *b)
{
	double dx, dy;

	dx = b->point.x - a->point.x;
	dy = b->point.y - a->point.y;

	return sqrt (dx * dx + dy * dy);
}

/* Returns length of a Bezier curve.
 * Seems like computing that analytically is not easy.	The
 * code just flattens the curve using cairo and adds the length
 * of segments.
 */
static double
curve_length (double x0, double y0,
				double x1, double y1,
				double x2, double y2,
				double x3, double y3)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_path_t *path;
	cairo_path_data_t *data, current_point;
	int i;
	double length;

	surface = cairo_image_surface_create (CAIRO_FORMAT_A8, 0, 0);
	cr = cairo_create (surface);
	cairo_surface_destroy (surface);

	cairo_move_to (cr, x0, y0);
	cairo_curve_to (cr, x1, y1, x2, y2, x3, y3);

	length = 0;
	path = cairo_copy_path_flat (cr);
	for (i=0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];
		switch (data->header.type) {

		case CAIRO_PATH_MOVE_TO:
			current_point = data[1];
			break;

		case CAIRO_PATH_LINE_TO:
			length += two_points_distance (&current_point, &data[1]);
			current_point = data[1];
			break;

		default:
		case CAIRO_PATH_CURVE_TO:
		case CAIRO_PATH_CLOSE_PATH:
			g_assert_not_reached ();
		}
	}
	cairo_path_destroy (path);

	cairo_destroy (cr);

	return length;
}


typedef double parametrization_t;

/* Compute parametrization info.	That is, for each part of the 
 * cairo path, tags it with its length.
 */
void parametrize_path (cairo_path_t *path, std::vector<parametrization_t> &parametrizationOut)
{
	int i;
	cairo_path_data_t *data, last_move_to, current_point;
	parametrization_t *parametrization;

	parametrizationOut.resize(path->num_data);

	for (i=0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];
		parametrizationOut[i] = 0.0;
		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			last_move_to = data[1];
			current_point = data[1];
			break;
		case CAIRO_PATH_CLOSE_PATH:
			/* Make it look like it's a line_to to last_move_to */
			data = (&last_move_to) - 1;
			/* fall through */
		case CAIRO_PATH_LINE_TO:
			parametrizationOut[i] = two_points_distance (&current_point, &data[1]);
			current_point = data[1];
			break;
		case CAIRO_PATH_CURVE_TO:
			/* naive curve-length, treating bezier as three line segments:
			parametrization[i] = two_points_distance (&current_point, &data[1])
						 + two_points_distance (&data[1], &data[2])
						 + two_points_distance (&data[2], &data[3]);
			*/
			parametrizationOut[i] = curve_length (current_point.point.x, current_point.point.x,
								 data[1].point.x, data[1].point.y,
								 data[2].point.x, data[2].point.y,
								 data[3].point.x, data[3].point.y);

			current_point = data[3];
			break;
		default:
			g_assert_not_reached ();
		}
	}
}

typedef void (*transform_point_func_t) (void *closure, double *x, double *y);

/* Project a path using a function.	Each point of the path (including
 * Bezier control points) is passed to the function for transformation.
 */
static void
transform_path (cairo_path_t *path, transform_point_func_t f, void *closure)
{
	int i;
	cairo_path_data_t *data;

	for (i=0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];
		switch (data->header.type) {
		case CAIRO_PATH_CURVE_TO:
			f (closure, &data[3].point.x, &data[3].point.y);
			f (closure, &data[2].point.x, &data[2].point.y);
		case CAIRO_PATH_MOVE_TO:
		case CAIRO_PATH_LINE_TO:
			f (closure, &data[1].point.x, &data[1].point.y);
			break;
		case CAIRO_PATH_CLOSE_PATH:
			break;
		default:
			g_assert_not_reached ();
		}
	}
}

/* Simple struct to hold a path and its parametrization */
typedef struct {
	cairo_path_t *path;
	std::vector<parametrization_t> parametrization;
} parametrized_path_t;

/* Project a point X,Y onto a parameterized path.	The final point is
 * where you get if you walk on the path forward from the beginning for X
 * units, then stop there and walk another Y units perpendicular to the
 * path at that point.	In more detail:
 *
 * There's three pieces of math involved:
 *
 *	 - The parametric form of the Line equation
 *		 http://en.wikipedia.org/wiki/Line
 *
 *	 - The parametric form of the Cubic Bézier curve equation
 *		 http://en.wikipedia.org/wiki/B%C3%A9zier_curve
 *
 *	 - The Gradient (aka multi-dimensional derivative) of the above
 *		 http://en.wikipedia.org/wiki/Gradient
 *
 * The parametric forms are used to answer the question of "where will I be
 * if I walk a distance of X on this path".	The Gradient is used to answer
 * the question of "where will I be if then I stop, rotate left for 90
 * degrees and walk straight for a distance of Y".
 */
static void
point_on_path (parametrized_path_t *param,
				 double *x, double *y)
{
	int i;
	double ratio, the_y = *y, the_x = *x, dx, dy;
	cairo_path_data_t *data, last_move_to, current_point;
	cairo_path_t *path = param->path;
	std::vector<parametrization_t> &parametrization = param->parametrization;

	for (i=0; i + path->data[i].header.length < path->num_data &&
			(the_x > parametrization[i] ||
			 path->data[i].header.type == CAIRO_PATH_MOVE_TO);
			 i += path->data[i].header.length) {
		the_x -= parametrization[i];
		data = &path->data[i];
		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			current_point = data[1];
			last_move_to = data[1];
			break;
		case CAIRO_PATH_LINE_TO:
			current_point = data[1];
			break;
		case CAIRO_PATH_CURVE_TO:
			current_point = data[3];
			break;
		case CAIRO_PATH_CLOSE_PATH:
			break;
		default:
			g_assert_not_reached ();
		}
	}
	data = &path->data[i];

	switch (data->header.type) {

	case CAIRO_PATH_MOVE_TO:
			break;
	case CAIRO_PATH_CLOSE_PATH:
			/* Make it look like it's a line_to to last_move_to */
			data = (&last_move_to) - 1;
			/* fall through */
	case CAIRO_PATH_LINE_TO:
			{
	ratio = the_x / parametrization[i];
	/* Line polynomial */
	*x = current_point.point.x * (1 - ratio) + data[1].point.x * ratio;
	*y = current_point.point.y * (1 - ratio) + data[1].point.y * ratio;

	/* Line gradient */
	dx = -(current_point.point.x - data[1].point.x);
	dy = -(current_point.point.y - data[1].point.y);

	/*optimization for: ratio = the_y / sqrt (dx * dx + dy * dy);*/
	ratio = the_y / parametrization[i];
	*x += -dy * ratio;
	*y +=	dx * ratio;
			}
			break;
	case CAIRO_PATH_CURVE_TO:
			{
	/* FIXME the formulas here are not exactly what we want, because the
	 * Bezier parametrization is not uniform.	But I don't know how to do
	 * better.	The caller can do slightly better though, by flattening the
	 * Bezier and avoiding this branch completely.	That has its own cost
	 * though, as large y values magnify the flattening error drastically.
	 */

	double ratio_1_0, ratio_0_1;
	double ratio_2_0, ratio_0_2;
	double ratio_3_0, ratio_2_1, ratio_1_2, ratio_0_3;
	double _1__4ratio_1_0_3ratio_2_0, _2ratio_1_0_3ratio_2_0;

	ratio = the_x / parametrization[i];

	ratio_1_0 = ratio;
	ratio_0_1 = 1 - ratio;

	ratio_2_0 = ratio_1_0 * ratio_1_0; /*			ratio	*			ratio	*/
	ratio_0_2 = ratio_0_1 * ratio_0_1; /* (1 - ratio) * (1 - ratio) */

	ratio_3_0 = ratio_2_0 * ratio_1_0; /*			ratio	*			ratio	*			ratio	*/
	ratio_2_1 = ratio_2_0 * ratio_0_1; /*			ratio	*			ratio	* (1 - ratio) */
	ratio_1_2 = ratio_1_0 * ratio_0_2; /*			ratio	* (1 - ratio) * (1 - ratio) */
	ratio_0_3 = ratio_0_1 * ratio_0_2; /* (1 - ratio) * (1 - ratio) * (1 - ratio) */

	_1__4ratio_1_0_3ratio_2_0 = 1 - 4 * ratio_1_0 + 3 * ratio_2_0;
	_2ratio_1_0_3ratio_2_0		=		 2 * ratio_1_0 - 3 * ratio_2_0;

	/* Bezier polynomial */
	*x = current_point.point.x * ratio_0_3
		 + 3 *	 data[1].point.x * ratio_1_2
		 + 3 *	 data[2].point.x * ratio_2_1
		 +			 data[3].point.x * ratio_3_0;
	*y = current_point.point.y * ratio_0_3
		 + 3 *	 data[1].point.y * ratio_1_2
		 + 3 *	 data[2].point.y * ratio_2_1
		 +			 data[3].point.y * ratio_3_0;

	/* Bezier gradient */
	dx =-3 * current_point.point.x * ratio_0_2
		 + 3 *			 data[1].point.x * _1__4ratio_1_0_3ratio_2_0
		 + 3 *			 data[2].point.x * _2ratio_1_0_3ratio_2_0
		 + 3 *			 data[3].point.x * ratio_2_0;
	dy =-3 * current_point.point.y * ratio_0_2
		 + 3 *			 data[1].point.y * _1__4ratio_1_0_3ratio_2_0
		 + 3 *			 data[2].point.y * _2ratio_1_0_3ratio_2_0
		 + 3 *			 data[3].point.y * ratio_2_0;

	ratio = the_y / sqrt (dx * dx + dy * dy);
	*x += -dy * ratio;
	*y +=	dx * ratio;
			}
			break;
	default:
			g_assert_not_reached ();
	}
}

/* Projects the current path of cr onto the provided path. */
static void
map_path_onto (cairo_t *cr, parametrized_path_t &param)
{
	cairo_path_t *current_path = cairo_copy_path (cr);
	cairo_new_path (cr);

	transform_path (current_path,
			(transform_point_func_t) point_on_path, &param);

	cairo_append_path (cr, current_path);

	cairo_path_destroy (current_path);
	param.parametrization.clear();
}


typedef void (*draw_path_func_t) (cairo_t *cr);

static void
draw_text (cairo_t *cr,
		 double x,
		 double y,
		 const char *text,
		 PangoFontDescription *desc,
		 PangoRectangle *ink_rect_out,
		 PangoRectangle *logical_rect_out)
{
	PangoLayout *layout;
	PangoLayoutLine *line;
	cairo_font_options_t *font_options;

	font_options = cairo_font_options_create ();

	cairo_font_options_set_hint_style (font_options, CAIRO_HINT_STYLE_NONE);
	cairo_font_options_set_hint_metrics (font_options, CAIRO_HINT_METRICS_OFF);

	cairo_set_font_options (cr, font_options);
	cairo_font_options_destroy (font_options);

	layout = pango_cairo_create_layout (cr);

	pango_layout_set_font_description (layout, desc);

	pango_layout_set_text (layout, text, -1);

	/* Use pango_layout_get_line() instead of pango_layout_get_line_readonly()
	 * for older versions of pango
	 */
	line = pango_layout_get_line_readonly (layout, 0);

	cairo_move_to (cr, x, y);
	pango_cairo_layout_line_path (cr, line);

	pango_layout_line_get_pixel_extents (line,
		ink_rect_out,
		logical_rect_out);

	g_object_unref (layout);
}

void calc_twisted_bbox(PangoRectangle &rect,
	parametrized_path_t &param,
	double x,
	double y,
	TwistedTriangles &trianglesOut)
{
	trianglesOut.clear();

	double stepSize = rect.height;
	double cx = rect.x + x;
	double y0 = rect.y + y;
	double y1 = rect.y + y + rect.height;

	double prev1x = cx;
	double prev1y = y0;
	point_on_path(&param, &prev1x, &prev1y);

	double prev2x = cx;
	double prev2y = y1;
	point_on_path(&param, &prev2x, &prev2y);

	double endX = rect.width + rect.x + x;
	cx += stepSize;
	bool looping = true;
	bool finalLoop = false;
	while(looping)
	{
		double p1x = cx;
		double p1y = y0;
		point_on_path(&param, &p1x, &p1y);

		double p2x = cx;
		double p2y = y1;
		point_on_path(&param, &p2x, &p2y);

		std::vector<Point> tri1;
		tri1.push_back(Point(prev1x, prev1y));
		tri1.push_back(Point(prev2x, prev2y));
		tri1.push_back(Point(p1x, p1y));
		trianglesOut.push_back(tri1);

		std::vector<Point> tri2;
		tri2.push_back(Point(prev2x, prev2y));
		tri2.push_back(Point(p1x, p1y));
		tri2.push_back(Point(p2x, p2y));
		trianglesOut.push_back(tri2);

		prev1x = p1x;
		prev1y = p1y;
		prev2x = p2x;
		prev2y = p2y;
		
		if(finalLoop)
			looping = false;
		if(cx < endX && !finalLoop)
			cx += stepSize;
		if(cx >= endX && !finalLoop)
		{
			cx = endX;
			finalLoop = true;
		}
	}

}

static void
draw_twisted (cairo_t *cr,
	double x,
	double y,
	PangoFontDescription *desc,
	const char *text,
	bool doDrawing,
	bool doTriangles,
	const class TextProperties &properties,
	TwistedTriangles &trianglesOut)
{
	cairo_path_t *path;

	cairo_save (cr);

	/* Decrease tolerance a bit, since it's going to be magnified */
	cairo_set_tolerance (cr, 0.01);

	/* Using cairo_copy_path() here shows our deficiency in handling
	 * Bezier curves, specially around sharper curves.
	 *
	 * Using cairo_copy_path_flat() on the other hand, magnifies the
	 * flattening error with large off-path values.	We decreased
	 * tolerance for that reason.	Increase tolerance to see that
	 * artifact.
	 */
	path = cairo_copy_path_flat (cr);
	/*path = cairo_copy_path (cr);*/

	cairo_new_path (cr);

	PangoRectangle ink_rect;
	PangoRectangle logical_rect;
	draw_text (cr, x, y, text, desc, &ink_rect, &logical_rect);
	printf("ink %d %d %d %d\n", ink_rect.x, ink_rect.y, ink_rect.width, ink_rect.height);
	printf("logical %d %d %d %d\n", logical_rect.x, logical_rect.y, logical_rect.width, logical_rect.height);

	parametrized_path_t param;
	param.path = path;
	parametrize_path (path, param.parametrization);

	if(doDrawing)
		map_path_onto (cr, param);

	if(doTriangles)
		calc_twisted_bbox(ink_rect, param, x, y, trianglesOut);
	else
		trianglesOut.clear();

	cairo_path_destroy (path);

	if(doDrawing && !properties.outline)
	{
		cairo_fill (cr);
	}
	cairo_restore (cr);
}

void RunTwistedCurveCmds(cairo_t *cr, const std::vector<TwistedCurveCmd> &cmds)
{
	for(size_t i = 0; i < cmds.size(); i++)
	{
		TwistedCurveCmd cmd = cmds[i];
		const std::vector<double> &vals = cmd.second;
		switch(cmd.first)
		{
		case MoveTo:
			cairo_move_to (cr, vals[0], vals[1]);
			break;
		case LineTo:
			cairo_line_to (cr, vals[0], vals[1]);
			break;
		case RelLineTo:
			cairo_rel_line_to (cr, vals[0], vals[1]);
			break;
		case CurveTo:
			cairo_curve_to (cr, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]);
			break;
		case RelCurveTo:
			cairo_rel_curve_to (cr, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]);
			break;
		}

	}

}

void draw_formatted_twisted_text (cairo_t *cr, const std::string &text, const std::vector<TwistedCurveCmd> &cmds,
	const class TextProperties &properties)
{
	cairo_save (cr);
	RunTwistedCurveCmds(cr, cmds);

	cairo_set_line_width (cr, properties.lineWidth);
	cairo_set_source_rgba (cr, properties.r, properties.g, properties.b, properties.a);

	//Draw Bezier curve used to define shape
	//fancy_cairo_stroke_preserve (cr);

	PangoFontDescription *desc = pango_font_description_from_string (properties.font.c_str());
	pango_font_description_set_size (desc, round(properties.fontSize * PANGO_SCALE));

	TwistedTriangles triangles;
	draw_twisted (cr,
		0, 0,
		desc,
		text.c_str(),
		true,
		false,
		properties,
		triangles);

	pango_font_description_free (desc);

	cairo_set_source_rgba (cr, 0.5, 0.5, 0.5, 0.4);
	fancy_cairo_draw_triangles(cr, triangles);
	cairo_restore (cr);
}

