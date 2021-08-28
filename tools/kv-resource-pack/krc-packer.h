#ifndef KVASIR_PACKER_H
#define KVASIR_PACKER_H 1

#include "kvasir-eng-include.h"
#include <vector>
#include <string>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <exception>

/*

.krc file type spec.

Numbers will always be little endian.
1st 3 bytes is the format signature ('k', 'r', 'c').
2nd 8 bytes is buffer length in bytes (uint64_t).
3rd 8 bytes is file length in bytes (uint64_t).
Next x bytes is the buffer describing data locations.
-- Contains globs of data with 1 byte for data type, 8 bytes for resource name, 8 bytes for resource length, 8 bytes for data start position (relative to data start point), and 32 bytes for variable data.
Next x bytes is the data.

*/

#define HEADER_VDATA_LEN 32
#define HEADER_BLOB_LEN (HEADER_VDATA_LEN + 1 + 8 + 8 + 8)
#define HEADER_STATIC_SIZE (3 + 8 + 8)

namespace byte_ops
{
	bool is_big_endian();
	bool is_little_endian();
	// Assumes the numeric values are the same endianness as the system.
	void swap_endianness(std::vector<kvasir::mesh3d::triangle> &vec, bool endianness);
	// Assumes v is the same endianness of the system.
	void swap_endianness(galg::vec3f &v, bool endianness);
	// Assumes v is the same endianness as the system.
	void swap_endianness(galg::vec2f &v, bool endianness);
	// Assumes tri vals are the same endianness as the system.
	void swap_endianness(kvasir::mesh3d::triangle &tri, bool endianness);
	// Assumes v is the same endianness as the system.
	void swap_endianness(uint64_t &v, bool endianness);

	// Assumes v is the same endianness as the system.
	template <typename T>
	void make_big_endian(T &v)
	{
		if (is_big_endian())
			return;
		reverse_bytes<T>(v);
	}

	void get_bytes(uint64_t n, unsigned char recv[sizeof(uint64_t)]);
	void set_bytes(unsigned char bytes[sizeof(uint64_t)], uint64_t &recv);

	void get_bytes(unsigned char n, unsigned char recv[sizeof(unsigned char)]);
	void set_bytes(unsigned char bytes[sizeof(unsigned char)], unsigned char &recv);

	void get_bytes(float n, unsigned char recv[sizeof(float)]);
	void set_bytes(unsigned char bytes[sizeof(float)], float &recv);

	template <typename T>
	void reverse_bytes(T &rev)
	{
		unsigned char rv[sizeof(T)]{0};
		get_bytes(rev, rv);
		unsigned char rt[sizeof(T)]{0};
		for (size_t i = 0; i < sizeof(T); ++i)
			rt[i] = rv[(sizeof(T) - 1) - i];
		set_bytes(rt, rev);
	}
}

namespace kvasir
{
	namespace packer
	{
		enum class krc : unsigned char
		{
			MESH3D = 'M',
			UNKNOWN = 'U'
		};
		enum class krc_mesh3d_fmt : unsigned char
		{
			V,
			VN,
			VT,
			VTN
		};
		struct krc_file
		{
			struct header_blob
			{
				krc type = krc::UNKNOWN;
				char name[8]{0};
				uint64_t len = 0;
				uint64_t ptr = 0;
				unsigned char vbuff[HEADER_VDATA_LEN]{0};

				bool is_valid();
			};

			std::vector<unsigned char> data;
			std::vector<unsigned char> header;
			uint64_t data_stack_ptr = 0;

			bool save(const char *path);
			std::vector<unsigned char> serialize();
			krc get_first_type();
			std::string get_first_name();

			void *get_resource_data(const char *name);
			void *get_resource_data_from_blob(header_blob &blob);
			header_blob find_blob(const char *name);

			// Header should not contain data always present such as the 'krc' tag or the header/data lengths.
			static krc_file deserialize(std::vector<unsigned char> header, std::vector<unsigned char> dat);
			static krc_file deserialize(std::vector<unsigned char> all_data);
			static krc_file deserialize(const char *file_path);

			static void add_uint64_t(uint64_t val, std::vector<unsigned char> &f);
			static void add_float(float val, std::vector<unsigned char> &f);
			static void add_char(char val, std::vector<unsigned char> &f);
			static void add_cstr(const char *str, std::vector<unsigned char> &f);
			static void get_cstr(char *recv, size_t len, std::basic_fstream<unsigned char, std::char_traits<unsigned char>> &fs);
			static void add_header_section(std::vector<unsigned char> &header, krc type, const char *data_name, uint64_t data_pos, uint64_t data_len, unsigned char vdata[HEADER_VDATA_LEN]);
			static header_blob get_blob(unsigned char *blob_start);

			template <typename T>
			static T get(std::basic_fstream<unsigned char, std::char_traits<unsigned char>> &fs)
			{
				unsigned char t_buffer[sizeof(T)]{0};
				fs.read(t_buffer, sizeof(T));
				T ret = 0;
				set_bytes(t_buffer, ret);
				if (is_big_endian())
					reverse_bytes<T>(ret);
				return ret;
			}
		};

		krc_file obj_data_to_krc(const char *name, const char *obj_data);
		krc_file obj_stream_to_krc(const char *name, std::basic_istream<char, std::char_traits<char>> *stream);
	}
}

#endif