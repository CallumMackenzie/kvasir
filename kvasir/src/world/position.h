#ifndef KVASIR_POSITION_H_
#define KVASIR_POSITION_H_ 1

#include "galg.h"

namespace kvasir
{
	struct position3d
	{
		galg::vec3f pos;
		galg::vec3f rot;
		galg::vec3f scale = galg::vec3f(1, 1, 1);
		galg::vec3f rot_pt;
	};

	struct position2d
	{
		galg::vec2f pos;
		float rot;
		galg::vec2f scale = galg::vec2f(1, 1);
		galg::vec2f rot_pt;
	};
}

#endif