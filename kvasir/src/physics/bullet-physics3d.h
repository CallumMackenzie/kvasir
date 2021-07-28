#ifndef KVASIR_PHYSICS_3D_H
#define KVASIR_PHYSICS_3D_H 1

#include <unordered_map>
#ifndef NO_USE_INCLUDES
#include "btBulletDynamicsCommon.h"
#include "physics.h"
#include "memory-aid.h"
#include "mesh.h"
#endif

namespace kvasir
{
	struct bullet_physics3d : physics3d
	{
		bullet_physics3d();
		~bullet_physics3d();

		void set_gravity(const galg::vec3f &g);
		bool add_mesh(mesh3d &mesh, bool convex, float mass);
		void step(float delta);
		galg::vec3f get_position(const mesh3d &mesh);
		galg::vec4f get_rotation(const mesh3d &mesh);

	private:
		struct obj_info
		{
			btCollisionShape *shape = nullptr;
			btRigidBody *body = nullptr;
			obj_info &operator=(const obj_info &o);
		};

		btDefaultCollisionConfiguration *configuration = nullptr;
		btCollisionDispatcher *dispatcher = nullptr;
		btBroadphaseInterface *overlap_pair_cache = nullptr;
		btSequentialImpulseConstraintSolver *solver = nullptr;
		btDiscreteDynamicsWorld *world = nullptr;
		std::unordered_map<size_t, obj_info> coll_shapes;

		bool mesh_is_valid(const mesh3d &mesh);

		static btVector3 btV3(const galg::vec3f &v);
		static galg::vec3f btV3(const btVector3 &v);
	};
}

#endif