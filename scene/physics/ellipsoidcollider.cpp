#include "ellipsoidcollider.h"

#include "collisionspace.h"
#include "collisionutil.h"
#include "intersect.h"

namespace sgd {

EllipsoidCollider::EllipsoidCollider(Entity* entity, uint32_t colliderType, float rradius, float hheight)
	: Collider(colliderType), radius(rradius), height(hheight), m_src(entity->worldPosition()) {

	auto updateBounds = [=] { setLocalBounds({{-radius(), -height() / 2, -radius()}, {radius(), height() / 2, radius()}}); };

	this->radius.changed.connect(nullptr, [=](float) { updateBounds(); });

	this->height.changed.connect(nullptr, [=](float) { updateBounds(); });

	updateBounds();

	attach(entity);
}

Collider* EllipsoidCollider::intersectRay(CLiner ray, float rradius, Contact& contact) {
	SGD_ERROR("TODO");
}

Collider* EllipsoidCollider::intersectRay(CLiner ray, CVec3f radii, Contact& contact) {
	auto rradii = Vec3r(radii) + Vec3r(this->radii());
	auto invRadii = (real)1 / rradii;

	Liner invRay(ray.o * invRadii, ray.d * contact.time * invRadii);

	Contact invContact = contact;
	invContact.time = length(invRay.d);
	invRay.d = normalize(invRay.d);

	if (!intersectRaySphere(invRay, entity()->worldPosition() * invRadii, 1, invContact)) return nullptr;

	contact.point = invContact.point * rradii;
	contact.normal = normalize(cofactor(Mat3<real>::scale(rradii)) * invContact.normal);
	contact.time = length(invRay.d * invContact.time * rradii);

	return this;
}

void EllipsoidCollider::onUpdate(const CollisionSpace* space, uint32_t colliderMask, Vector<Collision>& collisions) {

	auto dst = collideRay(space, m_src, entity()->worldPosition(), radii(), colliderMask, colliderType(), collisions);

	entity()->setWorldPosition(dst);

	m_src = dst;

	for (auto& c : collisions) c.contact.point -= c.contact.normal * Vec3r(radii());
}

void EllipsoidCollider::onReset() {

	m_src = entity()->worldPosition();
}

} // namespace sgd
