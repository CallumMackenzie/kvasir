#ifndef GALG_OPS_H
#define GALG_OPS_H 1

#ifndef NO_STD_INCLUDES
#include <string>
#endif

#ifndef NO_USE_INCLUDES
#include "galg.h"
#endif

namespace galg
{
	enum class str_fmt
	{
		algebraic,
		array,
		list,
		inline_array,
		bare
	};
	struct str_fmt_group
	{
		const char open[2];
		const char close[2];
		const char seper[2];
		const char newline[2];
		str_fmt_group(const char op, const char cl, const char br, const char nl) : open{op, '\0'}, close{cl, '\0'}, seper{br, '\0'}, newline{nl, '\0'} {}
	};
	str_fmt_group enclosing_chars(str_fmt fmt)
	{
		switch (fmt)
		{
		case str_fmt::algebraic:
			return str_fmt_group('|', '|', ' ', '\n');
		case str_fmt::array:
			return str_fmt_group('{', '}', ',', '\n');
		case str_fmt::list:
			return str_fmt_group('(', ')', ',', '\n');
		case str_fmt::inline_array:
			return str_fmt_group('{', '}', ',', ' ');
		case str_fmt::bare:
			return str_fmt_group(' ', ' ', ' ', '\n');
		}
		return str_fmt_group('?', '?', '?', '?');
	}
	template <typename T>
	std::string to_string(const vec4<T> &vec, str_fmt fmt = str_fmt::array)
	{
		auto ech = enclosing_chars(fmt);
		std::string ret(ech.open);
		for (size_s i = 0; i < 4; ++i)
			ret.append(std::to_string(vec[i])).append(i == 3 ? ech.close : ech.seper);
		return ret;
	}
	template <typename T>
	std::string to_string(const vec3<T> &vec, str_fmt fmt = str_fmt::array)
	{
		auto ech = enclosing_chars(fmt);
		std::string ret(ech.open);
		for (size_s i = 0; i < 3; ++i)
			ret.append(std::to_string(vec[i])).append(i == 2 ? ech.close : ech.seper);
		return ret;
	}
	template <typename T>
	std::string to_string(const vec2<T> &vec, str_fmt fmt = str_fmt::array)
	{
		auto ech = enclosing_chars(fmt);
		return std::string(ech.open).append(std::to_string(vec.x())).append(ech.seper).append(std::to_string(vec.y())).append(ech.close);
	}
	template <typename mat_x, size_s nelems>
	std::string to_string_mat_x(const mat_x &m, str_fmt fmt = str_fmt::array)
	{
		auto ech = enclosing_chars(fmt);
		std::string ret;
		for (size_s i = 0; i < nelems; ++i)
		{
			ret.append(ech.open);
			for (size_s j = 0; j < nelems; ++j)
			{
				ret.append(std::to_string(m[i][j]));
				if (j == nelems - 1)
				{
					if (i == nelems - 1)
						ret.append(ech.close);
					else
						ret.append(ech.close).append(ech.seper).append(ech.newline);
				}
				else
					ret.append(ech.seper);
			}
		}
		return ret;
	}
	template <typename T>
	std::string to_string(const mat2<T> &m, str_fmt fmt = str_fmt::array)
	{
		return to_string_mat_x<mat2<T>, 2>(m, fmt);
	}
	template <typename T>
	std::string to_string(const mat3<T> &m, str_fmt fmt = str_fmt::array)
	{
		return to_string_mat_x<mat3<T>, 3>(m, fmt);
	}
	template <typename T>
	std::string to_string(const mat4<T> &m, str_fmt fmt = str_fmt::array)
	{
		return to_string_mat_x<mat4<T>, 4>(m, fmt);
	}
	std::string to_string(bool b)
	{
		return std::string(b ? "true" : "false");
	}
}

#endif