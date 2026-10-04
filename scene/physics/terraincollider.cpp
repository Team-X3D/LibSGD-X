#include "terraincollider.h"

#include "intersect.h"

namespace sgd {

TerrainCollider::TerrainCollider(Entity* entity, uint32_t colliderType, TerrainBindings* bindings)
	: Collider(colliderType), m_bindings(bindings) {

	auto size = (float)m_bindings->size();

	auto bounds = Boxf(-size * .5f, size * .5f);
	bounds.min *= entity->worldScale();
	bounds.max *= entity->worldScale();

	setLocalBounds(bounds);

	attach(entity);
}

Vector<TerrainCollider::Triangle> TerrainCollider::buildLocalTriangles(CBoxr lbounds) const {

	auto x0 = std::floor(lbounds.min.x);
	auto z0 = std::floor(lbounds.min.z);
	auto x1 = std::floor(lbounds.max.x + 1);
	auto z1 = std::floor(lbounds.max.z + 1);

	Vector<Triangle> tris;

	for (auto z = z0; z < z1; ++z) {
		for (auto x = x0; x < x1; ++x) {
			auto heights = m_bindings->getLocalQuadVertexHeights(x, z);
			auto yc = (heights[0] + heights[1] + heights[2] + heights[3]) * .25f;
			Vec3r cv = Vec3f(x + .5f, yc, z + .5f);
			//
			tris.emplace_back(Vec3f(x, heights[0], z), cv, Vec3f(x + 1, heights[1], z));		 // 0,cv,1
			tris.emplace_back(Vec3f(x + 1, heights[1], z), cv, Vec3f(x + 1, heights[3], z + 1)); // 1,cv,3
			tris.emplace_back(Vec3f(x + 1, heights[3], z + 1), cv, Vec3f(x, heights[2], z + 1)); // 3,cv,2
			tris.emplace_back(Vec3f(x, heights[2], z + 1), cv, Vec3f(x, heights[0], z));		 // 2,cv,0
		}
	}
	return tris;
}

Collider* TerrainCollider::intersectRay(CLiner ray, float radius, Contact& contact) {

	Boxr bounds(ray.o);
	bounds |= ray * contact.time;
	bounds.min -= radius + boundsPadding;
	bounds.max += radius + boundsPadding;

	auto lbounds = inverse(entity()->worldMatrix()) * bounds;
	// SGD_LOG << "### lbounds"<<lbounds;

	auto tris = buildLocalTriangles(lbounds);
	// SGD_LOG << "###"<<tris.size();

	bool collision = false;

	for (auto& tri : tris) {
		Boxr triBounds(tri.v0);
		triBounds |= tri.v1;
		triBounds |= tri.v2;
		if (!intersects(lbounds, triBounds)) continue;

		auto v0 = entity()->worldMatrix() * tri.v0;
		auto v1 = entity()->worldMatrix() * tri.v1;
		auto v2 = entity()->worldMatrix() * tri.v2;

		collision |= intersectRayTriangle(ray, radius, v0, v1, v2, contact);
	}
	if (!collision) return nullptr;

	return this;
}

Collider* TerrainCollider::intersectRay(CLiner ray, CVec3f radii, Contact& contact) {
	Vec3r rradii(radii);
	Vec3r invRadii = (real)1 / rradii;

	Liner invRay(ray.o * invRadii, ray.d * contact.time * invRadii);

	Contact invContact = contact;
	invContact.time = length(invRay.d);
	invRay.d = normalize(invRay.d);

	Boxr bounds(ray.o);
	bounds |= ray * contact.time;
	bounds.min -= rradii + boundsPadding;
	bounds.max += rradii + boundsPadding;

	auto lbounds = inverse(entity()->worldMatrix()) * bounds;

	auto tris = buildLocalTriangles(lbounds);

	bool collision = false;

	for (auto& tri : tris) {
		Boxr triBounds(tri.v0);
		triBounds |= tri.v1;
		triBounds |= tri.v2;
		if (!intersects(lbounds, triBounds)) continue;

		auto v0 = entity()->worldMatrix() * tri.v0 * invRadii;
		auto v1 = entity()->worldMatrix() * tri.v1 * invRadii;
		auto v2 = entity()->worldMatrix() * tri.v2 * invRadii;

		collision |= intersectRayTriangle(invRay, 1, v0, v1, v2, invContact);
	}
	if (!collision) return nullptr;

	contact.point = invContact.point * rradii;
	contact.normal = normalize(invContact.normal * invRadii);
	contact.time = length(invRay.d * invContact.time * rradii);

	return this;
}

void TerrainCollider::onUpdate(const CollisionSpace* space, uint32_t colliderMask, Vector<Collision>& collisions) {
}

} // namespace sgd
