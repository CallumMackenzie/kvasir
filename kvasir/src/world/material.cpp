#include "material.h"

using namespace kvasir;
texture_base *material_base::get_diffuse()
{
	if (texs.size() == 0)
		return nullptr;
	return texs[0];
}
texture_base *material_base::get_specular()
{
	if (texs.size() <= 1)
		return nullptr;
	return texs[1];
}
texture_base *material_base::get_normal()
{
	if (texs.size() <= 2)
		return nullptr;
	return texs[2];
}
texture_base *material_base::get_parallax()
{
	if (texs.size() <= 3)
		return nullptr;
	return texs[3];
}
void material_base::bind()
{
	for (size_t i = 0; i < texs.size(); ++i)
		if (texs[i])
			texs[i]->bind();
}
