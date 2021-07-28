#ifndef KVASIR_PHYSICS_3D_H
#define KVASIR_PHYSICS_3D_H 1

#include <unordered_map>
#ifndef NO_USE_INCLUDES
#include "btBulletDynamicsCommon.h"
#include "physics.h"
#include "memory-aid.h"
#include "mesh.h"
#endif

using namespace galg;

namespace kvasir
{
	struct bullet_physics3d : physics3d
	{
		bullet_physics3d();
		~bullet_physics3d();

		void set_gravity(const vec3f &g);
		bool add_mesh(mesh3d &mesh, bool convex, float mass);
		void step(float delta);
		vec3f get_position(const mesh3d &mesh);
		vec4f get_rotation(const mesh3d &mesh);
		void set_position(const mesh3d &mesh, const vec3f &v);
		void set_rotation(const mesh3d &mesh, const vec4f &v);
		position3d get_transform(const mesh3d &mesh);
		void set_transform(const mesh3d &mesh, const position3d &trns);

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

		static btVector3 btV3(const vec3f &v);
		static vec3f btV3(const btVector3 &v);
		static vec4f btq_to_gq(const btQuaternion &q);
		static btQuaternion gq_to_btq(const vec4f &v);
	};
}

#endif