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

First 34 bytes is the static header describing file length.
-- 1st 3 bytes is the format signature ('k', 'r', 'c').
-- 2nd 8 bytes is buffer length in bytes (uint64_t).
-- 3rd 8 bytes is file length in bytes (uint64_t).

Next x bytes is the buffer describing data locations.
-- Contains globs of data with 
-- - 1 byte for data type
-- - 8 bytes for resource name
-- - 8 bytes for resource length
-- - 8 bytes for data start position (relative to data start point)
-- - 32 bytes for variable data.

Next x bytes is the data.

*/

#define KRC_TAG "krc"
#define BLOB_NAME_FILLER " "

#define BLOB_TYPE_LEN 1
#define BLOB_NAME_LEN 15
#define BLOB_LEN_LEN sizeof(uint64_t)
#define BLOB_POS_LEN sizeof(uint64_t)
#define BLOB_VDATA_LEN 32

#define SH_SIGNATURE_LEN (sizeof(KRC_TAG) - 1)
#define SH_BL_LEN sizeof(uint64_t)
#define SH_FL_LEN sizeof(uint64_t)

#define SH_SIGNATURE_LOC 0
#define SH_BL_LOC SH_SIGNATURE_LEN
#define SH_FL_LOC (SH_SIGNATURE_LEN + SH_BL_LEN)

#define BLOB_TYPE_LOC 0
#define BLOB_NAME_LOC BLOB_TYPE_LEN
#define BLOB_RES_LEN_LOC (BLOB_TYPE_LEN + BLOB_NAME_LEN)
#define BLOB_DAT_START_LOC (BLOB_TYPE_LEN + BLOB_NAME_LEN + BLOB_LEN_LEN)
#define BLOB_VDATA_LOC (BLOB_TYPE_LEN + BLOB_NAME_LEN + BLOB_LEN_LEN + BLOB_POS_LEN)

#define BLOB_LEN (BLOB_TYPE_LEN + BLOB_NAME_LEN + BLOB_LEN_LEN + BLOB_POS_LEN + BLOB_VDATA_LEN)
#define HEADER_STATIC_SIZE (SH_SIGNATURE_LEN + SH_BL_LEN + SH_FL_LEN)

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

	void get_bytes(unsigned n, unsigned char recv[sizeof(unsigned)]);
	void set_bytes(unsigned char bytes[sizeof(unsigned)], unsigned &recv);

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
			TEXTURE = 'T',
			UNKNOWN = 'U',
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
				char name[BLOB_NAME_LEN]{0};
				uint64_t len = 0;
				uint64_t ptr = 0;
				unsigned char vdata[BLOB_VDATA_LEN]{0};

				bool is_valid();
				std::string to_string();
			};

			std::vector<unsigned char> data;
			std::vector<unsigned char> header;
			uint64_t data_stack_ptr = 0;

			bool save(const char *path);
			std::vector<unsigned char> serialize();
			krc get_first_type();
			std::string get_first_name();

			void add_mesh3d(const char *name, std::vector<mesh3d::triangle> tris);
			void add_mesh3d(const char *name, mesh3d::triangle *tris, size_t ntris);
			void add_mesh3d_from_obj_data(const char *name, const char *obj_data);
			void add_mesh3d_from_obj_stream(const char *name, std::basic_istream<char, std::char_traits<char>> *stream);
			void add_mesh3d_from_obj_file(const char *name, const char *path);

			void add_texture(const char *name, const char *png_path, bool decode_image = true);
			void add_texture(const char *name, const texture_image &img, bool image_decoded = true);

			void add_resource(const char *name, krc type, const std::vector<unsigned char> &data, unsigned char vdata[BLOB_VDATA_LEN]);

			std::vector<unsigned char> get_resource_data(const char *name);
			std::vector<unsigned char> get_resource_data_from_blob(header_blob &blob);
			header_blob find_blob(const char *name);
			std::vector<mesh3d::triangle> get_mesh3d_data(const char *name);
			texture_image get_texture(const char *name);

			// Header should not contain data always present such as the 'krc' tag or the header/data lengths.
			static krc_file deserialize(std::vector<unsigned char> header, std::vector<unsigned char> dat);
			static krc_file deserialize(std::vector<unsigned char> all_data);
			static krc_file deserialize(const char *file_path);
			static void add_uint64_t(uint64_t val, std::vector<unsigned char> &f);
			static void add_float(float val, std::vector<unsigned char> &f);
			static void add_char(char val, std::vector<unsigned char> &f);
			static void add_cstr(const char *str, std::vector<unsigned char> &f);
			static void get_cstr(char *recv, size_t len, std::basic_fstream<unsigned char, std::char_traits<unsigned char>> &fs);
			static void add_header_section(std::vector<unsigned char> &header, krc type, const char *data_name, uint64_t data_pos, uint64_t data_len, unsigned char vdata[BLOB_VDATA_LEN]);
			static void add_data_section(std::vector<unsigned char> &data_dest, const std::vector<unsigned char> &data_src);
			static header_blob get_blob(unsigned char *blob_start);
			static header_blob get_blob_from_file(const char *file, const char *name);
			static std::vector<header_blob> get_blobs_in_file(const char *file);
			static std::vector<unsigned char> get_resource_data_from_blob_file(const header_blob &blob, const char *file);
			static std::vector<mesh3d::triangle> get_mesh3d_data_from_file(const char *name, const char *file);
			static std::vector<mesh3d::triangle> get_mesh3d_data_from_bytes(std::vector<unsigned char> &bytes);
			static texture_image get_texture_from_file(const char *name, const char *file);
			static texture_image get_texture_from_bytes(std::vector<unsigned char> &bytes, header_blob &blob);

			template <typename T>
			static void set_vdata_bytes(unsigned char vdata[BLOB_VDATA_LEN], size_t offset, T val)
			{
				unsigned char bytes[sizeof(T)]{0};
				if (is_big_endian())
					reverse_bytes(val);
				get_bytes(val, bytes);
				for (size_t i = 0; i < sizeof(T); ++i)
					vdata[i + offset] = bytes[i];
			}

			template <typename T>
			static T get_vdata(unsigned char vdata[BLOB_VDATA_LEN], size_t offset)
			{
				unsigned char bytes[sizeof(T)]{0};
				T ret = 0;
				for (size_t i = 0; i < sizeof(T); ++i)
					bytes[i] = vdata[i + offset];
				set_bytes(bytes, ret);
				if (is_big_endian())
					reverse_bytes<T>(ret);
				return ret;
			}

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

			template <typename T>
			static void add_bytes(std::vector<unsigned char> &v, T val)
			{
				unsigned char bytes[sizeof(T)]{0};
				if (is_big_endian())
					reverse_bytes(val);
				get_bytes(val, bytes);
				for (size_t i = 0; i < sizeof(T); ++i)
					v.push_back(bytes[i]);
			}

			template <typename T>
			static T get_num(unsigned char bytes[sizeof(T)])
			{
				T add = 0;
				set_bytes(bytes, add);
				if (is_big_endian())
					reverse_bytes(add);
				return add;
			}
		};

		krc_file obj_data_to_krc(const char *name, const char *obj_data);
		krc_file obj_stream_to_krc(const char *name, std::basic_istream<char, std::char_traits<char>> *stream);
	}
}

#endif