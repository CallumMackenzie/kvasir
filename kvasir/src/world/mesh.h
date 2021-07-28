#ifndef MESH_H__
#define MESH_H__ 1

#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#ifndef NO_USE_INCLUDES
#include "position.h"
#include "buffer.h"
#include "material.h"
#include "memory-aid.h"
#endif

using namespace galg;
namespace kvasir
{

	struct mesh3d : position3d
	{
		struct triangle
		{
			struct vert
			{
				vec3f p;
				vec2f t;
				vec3f n;
			};

			vert v[3];

			triangle();
			triangle(vec3f &p1, vec3f &p2, vec3f &p3);
		};

		material_base *material = nullptr;
		buffer_base *buffer = nullptr;
		size_t n_tris = 0;
		const size_t tag;

		mesh3d();
		~mesh3d();
		bool load_from_obj(const char *file_name, buffer_base *buf);
		void vertex_pos(const vec3f &pos);
		void vertex_rot(const vec4f &rot);
		void vertex_scale(const vec3f &scale);

		inline static bool use_geo_val_cache = true;
		inline static std::unordered_map<const char *, std::vector<triangle>> geo_val_cache;
		static std::vector<triangle> obj_to_tri_array(const char *file);
		static std::vector<triangle> check_val_cache(const char *file);

	private:
		inline static size_t current_tag = 0;
	};

	struct group_mesh3d : mesh3d
	{
		std::vector<size_t> t_n_tris;
		bool load_from_objs(std::vector<const char *> files, buffer_base *buf);
		void add_mesh_pos(size_t index, const vec3f &pos);
		void add_mesh_scale(size_t index, const vec3f &scale);
		void add_mesh_rot(size_t index, const vec4f &rot);
	};

	struct mesh2d : position2d
	{
	};
};

#endif