#include "common-ops.h"

using namespace galg;

void kvasir::cam_debug_controls(render_base *base, camera3d &cam, float delta, float mv_speed, float rot_speed, float spb1, float spb2)
{
	vec3f clv = cam.look_vector().xyz().normalized();
	float speed = mv_speed;
	float cms = rot_speed;
	vec3f forward;
	vec3f up = vec3f(0.f, 1.f, 0.f);
	vec3f rotate;
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
	if (base->key_pressed(Left))
		rotate.y() = cms;
	if (base->key_pressed(Right))
		rotate.y() = -cms;
	if (base->key_pressed(Up))
		rotate.x() = -cms;
	if (base->key_pressed(Down))
		rotate.x() = cms;
	if (base->key_pressed(LShift))
		speed *= 3.f;
	if (base->key_pressed(LControl))
		speed *= 7.f;

	cam.rot += rotate * delta;
	cam.pos += forward.normalize() * speed * delta;
}