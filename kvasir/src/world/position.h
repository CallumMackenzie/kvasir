#ifndef KVASIR_POSITION_H_
#define KVASIR_POSITION_H_ 1

#ifndef NO_USE_INCLUDES
#include "config.h"
#include "galg.h"
#include "galg-dllexport.h"
#endif

using namespace galg;

namespace kvasir
{
	struct KV_EXPORT position3d
	{

		vec3f &get_pos();
		quaternionf &get_rot();
		vec3f &get_scale();
		vec3f &get_rot_pt();

		vec3f pos;
		quaternionf rot = quaternionf::axis_angle(vec3f(0, 1, 0), 0);
		vec3f scale = vec3f(1, 1, 1);
		vec3f rot_pt;
	};

	struct KV_EXPORT position2d
	{
		vec2f &get_pos();
		float &get_rot();
		vec2f &get_scale();
		vec2f &get_rot_pt();

		float rot = 0.f;
		vec2f pos;
		vec2f scale = galg::vec2f(1, 1);
		vec2f rot_pt;
	};
}

#endif