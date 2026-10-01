
// Linux (bugTestSimple.cpp, keycodes.h, sgd.h, libsgd_dynamic.so, checkbox.png) :
// g++ -std=c++20 -O3 -Wall -Werror -I. -L. -Wl,-rpath,. -DSGD_DYNAMIC=1 -o bugTestSimple bugTestSimple.cpp -lsgd_dynamic
// ./bugTestSimple

// Mac (bugTestSimple.cpp, keycodes.h, sgd.h, libsgd_dynamic.dylib, checkbox.png) :
// clang -std=c++20 -O3 -Wall -Werror -I. -L. -Wl,-rpath,. -DSGD_DYNAMIC=1 -o bugTestSimple bugTestSimple.cpp -lsgd_dynamic
// ./bugTestSimple

#include "sgd/sgd.h"

// --------------------
// --- MAIN
// --------------------

int main(int argc, char *argv[]) {

	sgd_Init();

	sgd_CreateWindow(1280, 720, "Bug Test", SGD_WINDOW_FLAGS_CENTERED);

	SGD_Texture env {sgd_LoadCubeTexture("sgd://envmaps/sunnysky-cube.png", SGD_TEXTURE_FORMAT_ANY, SGD_TEXTURE_FLAGS_DEFAULT)};
	sgd_SetEnvTexture(env);

	SGD_Skybox skybox {sgd_CreateSkybox(env)};
	sgd_SetSkyboxRoughness(skybox, 0.3f);

	SGD_Camera camera {sgd_CreatePerspectiveCamera()};
	sgd_SetEntityPosition(camera, 0.0f, 0.0f, 0.0f);


	// --- Will crash on MacOS, but not every time. Works fine on Linux.
	SGD_Image checkboxImage {sgd_LoadArrayImage("~/Desktop/bugtest/checkbox.png", 4, 2, 2, 0)};
	// ---------------------------------------------


	sgd_SetImageRect(checkboxImage, 0.0f, 0.0f, 1.0f, 1.0f);


	bool finished {false};

	while (!finished) {

		if ((sgd_PollEvents() & SGD_EVENT_MASK_CLOSE_CLICKED) != 0) { finished = true; }
		if (sgd_IsKeyHit(SGD_KEY_ESCAPE)) { finished = true; }

		sgd_Clear2D();

		sgd_Draw2DImage(checkboxImage, 0, 0, 0);

		sgd_RenderScene();
		sgd_Present();

	}

	sgd_Terminate();

}
