#ifndef KVASIR_TIME_H_
#define KVASIR_TIME_H_ 1

#ifndef NO_STD_INCLUDES
#include <time.h>
#endif

#ifndef NO_USE_INCLUDES
#include "config.h"
#endif

namespace kvasir
{
	struct KV_EXPORT frame_manager
	{
		enum s_mode
		{
			BUSY,
			SLEEP
		};
		bool next_frame_ready();
		void set_fps(double fps);
		float delta() const;
		double delta_d() const;
		void set_delta_err(double new_err);
		static void sleep(size_t ms);
		static inline double clock_to_sec(const clock_t clock);

		s_mode mode = BUSY;

	private:
		clock_t last_frame = clock();
		double delta_time;				 // Seconds
		double desired_delta = 1.0 / 60; // Seconds
		double delta_err = 0;			 // Seconds
	};
}

#endif