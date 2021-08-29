#include "fps-manager.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace kvasir;

bool frame_manager::next_frame_ready()
{
	double frame_delta = clock_to_sec(clock() - last_frame);
	if (frame_delta + delta_err >= desired_delta)
	{
		delta_time = frame_delta;
		last_frame = clock();
		if (mode == SLEEP)
			sleep((size_t)(desired_delta * 100.0));
		return true;
	}
	return false;
}
void frame_manager::set_fps(double fps)
{
	desired_delta = 1.0 / fps;
}
float frame_manager::delta() const
{
	return (float)delta_time;
}
double frame_manager::delta_d() const
{
	return delta_time;
}
double frame_manager::clock_to_sec(const clock_t clock)
{
	return (double)clock / (double)CLOCKS_PER_SEC;
}
void frame_manager::sleep(size_t ms)
{
#ifdef _WIN32
	Sleep((DWORD)ms);
#else
	usleep(ms * 1000);
#endif
}
void frame_manager::set_delta_err(double new_err)
{
	delta_err = new_err;
}