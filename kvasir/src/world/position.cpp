#include "position.h"

using namespace kvasir;
vec3f &position3d::get_pos() { return pos; }
quaternionf &position3d::get_rot() { return rot; }
vec3f &position3d::get_scale() { return scale; }
vec3f &position3d::get_rot_pt() { return rot_pt; }
vec2f &position2d::get_pos() { return pos; }
float &position2d::get_rot() { return rot; }
vec2f &position2d::get_scale() { return scale; }
vec2f &position2d::get_rot_pt() { return rot_pt; }