#include "renderer.h"

using namespace kvasir;

int renderer_base::get_position_x()
{
	int x, y;
	get_position(&x, &y);
	return x;
}
int renderer_base::get_position_y()
{
	int x, y;
	get_position(&x, &y);
	return y;
}
int renderer_base::get_width()
{
	int x, y;
	get_size(&x, &y);
	return x;
};
int renderer_base::get_height()
{
	int x, y;
	get_size(&x, &y);
	return y;
};
void renderer_base::set_width(int w)
{
	set_size(w, get_height());
}
void renderer_base::set_height(int h)
{
	set_size(get_width(), h);
}
double renderer_base::get_aspect()
{
	int w, h;
	get_size(&w, &h);
	return (double)h / (double)w;
}