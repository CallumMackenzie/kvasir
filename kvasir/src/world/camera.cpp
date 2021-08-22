#include "camera.h"

using namespace kvasir;

mat4f camera3d::view()
{
	return mat4f::look_at(pos, (mat4f::rotation(rot) * vec4f(0, 0, 1)).xyz() + pos, vec3f(0, 1, 0));
}
mat4f camera3d::perspective()
{
	return mat4fp::perspective(fov, aspect, near, far);
}
vec4f camera3d::look_vector()
{
	return mat4f::rotation(rot) * vec4f(0, 0, 1);
}
vec3f &camera3d::get_pos() { return pos; }
vec3f &camera3d::get_rot() { return rot; }