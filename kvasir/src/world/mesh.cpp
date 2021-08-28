#include "mesh.h"

using namespace kvasir;
mesh3d::triangle::triangle() {}
mesh3d::triangle::triangle(vec3f &p1, vec3f &p2, vec3f &p3)
{
	v[0].p = p1;
	v[1].p = p2;
	v[2].p = p3;
}

mesh3d::mesh3d() : tag(++current_tag) {}
mesh3d::~mesh3d()
{
	DEL_PTR(buffer);
	DEL_PTR(material);
}
std::vector<mesh3d::triangle> mesh3d::check_val_cache(const std::string &file)
{
	if (use_geo_val_cache && geo_val_cache_valid())
		if (get_geo_val_cache().find(file) != get_geo_val_cache().end())
			return get_geo_val_cache()[file];
	return std::vector<triangle>{};
}
std::vector<mesh3d::triangle> mesh3d::obj_to_tri_array(const char *file)
{
	using namespace std;
	std::vector<triangle> tris;
	if (use_geo_val_cache)
	{
		tris = check_val_cache(std::string(file));
		if (tris.size() > 0)
			return tris;
	}
	ifstream ifs(file);
	tris = obj_data_stream_to_tri_arr(&ifs);
	if (tris.size() > 0 && use_geo_val_cache && geo_val_cache_valid())
		get_geo_val_cache()[std::string(file)] = tris;
	return tris;
}
std::vector<mesh3d::triangle> mesh3d::obj_data_to_tri_arr(const char *data)
{
	std::istringstream iss(data);
	return obj_data_stream_to_tri_arr(&iss);
}
std::vector<mesh3d::triangle> mesh3d::obj_data_stream_to_tri_arr(std::basic_istream<char, std::char_traits<char>> *f)
{
	using namespace std;
	std::vector<triangle> tris;
	if (!f)
		return tris;
	vector<vec3f> verts;
	vector<vec2f> texs;
	vector<vec3f> norms;
	bool has_tex = false;
	bool has_norm = false;
	while (!f->eof())
	{
		char line[300];
		char junk;
		f->getline(line, 300);
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
	return tris;
}
void mesh3d::setup_buffer(std::vector<triangle> &tris)
{
	if (!buffer)
		return;
	buffer->gen_buffer();
	buffer->set_data(&tris[0], tris.size() * sizeof(triangle));
	buffer->attrib_ptr(0, 3, sizeof(triangle::vert));
	buffer->attrib_ptr(1, 2, sizeof(triangle::vert), sizeof(vec3f));
	buffer->attrib_ptr(2, 3, sizeof(triangle::vert), sizeof(vec3f) + sizeof(vec2f));
}
bool mesh3d::geo_val_cache_valid()
{
	return (bool)geo_val_cache;
}
bool mesh3d::load_from_obj(const char *file_name, buffer_base *buf)
{
	DEL_PTR(buffer);
	std::vector<triangle> tris = obj_to_tri_array(file_name);
	n_tris = tris.size();
	if (n_tris <= 0 || !buf)
		return false;
	buffer = buf;
	setup_buffer(tris);
	return true;
}
bool mesh3d::load_from_obj_data(const char *data, buffer_base *buf)
{
	DEL_PTR(buffer);
	std::vector<triangle> tris = obj_data_to_tri_arr(data);
	n_tris = tris.size();
	if (n_tris <= 0 || !buf)
		return false;
	buffer = buf;
	setup_buffer(tris);
	return true;
}
bool mesh3d::load_from_tri_data(std::vector<triangle> data, buffer_base *buf)
{
	DEL_PTR(buffer);
	n_tris = data.size();
	if (n_tris <= 0 || !buf)
		return false;
	buffer = buf;
	setup_buffer(data);
	return true;
}
bool group_mesh3d::load_from_objs(std::vector<const char *> files, buffer_base *buf)
{
	DEL_PTR(buffer);
	n_tris = 0;
	get_t_n_tris().clear();
	std::vector<triangle> all_tris;
	for (size_t i = 0; i < files.size(); ++i)
	{
		std::vector<mesh3d::triangle> tris = mesh3d::obj_to_tri_array(files[i]);
		if (tris.size() <= 0)
			return false;
		get_t_n_tris().push_back(tris.size());
		for (size_t j = 0; j < tris.size(); ++j)
			all_tris.push_back(tris[j]);
		tris.clear();
	}
	n_tris = all_tris.size();
	size_t prev_offset = 0;
	set_buffer(buf);
	get_buffer()->gen_buffer();
	get_buffer()->set_data(&all_tris[0], all_tris.size() * sizeof(triangle));
	get_buffer()->attrib_ptr(0, 3, sizeof(triangle::vert));
	get_buffer()->attrib_ptr(1, 2, sizeof(triangle::vert), prev_offset += sizeof(vec3f));
	get_buffer()->attrib_ptr(2, 3, sizeof(triangle::vert), prev_offset += sizeof(vec2f));
	return true;
}
void mesh3d::vertex_pos(const vec3f &pos)
{

	triangle *buf_data = new triangle[n_tris];
	get_buffer()->get_data(buf_data, 0, n_tris * sizeof(triangle));
	for (size_t i = 0; i < n_tris; ++i)
		for (size_t j = 0; j < 3; ++j)
			buf_data[i].v[j].p += pos;
	get_buffer()->sub_data(buf_data, 0, n_tris * sizeof(triangle));
	DEL_ARR_PTR(buf_data);
}
void mesh3d::vertex_rot(const quaternionf &rot)
{
	mat4f m_rot = mat4f::rotation(rot);
	triangle *buf_data = new triangle[n_tris];
	get_buffer()->get_data(buf_data, 0, n_tris * sizeof(triangle));
	for (size_t i = 0; i < n_tris; ++i)
		for (size_t j = 0; j < 3; ++j)
			buf_data[i].v[j].p = (m_rot * buf_data[i].v[j].p.xyz1()).xyz();
	get_buffer()->sub_data(buf_data, 0, n_tris * sizeof(triangle));
	DEL_ARR_PTR(buf_data);
}
void mesh3d::vertex_scale(const vec3f &scale)
{
	triangle *buf_data = new triangle[n_tris];
	get_buffer()->get_data(buf_data, 0, n_tris * sizeof(triangle));
	for (size_t i = 0; i < n_tris; ++i)
		for (size_t j = 0; j < 3; ++j)
			buf_data[i].v[j].p *= scale;
	get_buffer()->sub_data(buf_data, 0, n_tris * sizeof(triangle));
	DEL_ARR_PTR(buf_data);
}
void group_mesh3d::add_mesh_pos(size_t index, const vec3f &pos)
{
	triangle *buf_data = new triangle[get_t_n_tris()[index]];
	size_t n_tris_before = 0;
	for (size_t i = 0; i < index; ++i)
		n_tris_before += get_t_n_tris()[i];
	get_buffer()->get_data(buf_data, n_tris_before * sizeof(triangle), get_t_n_tris()[index] * sizeof(triangle));
	for (size_t i = 0; i < get_t_n_tris()[index]; ++i)
		for (size_t j = 0; j < 3; ++j)
			buf_data[i].v[j].p += pos;
	get_buffer()->sub_data(buf_data, n_tris_before * sizeof(triangle), get_t_n_tris()[index] * sizeof(triangle));
	DEL_ARR_PTR(buf_data);
}
void group_mesh3d::add_mesh_scale(size_t index, const vec3f &scale)
{
	triangle *buf_data = new triangle[get_t_n_tris()[index]];
	size_t n_tris_before = 0;
	for (size_t i = 0; i < index; ++i)
		n_tris_before += get_t_n_tris()[i];
	get_buffer()->get_data(buf_data, n_tris_before * sizeof(triangle), get_t_n_tris()[index] * sizeof(triangle));
	for (size_t i = 0; i < get_t_n_tris()[index]; ++i)
		for (size_t j = 0; j < 3; ++j)
			buf_data[i].v[j].p *= scale;
	get_buffer()->sub_data(buf_data, n_tris_before * sizeof(triangle), get_t_n_tris()[index] * sizeof(triangle));
	DEL_ARR_PTR(buf_data);
}
void group_mesh3d::add_mesh_rot(size_t index, const quaternionf &rot)
{
	mat4f m_rot = mat4f::rotation(rot);
	triangle *buf_data = new triangle[get_t_n_tris()[index]];
	size_t n_tris_before = 0;
	for (size_t i = 0; i < index; ++i)
		n_tris_before += get_t_n_tris()[i];
	buffer->get_data(buf_data, n_tris_before * sizeof(triangle), get_t_n_tris()[index] * sizeof(triangle));
	for (size_t i = 0; i < get_t_n_tris()[index]; ++i)
		for (size_t j = 0; j < 3; ++j)
			buf_data[i].v[j].p = (m_rot * buf_data[i].v[j].p.xyz1()).xyz();
	buffer->sub_data(buf_data, n_tris_before * sizeof(triangle), get_t_n_tris()[index] * sizeof(triangle));
	DEL_ARR_PTR(buf_data);
}

mesh2d::mesh2d() {}
mesh2d::~mesh2d()
{
	DEL_PTR(buffer)
	DEL_PTR(material)
}

bool mesh2d::make_quad(const vec2f &size, buffer_base *buf)
{
	DEL_PTR(buffer)
	triangle tris[2];
	float x = size.x() * 0.5f;
	float y = size.y() * 0.5f;
	tris[0].v[0].p = vec2f(-x, -y);
	tris[0].v[1].p = vec2f(-x, y);
	tris[0].v[2].p = vec2f(x, y);
	tris[0].v[0].t = vec2f(0, 1);
	tris[0].v[1].t = vec2f(0, 0);
	tris[0].v[2].t = vec2f(1, 0);

	tris[1].v[0].p = vec2f(-x, -y);
	tris[1].v[1].p = vec2f(x, y);
	tris[1].v[2].p = vec2f(x, -y);
	tris[1].v[0].t = vec2f(0, 1);
	tris[1].v[1].t = vec2f(1, 0);
	tris[1].v[2].t = vec2f(1, 1);

	n_tris = 2;
	buffer = buf;
	buffer->gen_buffer();
	buffer->set_data(&tris[0], 2 * sizeof(triangle));
	buffer->attrib_ptr(0, 2, sizeof(triangle::vert));
	buffer->attrib_ptr(1, 2, sizeof(triangle::vert), sizeof(vec2f));
	return true;
}
std::unordered_map<std::string, std::vector<mesh3d::triangle>> &mesh3d::get_geo_val_cache() { return *geo_val_cache; }
std::vector<size_t> &group_mesh3d::get_total_tris() { return get_t_n_tris(); }
std::vector<size_t> &group_mesh3d::get_t_n_tris() { return *t_n_tris; }
void mesh3d::init()
{
	DEL_PTR(geo_val_cache);
	geo_val_cache = new std::unordered_map<std::string, std::vector<triangle>>();
}
void mesh3d::destroy()
{
	DEL_PTR(geo_val_cache);
}
group_mesh3d::~group_mesh3d()
{
	DEL_PTR(t_n_tris);
}
material_base *mesh_base::get_material() { return material; }
const material_base *mesh_base::get_material() const { return material; }
void mesh_base::set_material(material_base *b)
{
	if (b == material)
		return;
	DEL_PTR(material);
	material = b;
}
const buffer_base *mesh_base::get_buffer() const
{
	return buffer;
}
buffer_base *mesh_base::get_buffer()
{
	return buffer;
}
void mesh_base::set_buffer(buffer_base *b)
{
	if (b == buffer)
		return;
	DEL_PTR(buffer);
	buffer = b;
}