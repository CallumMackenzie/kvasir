#include "game-object.h"

using namespace kvasir;

game_object_3d::game_object_3d() {}
vec3f &game_object_3d::pos() { return mesh.pos; }
vec4f &game_object_3d::rot() { return mesh.rot; }
vec3f &game_object_3d::scale() { return mesh.scale; }