#ifndef KVASIR_CAMERA_H_
#define KVASIR_CAMERA_H_ 1

#include "position.h"

namespace kvasir
{

	struct camera3d : position3d
	{
		float fov = 75.f;
		float near = 0.1f;
		float far = 20.f;
		float aspect = 1.f;

		inline mat4f view()
		{
			return mat4f::look_at(pos, (mat4f::rotation(rot) * vec4f{0, 0, 1}).xyz() + pos, vec3f{0, 1, 0});
		}
		inline mat4f perspective()
		{
			return mat4fp::perspective(fov, aspect, near, far);
		}
		inline vec4f look_vector()
		{
			return mat4f::rotation(rot) * vec4f{0, 0, 1};
		}
	};

	struct camera2d : position2d
	{
		float aspect = 1.f;
	};

}

#endif