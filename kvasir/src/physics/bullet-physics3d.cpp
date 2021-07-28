#include "bullet-physics3d.h"

using namespace galg;
using namespace kvasir;

bullet_physics3d::bullet_physics3d()
{
	configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(configuration);
	overlap_pair_cache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, overlap_pair_cache, solver, configuration);

	set_gravity(vec3f(0, -9.81f, 0));
}
bullet_physics3d::~bullet_physics3d()
{
	for (size_t i = 0; i < coll_shapes.size(); ++i)
	{
		DEL_PTR(coll_shapes[i].body);
		DEL_PTR(coll_shapes[i].shape);
	}
	coll_shapes.clear();

	DEL_PTR(world);
	DEL_PTR(solver);
	DEL_PTR(overlap_pair_cache);
	DEL_PTR(dispatcher);
	DEL_PTR(configuration);
}
void bullet_physics3d::set_gravity(const vec3f &g)
{
	world->setGravity(btV3(g));
}
btVector3 bullet_physics3d::btV3(const vec3f &v)
{
	return btVector3(v.x(), v.y(), v.z());
}
vec3f bullet_physics3d::btV3(const btVector3 &v)
{
	return vec3f(v[0], v[1], v[2]);
}
bool bullet_physics3d::add_mesh(mesh3d &mesh, bool convex, float mass)
{
	if (coll_shapes.find(mesh.tag) != coll_shapes.end())
		return true;
	obj_info mesh_info;
	mesh3d::triangle *tris = new mesh3d::triangle[mesh.n_tris];
	mesh.buffer->get_data(tris, 0, mesh.n_tris * sizeof(mesh3d::triangle));
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
	DEL_ARR_PTR(tris);

	btDefaultMotionState *motion_state = new btDefaultMotionState(btTransform(
		btQuaternion(mesh.rot.x(), mesh.rot.y(), mesh.rot.z(), mesh.rot.w()), btV3(mesh.pos)));

	btVector3 b_inertia;
	mesh_info.shape->calculateLocalInertia((btScalar)mass, b_inertia);
	btRigidBody::btRigidBodyConstructionInfo body_ci = btRigidBody::btRigidBodyConstructionInfo((btScalar)mass, motion_state, mesh_info.shape, b_inertia);
	body_ci.m_restitution = 0.1f;
	body_ci.m_friction = 0.5f;

	mesh_info.body = new btRigidBody(body_ci);
	mesh_info.body->setUserPointer(&mesh);
	// mesh_info.body->setLinearFactor(btVector3(1, 1, 0));

	coll_shapes[mesh.tag] = mesh_info;
	world->addRigidBody(mesh_info.body);
	return true;
}
void bullet_physics3d::step(float delta)
{
	world->stepSimulation((btScalar)delta);
}
bool bullet_physics3d::mesh_is_valid(const mesh3d &mesh)
{
	if (coll_shapes.find(mesh.tag) == coll_shapes.end())
	{
		std::cerr << "Mesh (tag " << mesh.tag << ") not logged in physics simulation." << std::endl;
		return false;
	}
	if (!coll_shapes[mesh.tag].body || !coll_shapes[mesh.tag].shape)
	{
		std::cerr << "Mesh (tag " << mesh.tag << ") has null body or shape." << std::endl;
		return false;
	}
	return true;
}
vec3f bullet_physics3d::get_position(const mesh3d &mesh)
{
	if (!mesh_is_valid(mesh))
		return vec3f();
	btTransform trns = coll_shapes[mesh.tag].body->getWorldTransform();
	return btV3(trns.getOrigin());
}
vec4f bullet_physics3d::get_rotation(const mesh3d &mesh)
{
	if (!mesh_is_valid(mesh))
		return vec4f();
	btTransform trns = coll_shapes[mesh.tag].body->getWorldTransform();
	btQuaternion q = trns.getRotation();
	return vec4f(q.x(), q.y(), q.z(), q.w());
}
bullet_physics3d::obj_info &bullet_physics3d::obj_info::operator=(const obj_info &o)
{
	body = o.body;
	shape = o.shape;
	return *this;
}