#include "start.cpp"

/*
void entry() {
	for (;;) {
		pollEvents();

		render();
	}
}

int main() {
	start(entry);
}
*/

void entry() {

	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/Bricks076C_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/Fabric048_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/Fabric050_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/Gravel023_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/Ground037_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/PaintedWood009C_1K-JPG")).result();
	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/PavingStones065_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/PavingStones119_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/PavingStones131_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/Tiles019_1K-JPG")).result();
	//	MaterialPtr material = loadMaterial(Path("~/Desktop/materials/Tiles093_1K-JPG")).result();

	material->setVec2f("textureScale", {.5f, .5f});

	PlaneEntityPtr planeEntity = new PlaneEntity(material);
	scene->add(planeEntity);

	createPlayer(nullptr);

	move(player, {0, 10, -100});

	for (;;) {

		playerFly(.25f);

		pollEvents();

		render();
	}
}

int main() {
	start(entry);
}
