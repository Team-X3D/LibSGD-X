#pragma once

#include "../scene/entity.h"

namespace sgd {

SGD_SHARED(Material);
SGD_SHARED(PlaneBindings);

SGD_SHARED(PlaneEntity)

struct PlaneEntity : Entity {
	SGD_OBJECT_TYPE(PlaneEntity, Entity);

	PlaneEntity();

	explicit PlaneEntity(Material* material);

	PlaneBindings* bindings()const {
		return m_bindings;
	}

private:
	PlaneBindingsPtr m_bindings;

	explicit PlaneEntity(CPlaneEntity* that);

	PlaneEntity* onCopy() const override;

	void onShow() override;
	void onHide() override;
};

}
