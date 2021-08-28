#include "krc-packer.h"
#include <sstream>
#include <fstream>

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
	uint *n = (uint *)&na;
	for (size_t i = 0; i < sizeof(float); ++i)
		recv[(sizeof(float) - 1) - i] = (unsigned char)(*n >> (int)(i * 8));
}
void byte_ops::set_bytes(unsigned char bytes[sizeof(float)], float &rec)
{
	uint *recv = (uint *)&rec;
	for (size_t i = 0; i < sizeof(float); ++i)
		*recv |= bytes[(sizeof(float) - 1) - i] << (int)(i * 8);
}
void byte_ops::get_bytes(unsigned char n, unsigned char recv[sizeof(unsigned char)])
{
	recv[0] = n;
}
void byte_ops::set_bytes(unsigned char bytes[sizeof(unsigned char)], unsigned char &recv)
{
	recv = bytes[0];
}

using namespace kvasir;
using namespace byte_ops;

packer::krc_file packer::obj_data_to_krc(const char *obj_data)
{
	std::istringstream iss(obj_data);
	return obj_stream_to_krc(&iss);
}
packer::krc_file packer::obj_stream_to_krc(std::basic_istream<char, std::char_traits<char>> *stream)
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
	unsigned char header_vdata[HEADER_VDATA_LEN]{0};
	krc_file::add_header_section(ret.header, krc::MESH3D, ret.data_stack_ptr, header_vdata);
	ret.data_stack_ptr += ch_len;
	return ret;
}
std::vector<unsigned char> packer::krc_file::serialize()
{
	std::vector<unsigned char> ser;
	add_cstr("krc", ser);
	add_uint64_t(header.size(), ser);
	add_uint64_t(header.size() + data.size(), ser);

	for (size_t i = 0; i < header.size(); ++i)
		ser.push_back(header[i]);
	for (size_t i = 0; i < data.size(); ++i)
		ser.push_back(data[i]);
	return ser;
}
packer::krc_file packer::krc_file::deserialize(std::vector<unsigned char> header, std::vector<unsigned char> dat, uint64_t header_len, uint64_t data_len)
{

	return krc_file();
}
bool packer::krc_file::save(const char *path)
{
	std::vector<unsigned char> dat = serialize();
	std::ofstream fs;
	fs.open(path);
	fs.write((const char *)&dat[0], dat.size());
	fs.close();
	return true;
}
packer::krc_file packer::krc_file::deserialize(const char *file_path)
{
	std::basic_fstream<unsigned char, std::char_traits<unsigned char>> fs(file_path);
	if (!fs.is_open())
		return krc_file();

	char start_str[4]{0};
	get_cstr(&start_str[0], 3, fs);
	if (start_str[0] != 'k' || start_str[1] != 'r' || start_str[2] != 'c')
	{
		std::cerr << "File is not a proper KRC file." << std::endl;
		return krc_file();
	}
	uint64_t header_len = get<uint64_t>(fs);
	uint64_t file_len = get<uint64_t>(fs);
	uint64_t data_len = file_len - header_len;
	std::vector<unsigned char> fdata;
	std::vector<unsigned char> hdata;

	auto get_pow2 = [](uint64_t startctr, uint64_t val)
	{
		uint64_t ret = 0;
		while ((val % (ret = (uint64_t)std::powl(2, (long double)--startctr))) != 0)
			if (((int64_t)startctr - 1) == -1)
				break;
		return ret;
	};

	auto read_file = [&fs](std::vector<unsigned char> &dest, uint64_t buff_sz, uint64_t len)
	{
		unsigned char *buff = new unsigned char[buff_sz];
		uint64_t curr_len = 0;
		while (!fs.eof())
		{
			fs.read(buff, buff_sz);
			for (size_t i = 0; i < buff_sz; ++i)
				dest.push_back(buff[i]);
			curr_len += buff_sz;
			if (curr_len >= len)
				break;
		}
		delete[] buff;
		buff = nullptr;
	};

	read_file(hdata, get_pow2(6, header_len), header_len);
	read_file(fdata, get_pow2(8, data_len), data_len);

	return deserialize(hdata, fdata, header_len, data_len);
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
void packer::krc_file::get_cstr(char *recv, size_t len, std::basic_fstream<unsigned char, std::char_traits<unsigned char>> &fs)
{
	for (size_t i = 0; i < len; ++i)
		recv[i] = get<unsigned char>(fs);
}
void packer::krc_file::add_header_section(std::vector<unsigned char> &header, krc type, uint64_t data_pos, unsigned char vdata[HEADER_VDATA_LEN])
{
	header.push_back((unsigned char)type);
	add_uint64_t(data_pos, header);
	for (size_t i = 0; i < HEADER_VDATA_LEN; ++i)
		header.push_back(vdata[i]);
}