#ifndef KVASIR_PHYSICS_H_
#define KVASIR_PHYSICS_H_ 1

#ifndef NO_USE_INCLUDES
#include "galg.h"
#include "mesh.h"
#endif

using namespace galg;

namespace kvasir
{

	struct physics3d
	{
		struct phys_props
		{
			float mass = 0;
			float friction = 1.f;
			float restitution = 0;
			phys_props(float mass = 0, float friction = 1.f, float rest = 0);
		};
		static phys_props static_props(float friction = 1.f, float restitution = 0);
		static phys_props dynamic_props(float mass, float friction = 1.f, float restitution = 0);

		virtual ~physics3d() = 0;
		virtual void add_mesh(mesh3d &mesh, bool convex, const phys_props &props) = 0;
		virtual void add_mesh_sphere_hitbox(mesh3d &mesh, float diameter, const phys_props &props) = 0;
		virtual void add_mesh_box_hitbox(mesh3d &mesh, vec3f size, const phys_props &props) = 0;
		virtual void add_central_force(const mesh3d &mesh, vec3f force) = 0;
		virtual void add_mesh_prefab_hitbox(mesh3d &mesh, std::string key, const phys_props &props) = 0;
		virtual void set_gravity(const vec3f &g) = 0;
		virtual void step(float delta_seconds) = 0;
		virtual vec3f get_position(const mesh3d &mesh) = 0;
		virtual quaternionf get_rotation(const mesh3d &mesh) = 0;
		virtual void set_position(const mesh3d &mesh, const vec3f &v) = 0;
		virtual void set_rotation(const mesh3d &mesh, const quaternionf &v) = 0;
		virtual position3d get_transform(const mesh3d &mesh) = 0;
		virtual void set_transform(const mesh3d &mesh, const position3d &trns) = 0;
		virtual void create_mesh_hitbox_prefab(mesh3d &mesh, std::string key, bool convex) = 0;
		virtual void activate(const mesh3d &mesh) = 0;
		virtual mesh3d *raycast_first_hit(vec3f from, vec3f to) = 0;
		virtual void set_rotation_axes(const mesh3d &mesh, vec3f axes) = 0;
		virtual void set_position_axes(const mesh3d &mesh, vec3f axes) = 0;
		virtual vec3f get_velocity(const mesh3d &mesh) = 0;
		virtual vec3f get_angular_velocity(const mesh3d &mesh) = 0;
		virtual void set_velocity(const mesh3d &mesh, const vec3f &lv) = 0;
		virtual void set_angular_velocity(const mesh3d &mesh, const vec3f &lv) = 0;
		virtual float get_mass(const mesh3d &mesh) = 0;
	};

	struct physics2d
	{
		// TODO
	};

}

#endif