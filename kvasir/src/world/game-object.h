#ifndef KVASIR_GAME_OBJECT_H
#define KVASIR_GAME_OBJECT_H 1

#ifndef NO_USE_INCLUDES
#include "config.h"
#include "mesh.h"
#endif

namespace kvasir
{
	struct KV_EXPORT game_object_3d
	{
		game_object_3d();

		vec3f &pos();
		vec4f &rot();
		vec3f &scale();

	private:
		mesh3d mesh;
	};
}

#endif