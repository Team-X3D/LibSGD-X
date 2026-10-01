#include "terrainbindings.h"

#include "../core/textureutil.h"
#include "../material/emissivematerial.h"
#include "../render/renderqueue.h"

namespace sgd {

namespace {

uint32_t rndColor(uint32_t lod) {
	auto r = (uint32_t)rnd(128) + 128;
	uint32_t color;
	switch (lod) {
	case 0:
		color = r;
		break;
	case 1:
		color = r << 8;
		break;
	case 2:
		color = r << 16;
		break;
	case 3:
		color = r << 8 | r;
		break;
	case 4:
		color = r << 16 | r;
		break;
	case 5:
		color = r << 16 | r << 8;
		break;
	default:
		color = r << 16 | r << 8 | r;
		break;
	}
	return color | 0xff000000u;
}

} // namespace

TerrainBindings::TerrainBindings()
	: m_bindGroup(new BindGroup(&terrainBindGroupDescriptor)), //
	  m_uniformBuffer(new Buffer(BufferType::uniform, nullptr, sizeof(TerrainUniforms))) {

	m_bindGroup->setBuffer(0, m_uniformBuffer);

	worldMatrix.changed.connect(nullptr, [=](CAffineMat4f m) {
		lockUniforms().worldMatrix = m;
		unlockUniforms();
	});
	worldMatrix.update();

	size.changed.connect(nullptr, [=](uint32_t n) {
		lockUniforms().size = (float)n;
		unlockUniforms();
		invalidate();
	});
	size.update();

	lods.changed.connect(nullptr, [=](uint32_t n) {
		if (n < 1 || n > 6) SGD_ERROR("Terrain LODs must be in the range 1..6 inclusive");
		lockUniforms().lods = n;
		unlockUniforms();
		invalidate();
	});
	lods.update();

	materialSize.changed.connect(nullptr, [=](float n) {
		lockUniforms().materialTexelSize = 1.0f / (float)n;
		unlockUniforms();
		invalidate();
	});
	materialSize.update();

	heightTexture.changed.connect(nullptr, [=](CTexture* ttexture) { //
		auto size = ttexture->size().x;
		if (size != ttexture->size().y) SGD_ERROR("Terrain height texture must be square");
		if (size & (size - 1)) SGD_ERROR("Terrain height texture size must be a power of 2");
		//		if(size<512) SGD_ERROR("Terrain height texture size must be at least 512");
		lockUniforms().heightTexelSize = 1.0f / (float)size;
		unlockUniforms();
		m_bindGroup->setTexture(1, ttexture);
	});
	heightTexture = blackTexture();

	normalTexture.changed.connect(nullptr, [=](CTexture* ttexture) { //
		m_bindGroup->setTexture(3, ttexture);
	});
	normalTexture = flatTexture();

	debugMode.changed.connect(nullptr, [=](int mode) {
		lockUniforms().debugMode = mode;
		unlockUniforms();
	});
	debugMode.changed.emit(debugMode());

	material = new Material(&emissiveMaterialDescriptor);
}

CTerrainUniforms& TerrainBindings::uniforms() const {
	return *(CTerrainUniforms*)m_uniformBuffer->data();
}

TerrainUniforms& TerrainBindings::lockUniforms() const {
	return *(TerrainUniforms*)m_uniformBuffer->lock();
}

void TerrainBindings::unlockUniforms() const {
	m_uniformBuffer->unlock();
}

void TerrainBindings::onValidate() const {

	using Vertex = TerrainVertex;

	Vector<Vertex> vertices;
	Vector<uint32_t> indices;

	vertices.reserve(1024 * 1024 * 16);
	indices.reserve(1024 * 1024 * 16);

	auto quadsPerTile = (size() >> (lods() - 1)) / 4;

	lockUniforms().quadsPerTile = (float)quadsPerTile;
	unlockUniforms();

	for (uint32_t lod = 0; lod < lods(); ++lod) {

		auto scale = float(1 << lod);
		auto tileSize = float(quadsPerTile * 4) * scale;

		for (uint32_t v = 0; v < 4; ++v) {

			for (uint32_t h = 0; h < 4; ++h) {

				// Leave 'hole' in center of 4X4 tiles.
				if (lod && h == 1 && (v == 1 || v == 2)) h = 3;

				for (uint32_t j = 0; j < quadsPerTile; j += 2) {

					for (uint32_t i = 0; i < quadsPerTile; i += 2) {

						float x = float(h * quadsPerTile + i) * scale - tileSize / 2;
						float y = float(v * quadsPerTile + j) * scale - tileSize / 2;

						static constexpr uint32_t tris[]{
							// clang-format off
//							0,3,1,  3,4,1,  1,4,5,  1,5,2,
//							3,6,7,  3,7,4,  4,7,5,  7,8,5,
							0,9,1,  1,9,4,  4,9,3,  3,9,0,
							1,10,2, 2,10,5, 5,10,4, 4,10,1,
							3,11,4, 4,11,7, 7,11,6, 6,11,3,
							4,12,5, 5,12,8, 8,12,7, 7,12,4
							// clang-format on
						};
						uint32_t v0 = vertices.size();
						indices.reserve(indices.size() + std::size(tris));
						for (uint32_t k : tris) {
							indices.push_back(k + v0);
						}

						static constexpr Vec2f verts[]{
							// clang-format off
							Vec2f(0, 0), Vec2f(1, 0), Vec2f(2, 0),
							Vec2f(0, 1), Vec2f(1, 1), Vec2f(2, 1),
							Vec2f(0, 2), Vec2f(1, 2), Vec2f(2, 2),
							Vec2f(.5f, .5f), Vec2f(1.5f, .5f),
							Vec2f(.5f, 1.5f), Vec2f(1.5f, 1.5f)
							// clang-format on
						};
						auto color = rndColor(lod);
						for (CVec2f tv : verts) {
							vertices.emplace_back(Vec3f(x + tv.x * scale, (float)lod, -(y + tv.y * scale)), color);
						}
					}
				}
			}
		}
	}

	log() << "### Terrain mesh, quadsPerTile:" << quadsPerTile << "vertices:" << vertices.size()
		  << "triangles:" << indices.size() / 3;

	m_vertexBuffer = new Buffer(BufferType::vertex, vertices.data(), sizeof(Vertex) * vertices.size());

	m_indexBuffer = new Buffer(BufferType::index, indices.data(), sizeof(uint32_t) * indices.size());

	m_indexCount = indices.size();
}

Array<float, 4> TerrainBindings::getLocalQuadVertexHeights(float x, float z) const {

	auto fx = std::floor(x);
	auto fz = std::floor(z);

	float hsize = (float)size() / 2;
	if (fx < -hsize || fx >= hsize - 1 || fz < -hsize || fz >= hsize - 1) {
		return {}; // Off the map
	}
	auto ix = (uint32_t)(fx + hsize);
	auto iz = (uint32_t)(fz + hsize);
	auto data = heightTexture()->data();

	switch (data->format()) {
	case TextureFormat::r32f: {
		auto p0 = (float*)(data->data() + iz * data->pitch());
		auto p1 = (float*)(data->data() + (iz + 1u) * data->pitch());
		return {p0[ix], p0[ix + 1], p1[ix], p1[ix + 1]};
	}
	case TextureFormat::r16f: {
		auto p0 = (float16*)(data->data() + iz * data->pitch());
		auto p1 = (float16*)(data->data() + (iz + 1u) * data->pitch());
		return {(float)p0[ix], (float)p0[ix + 1], (float)p1[ix], (float)p1[ix + 1]};
	}
	default:
		SGD_ERROR("TODO");
	}
}

float TerrainBindings::getLocalHeight(float x, float z) const {

	auto ys = getLocalQuadVertexHeights(x,z);

	auto tx = x - std::floor(x);
	auto tz = z - std::floor(z);

	float y4 = (ys[1] - ys[0]) * tx + ys[0];
	float y5 = (ys[3] - ys[2]) * tx + ys[2];

	float y = (y5 - y4) * tz + y4;

	return y;
}

void TerrainBindings::render(RenderQueue* rq) const {
	rq->addRenderOp(m_vertexBuffer, m_indexBuffer,									  //
					material(), m_bindGroup, emptyBindGroup(BindGroupType::renderer), //
					m_indexCount, 1, 0, 0, shadowsEnabled());
}

} // namespace sgd
