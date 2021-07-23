#ifndef MESH_H__
#define MESH_H__ 1

#ifndef NO_USE_INCLUDES
#include <vector>
#include <fstream>
#include <sstream>
#include "position.h"
#include "buffer.h"
#include "material.h"
#endif

using namespace galg;
namespace kvasir
{
	struct mesh3d : position3d
	{
		struct triangle
		{
			struct component
			{
				vec3f p;
				vec2f t;
				vec3f n;
			};

			component v[3];

			triangle();
			triangle(vec3f p1, vec3f p2, vec3f p3);
		};

		std::vector<triangle> tris;
		material_base *material = nullptr;
		buffer_base *buffer = nullptr;

		~mesh3d();
		void load_to_buffer(buffer_base *buf);
		bool load_from_obj(const char *file_name);
	};

	struct mesh2d : position2d
	{
	};
};

#endif