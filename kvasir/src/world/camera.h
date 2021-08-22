#ifndef KVASIR_CAMERA_H_
#define KVASIR_CAMERA_H_ 1

#ifndef NO_USE_INCLUDES
#include "config.h"
#include "position.h"
#endif

namespace kvasir
{

	struct KV_EXPORT camera3d
	{
		float fov = 75.f;
		float near = 0.1f;
		float far = 100.f;
		float aspect = 1.f;
		vec3f pos;
		vec3f rot;

		mat4f view();
		mat4f perspective();
		vec4f look_vector();

		vec3f &get_pos();
		vec3f &get_rot();
	};

	struct KV_EXPORT camera2d : position2d
	{
		float aspect = 1.f;
	};

}

#endif