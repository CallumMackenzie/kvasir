#ifndef MESH_H__
#define MESH_H__ 1

#ifndef NO_STD_INCLUDES
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#endif

#ifndef NO_USE_INCLUDES
#include "config.h"
#include "galg-dllexport.h"
#include "position.h"
#include "buffer.h"
#include "material.h"
#include "memory-aid.h"
#endif

using namespace galg;

namespace kvasir
{

	struct mesh_base
	{
		material_base *get_material();
		const material_base *get_material() const;
		void set_material(material_base *b);
		buffer_base *get_buffer();
		const buffer_base *get_buffer() const;
		void set_buffer(buffer_base *b);
		size_t n_tris = 0;

	protected:
		material_base *material = nullptr;
		buffer_base *buffer = nullptr;
	};

	struct KV_EXPORT mesh3d : position3d, mesh_base
	{
		struct triangle
		{
			struct vert
			{
				vec3f p; // Vertex pos
				vec2f t; // Vertex UV
				vec3f n; // Vertex normal
			};

			vert v[3]; // Verteces in the triangle

			triangle();
			triangle(vec3f &p1, vec3f &p2, vec3f &p3);
		};

		const size_t tag;

		mesh3d();
		~mesh3d();
		bool load_from_obj(const char *file_name, buffer_base *buf);
		bool load_from_obj_data(const char *data, buffer_base *buf);
		void vertex_pos(const vec3f &pos);
		void vertex_rot(const quaternionf &rot);
		void vertex_scale(const vec3f &scale);

		static std::unordered_map<std::string, std::vector<triangle>> &get_geo_val_cache();
		static std::vector<triangle> obj_to_tri_array(const char *file);
		static std::vector<triangle> check_val_cache(const std::string &file);
		static std::vector<triangle> obj_data_to_tri_arr(const char *data);
		static std::vector<triangle> obj_data_stream_to_tri_arr(std::basic_istream<char, std::char_traits<char>> *stream);

		inline static bool use_geo_val_cache = true;
		inline static std::unordered_map<std::string, std::vector<triangle>> *geo_val_cache = nullptr;

		static void init();
		static void destroy();

	private:
		inline static size_t current_tag = 0;

		void setup_buffer(std::vector<triangle> &tris);
	};

	struct KV_EXPORT group_mesh3d : mesh3d
	{
		~group_mesh3d();
		bool load_from_objs(std::vector<const char *> files, buffer_base *buf);
		void add_mesh_pos(size_t index, const vec3f &pos);
		void add_mesh_scale(size_t index, const vec3f &scale);
		void add_mesh_rot(size_t index, const quaternionf &rot);
		std::vector<size_t> &get_total_tris();
		std::vector<size_t> &get_t_n_tris();

		std::vector<size_t> *t_n_tris = new std::vector<size_t>();
	};

	struct KV_EXPORT mesh2d : position2d, mesh_base
	{
		struct triangle
		{
			struct vert
			{
				vec2f p; // Vertex pos
				vec2f t; // Vertex UV
			};

			vert v[3]; // Vertecies in the triangle
		};

		mesh2d();
		~mesh2d();
		bool make_quad(const vec2f &size, buffer_base *buf);
	};
};

#endif