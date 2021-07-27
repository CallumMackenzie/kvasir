#ifndef KVASIR_POSITION_H_
#define KVASIR_POSITION_H_ 1

#ifndef NO_USE_INCLUDES
#include "galg.h"
#endif

using namespace galg;

namespace kvasir
{
	struct position3d
	{
		vec3f pos;
		vec4f rot;
		vec3f scale = vec3f(1, 1, 1);
		vec3f rot_pt;
	};

	struct position2d
	{
		vec2f pos;
		float rot;
		vec2f scale = galg::vec2f(1, 1);
		vec2f rot_pt;
	};
}

#endif