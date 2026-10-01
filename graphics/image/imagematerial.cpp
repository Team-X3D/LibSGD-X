#include "imagematerial.h"

#include "../core/textureutil.h"

namespace sgd {

namespace {

struct alignas(16) ImageMaterialUniforms {
	Vec4f albedoColor;
};

auto shaderSource{
#include "imagematerial.wgsl"
};

BindGroupDescriptor bindGroupDescriptor( //
	"imageMaterial",					 //
	BindGroupType::material,			 //
	{
		bufferBindGroupLayoutEntry(0, wgpu::ShaderStage::Fragment, wgpu::BufferBindingType::Uniform),
		textureBindGroupLayoutEntry(1, wgpu::ShaderStage::Fragment, wgpu::TextureViewDimension::e2D),
		samplerBindGroupLayoutEntry(2, wgpu::ShaderStage::Fragment),
	}, //
	shaderSource);

BindGroupDescriptor bindGroupDescriptorMX( //
	"imageMaterialMX",					   //
	BindGroupType::material,			   //
	{
		bufferBindGroupLayoutEntry(0, wgpu::ShaderStage::Fragment, wgpu::BufferBindingType::Uniform),
		textureBindGroupLayoutEntry(1, wgpu::ShaderStage::Fragment, wgpu::TextureViewDimension::e2DArray),
		samplerBindGroupLayoutEntry(2, wgpu::ShaderStage::Fragment),
	}, //
	String("#define IMAGE_MATERIAL_MX 1\n") + shaderSource);

} // namespace

const MaterialDescriptor imageMaterialDescriptor( //
	"image",									  //
	&bindGroupDescriptor,						  //
	sizeof(ImageMaterialUniforms),				  //
	{
		{"albedo", {offsetof(ImageMaterialUniforms, albedoColor), 0x104, new Vec4f(1)}},
	},
	{
		{"albedo", {1, TextureType::e2d, TextureFormat::any, TextureFlags::image}},

	},
	1); //

const MaterialDescriptor imageMaterialDescriptorMX( //
	"image",										//
	&bindGroupDescriptorMX,							//
	sizeof(ImageMaterialUniforms),					//
	{
		{"albedo", {offsetof(ImageMaterialUniforms, albedoColor), 0x104, new Vec4f(1)}},
	},
	{
		{"albedo", {1, TextureType::array, TextureFormat::any, TextureFlags::image}},

	},
	1); //

} // namespace sgd
