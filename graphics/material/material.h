#pragma once

#include "../core/bindgroup.h"
#include "../render/renderpipeline.h"

namespace sgd {

SGD_SHARED(Material);

struct MaterialDescriptor {
	struct UniformDesc {
		size_t offset;
		size_t type;	// n floats for now, 0x104=color, 4=vec4f, 3=vec3f, 2=vec2f, 1=float
		void* defValue;
	};
	struct TextureDesc {
		uint32_t binding;
		const TextureType defType = TextureType::e2d;
		const TextureFormat defFormat = TextureFormat::any;
		const TextureFlags defFlags = TextureFlags::default_;
		const uint32_t defRGBAColor = 0xffffff;
	};
	String const typeName;
	const BindGroupDescriptor* bindGroupDescriptor;
	uint32_t const uniformsSize;
	Map<String, UniformDesc> const uniformDescs;
	Map<String, TextureDesc> const textureDescs;
	uint32_t const mainTexture;

	MaterialDescriptor(String typeName,							   //
					   const BindGroupDescriptor* bindGroupDescriptor, //
					   uint32_t uniformsSize,						   //
					   Map<String, UniformDesc> uniformDescs,			   //
					   Map<String, TextureDesc> textureDescs,
					   uint32_t mainTexture);

	static const MaterialDescriptor* forTypeName(CString typeName);
};
using CMaterialDescriptor = const MaterialDescriptor;

struct Material : GraphicsResource {
	SGD_OBJECT_TYPE(Material, GraphicsResource);

	explicit Material(const MaterialDescriptor* desc);

	Property<Path> path;

	Property<BlendMode> blendMode{BlendMode::opaque};

	Property<DepthFunc> depthFunc{DepthFunc::lessEqual};

	Property<CullMode> cullMode{CullMode::back};

	CMaterialDescriptor* descriptor() const {
		return m_desc;
	}

	bool hasNormalTexture() const {
		return m_hasNormalTexture;
	}

	void setTexture(CString name, CTexture* texture);

	void setColor(CString name, CVec4f color);

	void setVec4f(CString name, CVec4f value);
	void setVec3f(CString name, CVec3f value);
	void setVec2f(CString name, CVec2f value);

	void setFloat(CString name, float value);

	wgpu::BindGroup wgpuBindGroup() const {
		return m_bindGroup->wgpuBindGroup();
	}

	BindGroup* bindGroup() const {
		return m_bindGroup;
	}

protected:
	CMaterialDescriptor* m_desc{};
	BindGroupPtr m_bindGroup;
	BufferPtr m_uniformBuffer;
	bool m_hasNormalTexture{};
};

} // namespace sgd
