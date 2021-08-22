#include "material.h"

using namespace kvasir;
material_base::~material_base()
{
	for (size_t i = 0; i < texs.size(); ++i)
		if (texs[i])
			DEL_PTR(texs[i]);
	texs.clear();
}
texture_base *&material_base::diffuse()
{
	return texs[0];
}
texture_base *&material_base::specular()
{
	return texs[1];
}
texture_base *&material_base::normal()
{
	return texs[2];
}
texture_base *&material_base::parallax()
{
	return texs[3];
}
void material_base::bind()
{
	for (size_t i = 0; i < texs.size(); ++i)
		if (texs[i])
			texs[i]->bind();
}

texture_base *&material_base::operator[](size_t index)
{
	return texs[index];
}
std::vector<texture_base *> &material_base::get_texs() { return texs; }