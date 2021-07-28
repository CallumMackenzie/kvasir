#ifndef KVASIR_PHYSICS_H_
#define KVASIR_PHYSICS_H_ 1

#ifndef NO_USE_INCLUDES
#include "galg.h"
#include "mesh.h"
#endif

namespace kvasir
{

	struct physics3d
	{
		virtual ~physics3d() = 0;

		virtual bool add_mesh(mesh3d &mesh, bool convex, float mass) = 0;
		virtual void set_gravity(const galg::vec3f &g) = 0;
		virtual void step(float delta) = 0;
		virtual galg::vec3f get_position(const mesh3d &mesh) = 0;
		virtual galg::vec4f get_rotation(const mesh3d &mesh) = 0;
	};

	struct physics2d
	{
		// TODO
	};

}

#endif