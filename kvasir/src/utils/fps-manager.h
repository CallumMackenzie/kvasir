#ifndef KVASIR_TIME_H_
#define KVASIR_TIME_H_ 1

#include <time.h>

namespace kvasir
{
	struct frame_manager
	{
		bool next_frame_ready();
		void set_fps(double fps);
		float delta() const;
		double delta_d() const;
		static inline double clock_to_sec(const clock_t clock);

	private:
		clock_t last_frame = clock();
		double delta_time;
		double desired_delta = 1.0 / 60;
	};
}

#endif