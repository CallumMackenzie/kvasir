#ifndef KVASIR_PHYSICS_3D_H
#define KVASIR_PHYSICS_3D_H 1

#include <unordered_map>
#ifndef NO_USE_INCLUDES
#include "exception.h"
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
		bool add_mesh(mesh3d &mesh, bool convex, const phys_props &props);
		bool add_mesh_sphere_hitbox(mesh3d &mesh, float diameter, const phys_props &props);
		bool add_mesh_box_hitbox(mesh3d &mesh, vec3f size, const phys_props &props);
		void create_mesh_hitbox_prefab(mesh3d &mesh, std::string key, bool convex);
		void step(float delta);
		vec3f get_position(const mesh3d &mesh);
		quaternionf get_rotation(const mesh3d &mesh);
		void set_position(const mesh3d &mesh, const vec3f &v);
		void set_rotation(const mesh3d &mesh, const quaternionf &v);
		position3d get_transform(const mesh3d &mesh);
		void set_transform(const mesh3d &mesh, const position3d &trns);

	private:
		struct obj_info
		{
			btCollisionShape *shape = nullptr;
			btRigidBody *body = nullptr;
		};

		btDefaultCollisionConfiguration *configuration = nullptr;
		btCollisionDispatcher *dispatcher = nullptr;
		btBroadphaseInterface *overlap_pair_cache = nullptr;
		btSequentialImpulseConstraintSolver *solver = nullptr;
		btDiscreteDynamicsWorld *world = nullptr;
		std::unordered_map<size_t, obj_info> coll_shapes;

		static inline bool use_coll_shape_cache = true;
		static inline std::unordered_map<std::string, btCollisionShape *> coll_shape_cache;

		bool mesh_is_valid(const mesh3d &mesh);
		btCollisionShape *create_mesh_hitbox(const mesh3d &mesh, bool convex);
		bool check_mesh_tracked(const mesh3d &mesh);
		btVector3 calculate_intertia(btCollisionShape *shape, float mass);
		btRigidBody *get_rigidbody(mesh3d &mesh, const phys_props &props, btCollisionShape *shape);

			static btVector3 btV3(const vec3f &v);
		static vec3f btV3(const btVector3 &v);
		static quaternionf btq_to_gq(const btQuaternion &q);
		static btQuaternion gq_to_btq(const quaternionf &v);
	};
}

#endif