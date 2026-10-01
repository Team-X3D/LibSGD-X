#pragma once

#include "collider.h"

#include <graphics/exports.h>

namespace sgd {

SGD_SHARED(TerrainBindings);

SGD_SHARED(TerrainCollider);

struct TerrainCollider : Collider {
	SGD_OBJECT_TYPE(TerrainCollider, Collider);

	static constexpr real boundsPadding = (real).01;

	TerrainCollider(Entity* entity, uint32_t colliderType, TerrainBindings* bindings);

	Collider* intersectRay(CLiner ray, float rradius, Contact& contact) override;

	Collider* intersectRay(CLiner ray, CVec3f radii, Contact& contact) override;

private:
	struct Triangle {
		Vec3r v0;
		Vec3r v1;
		Vec3r v2;
		Triangle() = default;
		Triangle(Vec3r v0, Vec3r v1,Vec3r v2):v0(v0),v1(v1),v2(v2){}
	};

	TerrainBindingsPtr m_bindings;

	void onUpdate(const CollisionSpace* space, uint32_t colliderMask, Vector<Collision>& collisions) override;
};

}
