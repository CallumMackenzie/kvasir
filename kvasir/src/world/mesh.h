#ifndef MESH_H__
#define MESH_H__ 1

#include <vector>
#include <fstream>
#include <sstream>
#include "position.h"
#include "buffer.h"
#include "material.h"

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

			triangle() {}
			triangle(vec3f p1, vec3f p2, vec3f p3)
			{
				v[0].p = p1;
				v[1].p = p2;
				v[2].p = p3;
			}
		};

		std::vector<triangle> tris;
		material_base *material = nullptr;
		buffer_base *buffer = nullptr;

		void load_to_buffer(buffer_base *buf)
		{
			if (!buf)
				return;
			buffer = buf;
			buf->gen_buffer();
			buf->set_data(&tris[0], tris.size() * sizeof(triangle));
			buf->attrib_ptr(0, 3, sizeof(triangle::component));
			buf->attrib_ptr(1, 2, sizeof(triangle::component), sizeof(vec3f));
			buf->attrib_ptr(2, 3, sizeof(triangle::component), sizeof(vec3f) + sizeof(vec2f));
		}
		bool load_from_obj(const char *file_name, size_t char_min = 33, size_t char_max = 120)
		{
			using namespace std;
			ifstream f(file_name);
			if (!f.is_open())
				return false;
			vector<vec3f> verts;
			vector<vec2f> texs;
			vector<vec3f> norms;
			bool has_tex = false;
			bool has_norm = false;
			while (!f.eof())
			{
				char line[300];
				char junk;
				f.getline(line, 300);
				stringstream s;
				s << line;
				if (line[0] == 'v')
				{
					if (line[1] == 't')
					{
						vec2f v;
						s >> junk >> junk >> v.x() >> v.y();
						texs.push_back(v);
					}
					else if (line[1] == 'n')
					{
						vec3f v;
						s >> junk >> junk >> v.x() >> v.y() >> v.z();
						norms.push_back(v);
						has_norm = true;
					}
					else
					{
						vec3f v;
						s >> junk >> v.x() >> v.y() >> v.z();
						verts.push_back(v);
						has_tex = true;
					}
				}
				if (line[0] == 'f')
				{
					int face[3] = {0};
					int text[3] = {0};
					int norml[3] = {0};
					if (!has_tex && !has_norm)
						s >> junk >> face[0] >> face[1] >> face[2];
					else if (!has_norm)
						s >> junk >>
							face[0] >> junk >> text[0] >>
							face[1] >> junk >> text[1] >>
							face[2] >> junk >> text[2];
					else if (has_norm && has_tex)
						s >> junk >>
							face[0] >> junk >> text[0] >> junk >> norml[0] >>
							face[1] >> junk >> text[1] >> junk >> norml[1] >>
							face[2] >> junk >> text[2] >> junk >> norml[2];
					triangle fTri;
					for (size_s j = 0; j < 3; ++j)
						fTri.v[j].p = verts[face[j] - 1];
					if (has_tex)
						for (size_s j = 0; j < 3; ++j)
							fTri.v[j].t = texs[text[j] - 1];
					if (has_norm)
						for (size_s j = 0; j < 3; ++j)
							fTri.v[j].n = norms[norml[j] - 1];
					tris.push_back(fTri);
				}
			}
			return tris.size() > 0;
		}
	};

	struct mesh2d : position2d
	{
	};
};

#endif