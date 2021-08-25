#include "galg_ops.h"

galg::str_fmt_group::str_fmt_group(const char op, const char cl, const char br, const char nl) : open{op, '\0'}, close{cl, '\0'}, seper{br, '\0'}, newline{nl, '\0'} {}
galg::str_fmt_group galg::enclosing_chars(galg::str_fmt fmt)
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
std::string galg::to_string(bool b)
{
	return std::string(b ? "true" : "false");
}