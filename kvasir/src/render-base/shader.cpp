#include "shader.h"

using namespace kvasir;

void shader_base::bind_material(material_base *base)
{
	base->bind();
}