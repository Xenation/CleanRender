#include "Material.h"

#include "ShaderProgram.h"
#include "SpecializedShaderProgram.h"
#include "UniformBuffer.h"
#include "RenderPass.h"



Material* Material::errorMaterial = nullptr;


Material::Material(ShaderProgram* shaderProgram, std::string renderPassName) : Material(shaderProgram->getSpecializedProgram(renderPassName)) {}

Material::Material(ShaderProgram* shaderProgram, RenderPass* renderPass) : Material(shaderProgram->getSpecializedProgram(renderPass)) {}

Material::Material(SpecializedShaderProgram* specializedProgram)
	: specializedProgram(specializedProgram), uniformBuffer(new UniformBuffer()), renderers(4, 16) {
	ShaderUniformLayoutFieldInfo* uniformField = (ShaderUniformLayoutFieldInfo*) specializedProgram->parentShader->getMaterialFieldInfo();
	uniformBuffer->setLayouts(1, new UniformLayout[1]{UniformLayout(10, uniformField->subFieldCount, uniformField->getMembersTypes())});
	uniformBuffer->uploadToGL();
	uniformLayout = &uniformBuffer->getLayout(0);
	idInProgram = specializedProgram->materials.add(this);
}

Material::~Material() {
	if (uniformBuffer != nullptr) {
		delete uniformBuffer;
	}
	specializedProgram->materials.remove(idInProgram);
}


void Material::setField(unsigned int index, bool value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, int value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, unsigned int value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, float value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, double value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec2f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec3f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec4f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Color value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value.vec);
}
void Material::setField(unsigned int index, Matrix4x4f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}

void Material::updateFields() {
	fieldsExpired = false;
	uniformBuffer->updateLayout(0);
}

void Material::use() {
	if (fieldsExpired) {
		updateFields();
	}
	uniformBuffer->bindLayout(0);
}
