#include "common-ops.h"

using namespace galg;

void kvasir::cam_debug_controls(render_base *base, camera3d &cam, float delta, float mv_speed, float rot_speed, float spb1, float spb2)
{
	vec3f clv = cam.look_vector().xyz().normalized();
	float speed = mv_speed;
	float cms = rot_speed;
	vec3f forward;
	vec3f up = vec3f(0.f, 1.f, 0.f);
	if (base->key_pressed(KeyW))
		forward += clv;
	if (base->key_pressed(KeyS))
		forward -= clv;
	if (base->key_pressed(KeyD))
		forward -= clv.cross(up);
	if (base->key_pressed(KeyA))
		forward += clv.cross(up);
	if (base->key_pressed(KeyQ))
		forward.y() += 1.f;
	if (base->key_pressed(KeyE))
		forward.y() -= 1.f;
	if (base->key_pressed(LShift))
		speed *= 3.f;
	if (base->key_pressed(LControl))
		speed *= 7.f;

	cam.pos += forward.normalize() * speed * delta;

	cam_debug_rotation(base, cam, delta, rot_speed);
}
void kvasir::cam_debug_rotation(render_base *base, camera3d &cam, float delta, float rot_speed)
{
	vec3f rotate;
	// if (std::abs(cam.rot.y()) >= 3.14159f * 2.f)
	// 	cam.rot.y() = 0.f;
	if (base->key_pressed(Left))
		rotate.y() = rot_speed;
	if (base->key_pressed(Right))
		rotate.y() = -rot_speed;
	if (base->key_pressed(Up))
		rotate.x() = -rot_speed;
	if (base->key_pressed(Down))
		rotate.x() = rot_speed;
	cam.rot += rotate * delta;
	// if (cam.rot.x() >= 3.14159f / 2.1f)
	// 	cam.rot.x() = 3.14159f / 2.1f;
	// if (cam.rot.x() <= -3.14159f / 2.1f)
	// 	cam.rot.x() = -3.14159f / 2.1f;
}
kvasir::material_base *kvasir::make_material(render_base *base, const char *diffuse_img_path)
{
	material_base *mat = base->make_material();
	mat->get_texs()[0] = base->make_texture();
	mat->get_texs()[0]->make_png(diffuse_img_path);
	return mat;
}
kvasir::material_base *kvasir::make_material(render_base *base, long colour)
{
	material_base *mat = base->make_material();
	mat->get_texs()[0] = base->make_texture();
	mat->get_texs()[0]->make_colour(colour);
	return mat;
}