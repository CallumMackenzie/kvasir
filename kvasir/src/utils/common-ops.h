#ifndef KVASIR_COMMON_OPS_H
#define KVASIR_COMMON_OPS_H 1

#ifndef NO_USE_INCLUDES
#include "galg.h"
#include "renderer.h"
#include "mesh.h"
#include "camera.h"
#endif

namespace kvasir
{
	void cam_debug_controls(render_base *base, camera3d &cam, float delta, float mv_speed = 3.f, float rot_speed = 1.4f, float spb1 = 3.f, float spb2 = 7.f);
	void cam_debug_rotation(render_base *base, camera3d &cam, float delta, float rot_speed = 1.4f);
	material_base *make_material(render_base *base, const char *diffuse_img_path);
	material_base *make_material(render_base *base, long diffuse_colour);

}

#endif