#include "krc-packer.h"

#define CONTAINS_TEX(T) (T == krc_mesh3d_fmt::VT || T == krc_mesh3d_fmt::VTN)
#define CONTAINS_NORM(N) (N == krc_mesh3d_fmt::VN || N == krc_mesh3d_fmt::VTN)

size_t get_mesh_size(kvasir::packer::krc_mesh3d_fmt f)
{
	{
		using namespace kvasir::packer;
		switch (f)
		{
		case krc_mesh3d_fmt::VTN:
			return (sizeof(vec3f) + sizeof(vec2f) + sizeof(vec3f)) * 3;
		case krc_mesh3d_fmt::VT:
			return (sizeof(vec3f) + sizeof(vec2f)) * 3;
		case krc_mesh3d_fmt::VN:
			return (sizeof(vec3f) + sizeof(vec3f)) * 3;
		case krc_mesh3d_fmt::V:
			return (sizeof(vec3f)) * 3;
		default:
			break;
		}
	}
	return sizeof(kvasir::mesh3d::triangle);
}

bool byte_ops::is_big_endian()
{
	union
	{
		uint32_t i;
		char c[4];
	} sdjksdj = {0x01020304};
	return sdjksdj.c[0] == 1;
}
bool byte_ops::is_little_endian()
{
	return !is_big_endian();
}
void byte_ops::swap_endianness(galg::vec3f &v, bool endian)
{
	if (endian)
		return;
	reverse_bytes(v.x());
	reverse_bytes(v.y());
	reverse_bytes(v.z());
}
void byte_ops::swap_endianness(galg::vec2f &v, bool endian)
{
	if (endian)
		return;
	reverse_bytes(v.x());
	reverse_bytes(v.y());
}
void byte_ops::swap_endianness(kvasir::mesh3d::triangle &tri, bool endian)
{
	if (endian)
		return;
	for (size_t j = 0; j < 3; ++j)
	{
		kvasir::mesh3d::triangle::vert &vt = tri.v[j];
		swap_endianness(vt.p, endian);
		swap_endianness(vt.t, endian);
		swap_endianness(vt.n, endian);
	}
}
void byte_ops::swap_endianness(std::vector<kvasir::mesh3d::triangle> &vec, bool endian)
{
	if (endian)
		return;
	for (size_t i = 0; i < vec.size(); ++i)
		swap_endianness(vec[i], endian);
}
void byte_ops::swap_endianness(uint64_t &v, bool endianness)
{
	if (endianness)
		return;
	reverse_bytes(v);
}
void byte_ops::get_bytes(uint64_t n, unsigned char recv[sizeof(uint64_t)])
{
	for (size_t i = 0; i < sizeof(uint64_t); ++i)
		recv[(sizeof(uint64_t) - 1) - i] = (unsigned char)(n >> (int)(i * 8));
}
void byte_ops::set_bytes(unsigned char bytes[sizeof(uint64_t)], uint64_t &recv)
{
	for (size_t i = 0; i < sizeof(uint64_t); ++i)
		recv |= bytes[(sizeof(uint64_t) - 1) - i] << (int)(i * 8);
}

void byte_ops::get_bytes(float na, unsigned char recv[sizeof(float)])
{
	unsigned char *bytes = (unsigned char *)&na;
	for (size_t i = 0; i < sizeof(float); ++i)
		recv[i] = bytes[i];
}
void byte_ops::set_bytes(unsigned char bytes[sizeof(float)], float &rec)
{
	rec = *((float *)bytes);
}
void byte_ops::get_bytes(unsigned char n, unsigned char recv[sizeof(unsigned char)])
{
	recv[0] = n;
}
void byte_ops::set_bytes(unsigned char bytes[sizeof(unsigned char)], unsigned char &recv)
{
	recv = bytes[0];
}
void byte_ops::get_bytes(unsigned n, unsigned char recv[sizeof(unsigned)])
{
	for (unsigned char i = 0; i < sizeof(unsigned); ++i)
		recv[(sizeof(unsigned) - 1) - i] = (unsigned char)(n >> (i * 8));
}
void byte_ops::set_bytes(unsigned char bytes[sizeof(unsigned)], unsigned &recv)
{
	for (unsigned char i = 0; i < sizeof(unsigned); ++i)
		recv |= bytes[(sizeof(unsigned) - 1) - i] << (i * 8);
}

using namespace kvasir;
using namespace byte_ops;

packer::krc_file packer::obj_data_to_krc(const char *name, const char *obj_data)
{
	std::istringstream iss(obj_data);
	return obj_stream_to_krc(name, &iss);
}
packer::krc_file packer::obj_stream_to_krc(const char *name, std::basic_istream<char, std::char_traits<char>> *stream)
{
	krc_file ret;
	if (!stream)
		return ret;
	std::vector<mesh3d::triangle> tri_arr = mesh3d::obj_data_stream_to_tri_arr(stream);
	swap_endianness(tri_arr, is_little_endian());
	unsigned char *ch_data_ptr = (unsigned char *)&tri_arr[0];
	size_t ch_len = (sizeof(mesh3d::triangle) * tri_arr.size());
	for (size_t i = 0; i < ch_len; ++i)
		ret.data.push_back(ch_data_ptr[i]);
	ret.header.clear();
	unsigned char header_vdata[BLOB_VDATA_LEN]{0};
	krc_file::add_header_section(ret.header, krc::MESH3D, name, ret.data_stack_ptr, ret.data.size(), header_vdata);
	ret.data_stack_ptr += ch_len;
	return ret;
}
std::vector<unsigned char> packer::krc_file::serialize()
{
	std::vector<unsigned char> ser;
	add_cstr(KRC_TAG, ser);
	add_uint64_t(header.size(), ser);
	add_uint64_t(header.size() + data.size(), ser);

	for (size_t i = 0; i < header.size(); ++i)
		ser.push_back(header[i]);
	for (size_t i = 0; i < data.size(); ++i)
		ser.push_back(data[i]);
	return ser;
}
packer::krc_file packer::krc_file::deserialize(std::vector<unsigned char> header, std::vector<unsigned char> dat)
{
	krc_file ret;
	ret.header = header;
	ret.data = dat;
	ret.data_stack_ptr = dat.size();
	return ret;
}
packer::krc packer::krc_file::get_first_type()
{
	if (header.size() == 0)
		return krc::UNKNOWN;
	return (krc)header[BLOB_TYPE_LOC];
}
std::string packer::krc_file::get_first_name()
{
	if (header.size() == 0)
		return std::string("");
	char recv[BLOB_NAME_LEN + 1]{0};
	for (size_t i = 0; i < BLOB_NAME_LEN; ++i)
		recv[i] = (char)header[BLOB_NAME_LOC + i];
	return std::string(recv);
}
bool packer::krc_file::save(const char *path)
{
	std::vector<unsigned char> dat = serialize();
	std::ofstream fs(path, std::ios::binary);
	if (!fs.is_open())
		return false;
	fs.write((char *)&dat[0], dat.size());
	fs.close();
	return true;
}
packer::krc_file packer::krc_file::deserialize(const char *file_path)
{
	std::ifstream fs(file_path, std::ios::binary);
	if (!fs.is_open())
		return krc_file();

	std::vector<unsigned char> alldata;

	unsigned char buff[1024]{0};
	while (!fs.eof())
	{
		fs.read(reinterpret_cast<char *>(buff), 1024);
		for (size_t i = 0; i < 1024; ++i)
			alldata.push_back(buff[i]);
	}

	return deserialize(alldata);
}
void packer::krc_file::add_uint64_t(uint64_t val, std::vector<unsigned char> &f)
{
	unsigned char buffer[sizeof(uint64_t)]{0};
	if (is_big_endian())
		reverse_bytes(val);
	get_bytes(val, buffer);
	for (unsigned short i = 0; i < sizeof(uint64_t); ++i)
		f.push_back(buffer[i]);
}
void packer::krc_file::add_float(float val, std::vector<unsigned char> &f)
{
	unsigned char buffer[sizeof(float)]{0};
	if (is_big_endian())
		reverse_bytes(val);
	get_bytes(val, buffer);
	for (unsigned short i = 0; i < sizeof(float); ++i)
		f.push_back(buffer[i]);
}
void packer::krc_file::add_char(char val, std::vector<unsigned char> &f)
{
	unsigned char *cptr = (unsigned char *)&val;
	f.push_back(*cptr);
}
void packer::krc_file::add_cstr(const char *str, std::vector<unsigned char> &f)
{
	char val;
	int str_ctr = -1;
	while ((val = str[++str_ctr]) != '\0')
		add_char(val, f);
}
void packer::krc_file::get_cstr(char *recv, size_t len, std::ifstream &fs)
{
	for (size_t i = 0; i < len; ++i)
		recv[i] = get<unsigned char>(fs);
}
void packer::krc_file::add_header_section(std::vector<unsigned char> &header, krc type, const char *data_name_c, uint64_t data_pos, uint64_t data_len, unsigned char vdata[BLOB_VDATA_LEN])
{
	std::string data_name(data_name_c);
	if (data_name.size() > BLOB_NAME_LEN)
		throw std::runtime_error("Data name was too many characters.");
	while (data_name.size() < BLOB_NAME_LEN)
		data_name.append(BLOB_NAME_FILLER);
	header.push_back((unsigned char)type); // type
	add_cstr(data_name.c_str(), header);   // name
	add_uint64_t(data_len, header);		   // data len
	add_uint64_t(data_pos, header);		   // start pos
	if (vdata)
		for (size_t i = 0; i < BLOB_VDATA_LEN; ++i) // buffer vdata
			header.push_back(vdata[i]);
	else
		for (size_t i = 0; i < BLOB_VDATA_LEN; ++i)
			header.push_back((unsigned char)0);
}
packer::krc_file packer::krc_file::deserialize(std::vector<unsigned char> all_data)
{
	if (all_data.size() < HEADER_STATIC_SIZE)
		throw std::runtime_error("Not a correct KRC file : too small.");
	std::vector<unsigned char> header;
	std::vector<unsigned char> data;

	uint64_t header_len = 0, file_len = 0, data_len = 0;
	set_bytes(&all_data[SH_BL_LOC], header_len);
	set_bytes(&all_data[SH_FL_LOC], file_len);
	if (is_big_endian())
	{
		reverse_bytes(header_len);
		reverse_bytes(file_len);
	}
	data_len = file_len - header_len;

	for (size_t i = 0; i < header_len; ++i)
		header.push_back(all_data[i + HEADER_STATIC_SIZE]);

	for (size_t i = 0; i < data_len; ++i)
		data.push_back(all_data[i + HEADER_STATIC_SIZE + header_len]);

	return deserialize(header, data);
}
std::vector<unsigned char> packer::krc_file::get_resource_data(const char *cname)
{
	header_blob blob = find_blob(cname);
	if (!blob.is_valid())
		throw std::runtime_error("Blob is not valid.");
	return get_resource_data_from_blob(blob);
}
packer::krc_file::header_blob packer::krc_file::get_blob(unsigned char *blob_start)
{
	header_blob ret;
	ret.type = (krc)blob_start[BLOB_TYPE_LOC];
	for (size_t i = 0; i < BLOB_NAME_LEN; ++i)
		ret.name[i] = blob_start[BLOB_NAME_LOC + i];
	set_bytes(&blob_start[BLOB_RES_LEN_LOC], ret.len);
	if (is_big_endian())
		reverse_bytes(ret.len);
	set_bytes(&blob_start[BLOB_DAT_START_LOC], ret.ptr);
	if (is_big_endian())
		reverse_bytes(ret.ptr);
	for (size_t i = 0; i < BLOB_VDATA_LEN; ++i)
		ret.vdata[i] = blob_start[BLOB_VDATA_LOC + i];
	return ret;
}
std::vector<unsigned char> packer::krc_file::get_resource_data_from_blob(header_blob &blob)
{
	unsigned char *data_ptr = &data[blob.ptr];
	std::vector<unsigned char> krc_data;
	for (size_t i = 0; i < blob.len; ++i)
		krc_data.push_back(data_ptr[i]);
	return krc_data;
}
packer::krc_file::header_blob packer::krc_file::find_blob(const char *cname)
{
	std::string name(cname);
	if (name.size() > BLOB_NAME_LEN)
		throw std::runtime_error("Data name was too many characters.");
	while (name.size() < BLOB_NAME_LEN)
		name.append(BLOB_NAME_FILLER);
	for (size_t i = 0; i < header.size(); i += BLOB_LEN)
	{
		header_blob blob = get_blob(&header[i]);
		bool name_eq = true;
		for (size_t i = 0; i < BLOB_NAME_LEN; ++i)
			name_eq &= (blob.name[i] == name.c_str()[i]);
		if (name_eq)
			return blob;
	}
	return header_blob();
}
bool packer::krc_file::header_blob::is_valid()
{
	return (len != 0 && name[0] != '\0');
}
void packer::krc_file::add_data_section(std::vector<unsigned char> &data_dest, const std::vector<unsigned char> &data_src)
{
	for (size_t i = 0; i < data_src.size(); ++i)
		data_dest.push_back(data_src[i]);
}
void packer::krc_file::add_resource(const char *name, krc type, const std::vector<unsigned char> &newdata, unsigned char vdata[BLOB_VDATA_LEN])
{
	add_data_section(data, newdata);
	add_header_section(header, type, name, data_stack_ptr, newdata.size(), vdata);
	data_stack_ptr += newdata.size();
}
void packer::krc_file::add_mesh3d(const char *name, mesh3d::triangle *tris, size_t ntris, krc_mesh3d_fmt mfmt)
{
	std::vector<mesh3d::triangle> vtris;
	for (size_t i = 0; i < ntris; ++i)
		vtris.push_back(tris[i]);
	add_mesh3d(name, vtris, mfmt);
}
void packer::krc_file::add_mesh3d_from_obj_data(const char *name, const char *obj_data, krc_mesh3d_fmt mfmt)
{
	std::vector<mesh3d::triangle> tris = mesh3d::obj_data_to_tri_arr(obj_data);
	add_mesh3d(name, tris, mfmt);
}
std::vector<mesh3d::triangle> packer::krc_file::get_mesh3d_data(const char *name)
{
	header_blob blob = find_blob(name);
	if (!blob.is_valid())
		throw std::runtime_error("Header blob is not valid.");
	if (blob.type != krc::MESH3D)
		throw std::runtime_error("Blob is not of type MESH3D.");
	std::vector<unsigned char> chdata = get_resource_data_from_blob(blob);
	return get_mesh3d_data_from_bytes(chdata, blob.vdata);
}
void packer::krc_file::add_texture(const char *name, const texture_image &img, bool image_decoded)
{
	unsigned char vdata[BLOB_VDATA_LEN]{0};
	set_vdata_bytes<unsigned>(vdata, 0, img.w);
	set_vdata_bytes<unsigned>(vdata, sizeof(unsigned), img.h);
	set_vdata_bytes<unsigned char>(vdata, sizeof(unsigned) * 2, (unsigned char)image_decoded);
	add_resource(name, krc::TEXTURE, img.get_pixels(), vdata);
}
void packer::krc_file::add_texture(const char *name, const char *png_path, bool decode_image)
{
	if (decode_image)
		add_texture(name, texture_base::load_image(png_path), true);
	else
	{
		std::vector<unsigned char> png_data = texture_image::get_png_data(png_path);
		texture_image tex;
		tex.set_pixels(png_data);
		tex.w = 0;
		tex.h = 0;
		add_texture(name, tex, false);
	}
}
texture_image packer::krc_file::get_texture(const char *name)
{
	header_blob blob = find_blob(name);
	if (!blob.is_valid())
		throw std::runtime_error("Header blob is not valid.");
	if (blob.type != krc::TEXTURE)
		throw std::runtime_error("Blob is not of type TEXTURE.");
	std::vector<unsigned char> pixels = get_resource_data_from_blob(blob);
	return get_texture_from_bytes(pixels, blob);
}
void packer::krc_file::add_mesh3d_from_obj_stream(const char *name, std::basic_istream<char, std::char_traits<char>> *stream, krc_mesh3d_fmt mfmt)
{
	if (!stream)
		return;
	add_mesh3d(name, mesh3d::obj_data_stream_to_tri_arr(stream), mfmt);
}
void packer::krc_file::add_mesh3d_from_obj_file(const char *name, const char *path, krc_mesh3d_fmt mfmt)
{
	std::fstream f(path);
	add_mesh3d_from_obj_stream(name, &f, mfmt);
	f.close();
}
packer::krc_file::header_blob packer::krc_file::get_blob_from_file(const char *file, const char *cname)
{
	std::vector<header_blob> blobs = get_blobs_in_file(file);
	std::string name(cname);
	if (name.size() > BLOB_NAME_LEN)
		throw std::runtime_error("Blob name was too many characters.");
	while (name.size() < BLOB_NAME_LEN)
		name.append(BLOB_NAME_FILLER);
	header_blob *target_blob = nullptr;
	for (size_t i = 0; i < blobs.size(); ++i)
	{
		bool nameeq = true;
		for (size_t j = 0; j < BLOB_NAME_LEN; ++j)
			nameeq &= blobs[i].name[j] == name[j];
		if (nameeq)
		{
			target_blob = &blobs[i];
			break;
		}
	}
	if (!target_blob)
		return header_blob{};
	return *target_blob;
}
std::string packer::krc_file::header_blob::to_string()
{
	char nm[BLOB_NAME_LEN + 1]{0};
	for (size_t i = 0; i < BLOB_NAME_LEN; ++i)
		nm[i] = name[i];
	char tp[BLOB_TYPE_LEN + 1]{(char)type, 0};
	return std::string("hb(type=").append(std::string(tp)).append(", name=").append(std::string(nm)).append(", len=").append(std::to_string(len)).append(", ptr=").append(std::to_string(ptr)).append(")");
}
std::vector<packer::krc_file::header_blob> packer::krc_file::get_blobs_in_file(const char *file)
{
	std::ifstream f(file, std::ios::binary);
	if (!f.is_open())
		throw std::runtime_error(std::string("Could not open file (").append(std::string(file)).append(").").c_str());
	unsigned char static_header_buff[HEADER_STATIC_SIZE]{0};
	f.read((char *)static_header_buff, HEADER_STATIC_SIZE);
	uint64_t header_len = get_num<uint64_t>(&static_header_buff[SH_BL_LOC]);
	uint64_t file_len = get_num<uint64_t>(&static_header_buff[SH_FL_LOC]);
	unsigned char *dynamic_header_buff = new unsigned char[header_len]{0};
	f.read((char *)dynamic_header_buff, header_len);
	uint64_t n_blobs = header_len / BLOB_LEN;
	std::vector<header_blob> blobs;
	for (size_t i = 0; i < n_blobs; ++i)
		blobs.push_back(get_blob(&dynamic_header_buff[i * BLOB_LEN]));
	DEL_ARR_PTR(dynamic_header_buff);
	return blobs;
}
std::vector<unsigned char> packer::krc_file::get_resource_data_from_blob_file(const header_blob &blob, const char *file)
{
	std::ifstream f(file, std::ios::binary);
	if (!f.is_open())
		throw std::runtime_error(std::string("Could not open file (").append(std::string(file)).append(").").c_str());
	unsigned char static_header_buff[HEADER_STATIC_SIZE]{0};
	f.read((char *)static_header_buff, HEADER_STATIC_SIZE);
	uint64_t header_len = get_num<uint64_t>(&static_header_buff[SH_BL_LOC]);
	uint64_t file_len = get_num<uint64_t>(&static_header_buff[SH_FL_LOC]);
	uint64_t data_len = file_len - header_len;
	unsigned char *dynamic_header_buff = new unsigned char[header_len]{0};
	f.read((char *)dynamic_header_buff, header_len);
	DEL_ARR_PTR(dynamic_header_buff);
	f.seekg(blob.ptr + header_len + HEADER_STATIC_SIZE);
#define READ_BUFF_SZ 1024
	std::vector<unsigned char> data;
	unsigned char buff[READ_BUFF_SZ]{0};
	for (uint64_t i = 0; i < blob.len; i += READ_BUFF_SZ)
	{
		f.read((char *)buff, READ_BUFF_SZ);
		for (size_t j = 0; j < READ_BUFF_SZ; ++j)
			data.push_back(buff[j]);
	}
	while (data.size() > blob.len)
		data.pop_back();
	return data;
}
std::vector<mesh3d::triangle> packer::krc_file::get_mesh3d_data_from_file(const char *name, const char *file)
{
	header_blob blob = get_blob_from_file(file, name);
	if (!blob.is_valid())
		throw std::runtime_error("No valid blob found.");
	if (blob.type != krc::MESH3D)
		throw std::runtime_error("Blob is not of type MESH3D.");
	std::vector<unsigned char> chdata = get_resource_data_from_blob_file(blob, file);
	return get_mesh3d_data_from_bytes(chdata, blob.vdata);
}
texture_image packer::krc_file::get_texture_from_file(const char *name, const char *file)
{
	header_blob blob = get_blob_from_file(file, name);
	if (!blob.is_valid())
		throw std::runtime_error("No valid blob found.");
	if (blob.type != krc::TEXTURE)
		throw std::runtime_error("Blob is not of type TEXTURE.");
	std::vector<unsigned char> chdata = get_resource_data_from_blob_file(blob, file);
	return get_texture_from_bytes(chdata, blob);
}
void packer::krc_file::add_mesh3d(const char *name, std::vector<mesh3d::triangle> tris, krc_mesh3d_fmt mfmt)
{
	unsigned char mesh3d_vdata[BLOB_VDATA_LEN]{0};
	mesh3d_vdata[0] = (unsigned char)mfmt;
	std::vector<unsigned char> tris_chdata;
	for (size_t i = 0; i < tris.size(); ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			mesh3d::triangle::vert &vt = tris[i].v[j];
			add_bytes<float>(tris_chdata, vt.p.x());
			add_bytes<float>(tris_chdata, vt.p.y());
			add_bytes<float>(tris_chdata, vt.p.z());

			if (CONTAINS_TEX(mfmt))
			{
				add_bytes<float>(tris_chdata, vt.t.x());
				add_bytes<float>(tris_chdata, vt.t.y());
			}
			if (CONTAINS_NORM(mfmt))
			{
				add_bytes<float>(tris_chdata, vt.n.x());
				add_bytes<float>(tris_chdata, vt.n.y());
				add_bytes<float>(tris_chdata, vt.n.z());
			}
		}
	add_resource(name, krc::MESH3D, tris_chdata, mesh3d_vdata);
}
std::vector<mesh3d::triangle> packer::krc_file::get_mesh3d_data_from_bytes(std::vector<unsigned char> &chdata, unsigned char vdata[BLOB_VDATA_LEN])
{
	krc_mesh3d_fmt mfmt = (krc_mesh3d_fmt)vdata[0];
	std::vector<mesh3d::triangle> ret;
	int64_t float_ctr = -1;
	for (size_t i = 0; i < chdata.size() / get_mesh_size(mfmt); ++i)
	{
		ret.push_back(mesh3d::triangle{});
		for (size_t j = 0; j < 3; ++j)
		{
			mesh3d::triangle::vert &vt = ret[i].v[j];
			vt.p.x() = get_num<float>(&chdata[(++float_ctr) * sizeof(float)]);
			vt.p.y() = get_num<float>(&chdata[(++float_ctr) * sizeof(float)]);
			vt.p.z() = get_num<float>(&chdata[(++float_ctr) * sizeof(float)]);

			if (CONTAINS_TEX(mfmt))
			{
				vt.t.x() = get_num<float>(&chdata[(++float_ctr) * sizeof(float)]);
				vt.t.y() = get_num<float>(&chdata[(++float_ctr) * sizeof(float)]);
			}
			if (CONTAINS_NORM(mfmt))
			{
				vt.n.x() = get_num<float>(&chdata[(++float_ctr) * sizeof(float)]);
				vt.n.y() = get_num<float>(&chdata[(++float_ctr) * sizeof(float)]);
				vt.n.z() = get_num<float>(&chdata[(++float_ctr) * sizeof(float)]);
			}
		}
		if (mfmt != krc_mesh3d_fmt::VN && mfmt != krc_mesh3d_fmt::VTN)
		{
			vec3f norm = (ret[i].v[1].p - ret[i].v[0].p).cross(ret[i].v[2].p - ret[i].v[0].p);
			for (size_t k = 0; k < 3; ++k)
				ret[i].v[k].n = norm;
		}
	}
	swap_endianness(ret, is_little_endian());
	return ret;
}
texture_image packer::krc_file::get_texture_from_bytes(std::vector<unsigned char> &bytes, header_blob &blob)
{
	texture_image ret;
	ret.set_pixels(bytes);
	ret.w = get_vdata<unsigned>(blob.vdata, 0);
	ret.h = get_vdata<unsigned>(blob.vdata, sizeof(unsigned));
	bool image_decoded = (bool)get_vdata<unsigned char>(blob.vdata, sizeof(unsigned) * 2);
	if (!image_decoded)
		ret = texture_image::decode_png_data(ret.get_pixels());
	return ret;
}
void packer::krc_file::clear()
{
	header.clear();
	data.clear();
	data_stack_ptr = 0;
}
std::vector<unsigned char> packer::krc_file::get_file_bytes(const char *path)
{
	std::vector<unsigned char> data;

	std::ifstream fs(path, std::ios::binary | std::ios::ate);
	if (!fs.is_open())
		return data;
	size_t size = fs.tellg();
	fs.close();

	fs = std::ifstream(path, std::ios::binary);
	unsigned char *in = new unsigned char[size]{0};
	fs.read((char *)in, size);
	fs.close();
	for (size_t i = 0; i < size; ++i)
		data.push_back(in[i]);
	if (size > 0)
		DEL_ARR_PTR(in);
	return data;
}
void packer::krc_file::add_string(const char *name, const char *str)
{
	std::vector<unsigned char> data;
	uint64_t ctr = -1;
	char c;
	while ((c = str[++ctr]) != '\0')
		data.push_back((unsigned char)c);
	std::vector<unsigned char> vdata;
	add_uint64_t(ctr, vdata);
	while (vdata.size() < BLOB_VDATA_LEN)
		vdata.push_back(0);
	add_resource(name, krc::TEXT, data, &vdata[0]);
}