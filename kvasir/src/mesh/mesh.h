#ifndef MESH_H__
#define MESH_H__ 1

#include "galg.h"

using namespace galg;
namespace kvasir
{
	template <typename WSP, typename ROT>
	struct mesh
	{
		WSP pos;
		WSP scale;
		ROT rot;
		WSP rot_pt;
		vec3f colour;
	};

	struct mesh3d : mesh<vec3f, vec3f>
	{

	};
	struct mesh2d : mesh<vec2f, float>
	{

	};
};

#endif