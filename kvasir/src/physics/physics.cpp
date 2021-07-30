#include "physics.h"

using namespace kvasir;

physics3d::~physics3d() {}
physics3d::phys_props::phys_props(float ms, float fr, float rest)
{
	mass = ms;
	friction = fr;
	restitution = rest;
}
physics3d::phys_props physics3d::static_props(float friction, float restitution)
{
	return phys_props(0, friction, restitution);
}
physics3d::phys_props physics3d::dynamic_props(float mass, float friction, float restitution)
{
	return phys_props(mass, friction, restitution);
}