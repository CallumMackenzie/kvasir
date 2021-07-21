#ifndef KVASIR_TIME_H_
#define KVASIR_TIME_H_ 1

#include <time.h>

struct frame_manager
{
	bool next_frame_ready()
	{
		if (clock_to_sec(clock() - last_frame) >= desired_delta)
		{
			last_frame = clock();
			return true;
		}
		return false;
	}

	void set_fps(double fps)
	{
		desired_delta = 1.0 / fps;
	}

	static inline double clock_to_sec(clock_t clock)
	{
		return (double)clock / (double)CLOCKS_PER_SEC;
	}

private:
	clock_t last_frame;
	double delta_time;
	double desired_delta = 1.0 / 60;
};

#endif