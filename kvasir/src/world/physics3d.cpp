#include "physics3d.h"

using namespace galg;
using namespace kvasir;

physics3d::physics3d()
{
	configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(configuration);
	overlap_pair_cache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, overlap_pair_cache, solver, configuration);

	set_gravity(vec3f(0, -9.81f, 0));
}
physics3d::~physics3d()
{
	for (size_t i = world->getNumCollisionObjects() - 1; i >= 0; --i)
	{
		btCollisionObject *obj = world->getCollisionObjectArray()[i];
		btRigidBody *body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
			delete body->getMotionState();
		world->removeCollisionObject(obj);
		DEL_PTR(obj);
	}

	coll_shapes.clear();

	DEL_PTR(world);
	DEL_PTR(solver);
	DEL_PTR(overlap_pair_cache);
	DEL_PTR(dispatcher);
	DEL_PTR(configuration);
}
void physics3d::set_gravity(const vec3f &g)
{
	world->setGravity(btV3(g));
}
btVector3 physics3d::btV3(const vec3f &v)
{
	return btVector3(v.x(), v.y(), v.z());
}
vec3f physics3d::btV3(const btVector3 &v)
{
	return vec3f(v[0], v[1], v[2]);
}
bool physics3d::add_mesh(const mesh3d &mesh, bool convex)
{
	obj_info mesh_info;
	mesh3d::triangle *tris = nullptr;
	mesh.buffer->get_data(tris, 0, mesh.n_tris * sizeof(mesh3d::triangle));
	if (!tris)
		return false;
	if (convex)
	{
		mesh_info.shape = new btConvexHullShape();
		for (size_t i = 0; i < mesh.n_tris; ++i)
			for (size_t j = 0; j < 3; ++j)
				((btConvexHullShape *)mesh_info.shape)->addPoint(btV3(tris[i].v[j].p));
	}
	else
	{
		btTriangleMesh *bt_tri_mesh = new btTriangleMesh();
		for (size_t i = 0; i < mesh.n_tris; ++i)
		{
			btVector3 bv1 = btV3(tris[i].v[0].p),
					  bv2 = btV3(tris[i].v[1].p),
					  bv3 = btV3(tris[i].v[2].p);
			bt_tri_mesh->addTriangle(bv1, bv2, bv3);
		}
		mesh_info.shape = new btBvhTriangleMeshShape(bt_tri_mesh, true);
	}
	return true;
}
physics3d::obj_info::~obj_info()
{
	DEL_PTR(shape);
	DEL_PTR(rigid_body);
}