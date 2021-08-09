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
	for (size_t i = world->getNumCollisionObjects() - 1; i > 0; --i)
	{
		btCollisionObject *obj = world->getCollisionObjectArray()[(int)i];
		btRigidBody *body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
			delete body->getMotionState();
		world->removeCollisionObject(obj);
		delete obj;
	}

	for (size_t i = 0; i < coll_shapes.size(); ++i)
		DEL_PTR(coll_shapes[i].shape);
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
void bullet_physics3d::add_mesh(mesh3d &mesh, bool convex, const phys_props &props)
{
	if (check_mesh_tracked(mesh))
		return;
	obj_info mesh_info;
	mesh_info.shape = create_mesh_hitbox(mesh, convex);
	mesh_info.body = get_rigidbody(mesh, props, mesh_info.shape);
	coll_shapes[mesh.tag] = mesh_info;
	world->addRigidBody(mesh_info.body);
}
void bullet_physics3d::step(float delta)
{
	world->stepSimulation((btScalar)delta * 1000.f);
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
	return btV3(coll_shapes[mesh.tag].body->getWorldTransform().getOrigin());
}
quaternionf bullet_physics3d::get_rotation(const mesh3d &mesh)
{
	if (!mesh_is_valid(mesh))
		return quaternionf();
	return btq_to_gq(coll_shapes[mesh.tag].body->getWorldTransform().getRotation());
}
void bullet_physics3d::set_position(const mesh3d &mesh, const vec3f &v)
{
	if (!mesh_is_valid(mesh))
		return;
	coll_shapes[mesh.tag].body->getWorldTransform().setOrigin(btV3(v));
	activate(mesh);
}
void bullet_physics3d::set_rotation(const mesh3d &mesh, const quaternionf &v)
{
	if (!mesh_is_valid(mesh))
		return;
	coll_shapes[mesh.tag].body->getWorldTransform().setRotation(gq_to_btq(v));
	activate(mesh);
}
position3d bullet_physics3d::get_transform(const mesh3d &mesh)
{
	position3d ret;
	if (!mesh_is_valid(mesh))
		return ret;
	auto wt = coll_shapes[mesh.tag].body->getWorldTransform();
	ret.pos = btV3(wt.getOrigin());
	ret.rot = btq_to_gq(wt.getRotation());
	return ret;
}
void bullet_physics3d::set_transform(const mesh3d &mesh, const position3d &trns)
{
	if (!mesh_is_valid(mesh))
		return;
	btTransform &wt = coll_shapes[mesh.tag].body->getWorldTransform();
	wt.setOrigin(btV3(trns.pos));
	wt.setRotation(gq_to_btq(trns.rot));
	activate(mesh);
}
quaternionf bullet_physics3d::btq_to_gq(const btQuaternion &q)
{
	return quaternionf(-q.x(), -q.y(), -q.z(), q.w());
}
btQuaternion bullet_physics3d::gq_to_btq(const quaternionf &v)
{
	return btQuaternion(-v.x(), -v.y(), -v.z(), v.w());
}
void bullet_physics3d::add_mesh_sphere_hitbox(mesh3d &mesh, float diameter, const phys_props &props)
{
	if (check_mesh_tracked(mesh))
		return;
	obj_info mesh_info;
	mesh_info.shape = new btSphereShape(diameter);
	mesh_info.body = get_rigidbody(mesh, props, mesh_info.shape);

	coll_shapes[mesh.tag] = mesh_info;
	world->addRigidBody(mesh_info.body);
}
btCollisionShape *bullet_physics3d::create_mesh_hitbox(const mesh3d &mesh, bool convex)
{
	btCollisionShape *ret = nullptr;
	mesh3d::triangle *tris = new mesh3d::triangle[mesh.n_tris];
	mesh.buffer->get_data(tris, 0, mesh.n_tris * sizeof(mesh3d::triangle));
	if (convex)
	{
		ret = new btConvexHullShape();
		for (size_t i = 0; i < mesh.n_tris; ++i)
			for (size_t j = 0; j < 3; ++j)
				((btConvexHullShape *)ret)->addPoint(btV3(tris[i].v[j].p));
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
		ret = new btBvhTriangleMeshShape(bt_tri_mesh, true);
	}
	DEL_ARR_PTR(tris);
	return ret;
}
bool bullet_physics3d::check_mesh_tracked(const mesh3d &mesh) { return coll_shapes.find(mesh.tag) != coll_shapes.end(); }
btVector3 bullet_physics3d::calculate_intertia(btCollisionShape *shape, float mass)
{
	btVector3 b_inertia(0, 0, 0);
	if (mass != 0.f)
		shape->calculateLocalInertia((btScalar)mass, b_inertia);
	return b_inertia;
}
btRigidBody *bullet_physics3d::get_rigidbody(mesh3d &mesh, const phys_props &props, btCollisionShape *shape)
{
	btDefaultMotionState *motion_state = new btDefaultMotionState(btTransform(gq_to_btq(mesh.rot), btV3(mesh.pos)));
	btRigidBody::btRigidBodyConstructionInfo body_ci =
		btRigidBody::btRigidBodyConstructionInfo(
			(btScalar)props.mass, motion_state, shape,
			calculate_intertia(shape, props.mass));
	body_ci.m_restitution = props.restitution;
	body_ci.m_friction = props.friction;
	btRigidBody *ret = new btRigidBody(body_ci);
	ret->setUserPointer(&mesh);
	return ret;
}
void bullet_physics3d::add_mesh_box_hitbox(mesh3d &mesh, vec3f size, const phys_props &props)
{
	if (check_mesh_tracked(mesh))
		return;
	obj_info mesh_info;
	mesh_info.shape = new btBoxShape(btV3(size));
	mesh_info.body = get_rigidbody(mesh, props, mesh_info.shape);

	coll_shapes[mesh.tag] = mesh_info;
	world->addRigidBody(mesh_info.body);
}
void bullet_physics3d::create_mesh_hitbox_prefab(mesh3d &mesh, std::string key, bool convex)
{
	coll_shape_cache[key] = create_mesh_hitbox(mesh, convex);
}
void bullet_physics3d::add_mesh_prefab_hitbox(mesh3d &mesh, std::string key, const phys_props &props)
{
	if (check_mesh_tracked(mesh))
		return;
	obj_info mesh_info;
	mesh_info.shape = coll_shape_cache[key];
	mesh_info.body = get_rigidbody(mesh, props, mesh_info.shape);

	coll_shapes[mesh.tag] = mesh_info;
	world->addRigidBody(mesh_info.body);
}
void bullet_physics3d::add_central_force(const mesh3d &mesh, vec3f force)
{
	if (!mesh_is_valid(mesh))
		return;
	activate(mesh);
	coll_shapes[mesh.tag].body->applyCentralImpulse(btV3(force));
}
void bullet_physics3d::activate(const mesh3d &mesh)
{
	if (!mesh_is_valid(mesh))
		return;
	coll_shapes[mesh.tag].body->activate(true);
}
mesh3d *bullet_physics3d::raycast_first_hit(vec3f from_, vec3f to_)
{
	btVector3 from = btV3(from_);
	btVector3 to = btV3(to_);

	btCollisionWorld::ClosestRayResultCallback closest(from, to);

	world->rayTest(from, to, closest);

	if (closest.hasHit())
		return (mesh3d *)closest.m_collisionObject->getUserPointer();
	else
		return nullptr;
}
void bullet_physics3d::set_rotation_axes(const mesh3d &mesh, vec3f axes)
{
	coll_shapes[mesh.tag].body->setAngularFactor(btV3(axes));
}
void bullet_physics3d::set_position_axes(const mesh3d &mesh, vec3f axes)
{
	coll_shapes[mesh.tag].body->setLinearFactor(btV3(axes));
}
vec3f bullet_physics3d::get_velocity(const mesh3d &mesh)
{
	return btV3(coll_shapes[mesh.tag].body->getLinearVelocity());
}
vec3f bullet_physics3d::get_angular_velocity(const mesh3d &mesh)
{
	return btV3(coll_shapes[mesh.tag].body->getAngularVelocity());
}
void bullet_physics3d::set_velocity(const mesh3d &mesh, const vec3f &lv)
{
	coll_shapes[mesh.tag].body->setLinearVelocity(btV3(lv));
}
void bullet_physics3d::set_angular_velocity(const mesh3d &mesh, const vec3f &lv)
{
	coll_shapes[mesh.tag].body->setAngularVelocity(btV3(lv));
}
float bullet_physics3d::get_mass(const mesh3d &mesh)
{
	return (float)coll_shapes[mesh.tag].body->getMass();
}