#ifndef KVASIR_CAMERA_H_
#define KVASIR_CAMERA_H_ 1

#ifndef NO_USE_INCLUDES
#include "position.h"
#endif

namespace kvasir
{

	struct camera3d : position3d
	{
		float fov = 75.f;
		float near = 0.1f;
		float far = 20.f;
		float aspect = 1.f;

		mat4f view();
		mat4f perspective();
		vec4f look_vector();
	};

	struct camera2d : position2d
	{
		float aspect = 1.f;
	};

}

#endif