#include "planeentity.h"

#include "planerenderer.h"
#include "../scene/scenerenderer.h"

namespace sgd {

PlaneEntity::PlaneEntity() :m_bindings(new PlaneBindings()){
}

PlaneEntity::PlaneEntity(Material* material) :m_bindings(new PlaneBindings()){
	m_bindings->material = material;
}

PlaneEntity::PlaneEntity(CPlaneEntity* that) :m_bindings(that->m_bindings){
}

PlaneEntity* PlaneEntity::onCopy() const {
	return new PlaneEntity(this);
}

void PlaneEntity::onShow() {
	scene()->sceneRenderer()->planeRenderer()->add(this);
}

void PlaneEntity::onHide() {
	scene()->sceneRenderer()->planeRenderer()->remove(this);
}

}
