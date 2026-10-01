#include "material.h"

#include "../core/textureutil.h"

namespace sgd {

namespace {

Map<String, CMaterialDescriptor*>* g_materialDescs;

Map<String, CMaterialDescriptor*>& materialDescs() {
	if (!g_materialDescs) g_materialDescs = new Map<String, CMaterialDescriptor*>();
	return *g_materialDescs;
}

} // namespace

// ***** MaterialDescriptor *****

MaterialDescriptor::MaterialDescriptor(String typeName,								   //
									   const BindGroupDescriptor* bindGroupDescriptor, //
									   uint32_t uniformsSize,						   //
									   Map<String, UniformDesc> uniformDescs,		   //
									   Map<String, TextureDesc> textureDescs,
									   uint32_t mainTexture)
	: typeName(std::move(typeName)),			//
	  bindGroupDescriptor(bindGroupDescriptor), //
	  uniformsSize(uniformsSize),				//
	  uniformDescs(std::move(uniformDescs)),	//
	  textureDescs(std::move(textureDescs)), mainTexture(mainTexture) {
	materialDescs().insert(std::make_pair(this->typeName, this));
}

CMaterialDescriptor* MaterialDescriptor::forTypeName(CString typeName) {
	auto it = materialDescs().find(typeName);
	return it != materialDescs().end() ? it->second : nullptr;
}

// ***** Material *****

Material::Material(CMaterialDescriptor* desc) : m_desc(desc),
	m_bindGroup(new BindGroup(m_desc->bindGroupDescriptor)) {

	auto uniforms = (uint8_t*)std::malloc(m_desc->uniformsSize);
	for (auto& kv : m_desc->uniformDescs) {
		std::memcpy(uniforms + kv.second.offset, kv.second.defValue, (kv.second.type & 0xff) * 4);
	}
	m_uniformBuffer = new Buffer(BufferType::uniform, uniforms, m_desc->uniformsSize);
	std::free(uniforms);

	m_bindGroup->setBuffer(0, m_uniformBuffer);

	for (auto& [id, tdesc] : m_desc->textureDescs) {
		m_bindGroup->setTexture(tdesc.binding, rgbaTexture(tdesc.defRGBAColor, tdesc.defType));
	}
	blendMode.changed.connect(this, [=](BlendMode) { //
		invalidate(true);
	});
	depthFunc.changed.connect(this, [=](DepthFunc) { //
		invalidate(true);
	});
	cullMode.changed.connect(this, [=](CullMode) { //
		invalidate(true);
	});
	addDependency(m_bindGroup);
}

void Material::setTexture(CString name, CTexture* texture) {

	auto it = m_desc->textureDescs.find(name);
	if (it == m_desc->textureDescs.end()) SGD_ERROR("Material texture paramter \"" + name + "\" not found");

	if (name == "normal") m_hasNormalTexture = bool(texture);
	if (!texture) texture = rgbaTexture(it->second.defRGBAColor, it->second.defType);

	m_bindGroup->setTexture(it->second.binding, texture);
}

void Material::setColor(CString name, CVec4f color) {
	auto it = m_desc->uniformDescs.find(name);
	if (it == m_desc->uniformDescs.end()) SGD_ERROR("Material color parameter \"" + name + "\" not found");
	if (it->second.type != 0x104) SGD_ERROR("Material parameter \"" + name + "\" is not of type 'color'");

	auto lcolor = toLinearColor(color);

	m_uniformBuffer->update(&lcolor, it->second.offset, sizeof(lcolor));
}

void Material::setVec4f(CString name, CVec4f value) {
	auto it = m_desc->uniformDescs.find(name);
	if (it == m_desc->uniformDescs.end()) SGD_ERROR("Material color parameter \"" + name + "\" not found");
	if (it->second.type != 4) SGD_ERROR("Material parameter \"" + name + "\" is not of type 'vec4f");

	m_uniformBuffer->update(&value, it->second.offset, sizeof(value));
}

void Material::setVec3f(CString name, CVec3f value) {
	auto it = m_desc->uniformDescs.find(name);
	if (it == m_desc->uniformDescs.end()) SGD_ERROR("Material color parameter \"" + name + "\" not found");
	if (it->second.type != 3) SGD_ERROR("Material parameter \"" + name + "\" is not of type 'vec4f");
	m_uniformBuffer->update(&value, it->second.offset, sizeof(value));
}

void Material::setVec2f(CString name, CVec2f value) {
	auto it = m_desc->uniformDescs.find(name);
	if (it == m_desc->uniformDescs.end()) SGD_ERROR("Material color parameter \"" + name + "\" not found");
	if (it->second.type != 2) SGD_ERROR("Material parameter \"" + name + "\" is not of type 'vec4f");

	m_uniformBuffer->update(&value, it->second.offset, sizeof(value));
}

void Material::setFloat(CString name, float value) {
	auto it = m_desc->uniformDescs.find(name);
	if (it == m_desc->uniformDescs.end()) SGD_ERROR("Material float parameter \"" + name + "\" not found");
	if (it->second.type != 1) SGD_ERROR("Material paramter \"" + name + "\" is not of type 'float'");

	m_uniformBuffer->update(&value, it->second.offset, sizeof(value));
}

} // namespace sgd
