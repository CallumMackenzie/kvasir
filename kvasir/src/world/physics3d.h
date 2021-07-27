#ifndef KVASIR_PHYSICS_3D_H
#define KVASIR_PHYSICS_3D_H 1

#include <unordered_map>
#ifndef NO_USE_INCLUDES
#include "btBulletDynamicsCommon.h"
#include "galg.h"
#include "memory-aid.h"
#include "mesh.h"
#endif

namespace kvasir
{
	struct physics3d
	{
		physics3d();
		~physics3d();

		void set_gravity(const galg::vec3f &g);
		bool add_mesh(const mesh3d &mesh, bool convex);

	private:
		struct obj_info
		{
			~obj_info();

			btCollisionShape *shape = nullptr;
			btRigidBody *rigid_body = nullptr;
		};

		btDefaultCollisionConfiguration *configuration = nullptr;
		btCollisionDispatcher *dispatcher = nullptr;
		btBroadphaseInterface *overlap_pair_cache = nullptr;
		btSequentialImpulseConstraintSolver *solver = nullptr;
		btDiscreteDynamicsWorld *world = nullptr;
		std::unordered_map<size_t, obj_info> coll_shapes;

		static btVector3 btV3(const galg::vec3f &v);
		static galg::vec3f btV3(const btVector3 &v);
	};
}

#endif