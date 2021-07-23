#include "fps-manager.h"

using namespace kvasir;

bool frame_manager::next_frame_ready()
{
	double frame_delta = clock_to_sec(clock() - last_frame);
	if (frame_delta >= desired_delta)
	{
		delta_time = frame_delta;
		last_frame = clock();
		return true;
	}
	return false;
}
void frame_manager::set_fps(double fps)
{
	desired_delta = 1.0 / fps;
}
double frame_manager::delta() const
{
	return delta_time;
}
double frame_manager::clock_to_sec(const clock_t clock)
{
	return (double)clock / (double)CLOCKS_PER_SEC;
}