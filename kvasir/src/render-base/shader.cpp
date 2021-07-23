#include "shader.h"

using namespace kvasir;

shader_base::~shader_base() {}
void shader_base::bind_material(material_base *base)
{
	base->bind();
}