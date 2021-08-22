#include "material.h"

using namespace kvasir;
material_base::~material_base()
{
	for (size_t i = 0; i < get_texs().size(); ++i)
		if (get_texs()[i])
			DEL_PTR(get_texs()[i]);
	get_texs().clear();
	DEL_PTR(texs);
}
texture_base *&material_base::diffuse()
{
	return get_texs()[0];
}
texture_base *&material_base::specular()
{
	return get_texs()[1];
}
texture_base *&material_base::normal()
{
	return get_texs()[2];
}
texture_base *&material_base::parallax()
{
	return get_texs()[3];
}
void material_base::bind()
{
	for (size_t i = 0; i < get_texs().size(); ++i)
		if (get_texs()[i])
			get_texs()[i]->bind();
}

texture_base *&material_base::operator[](size_t index)
{
	return get_texs()[index];
}
std::vector<texture_base *> &material_base::get_texs()
{
	return *texs;
}