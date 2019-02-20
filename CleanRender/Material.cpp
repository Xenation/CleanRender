#include "Material.h"

#include "ShaderProgram.h"
#include "UniformBuffer.h"
#include "RenderPass.h"



Material* Material::errorMaterial = nullptr;


Material::Material(ShaderProgram* shaderProgram)
	: shaderProgram(shaderProgram), uniformBuffer(new UniformBuffer()), renderers(4, 16) {
	uniformBuffer->setLayouts(1, new UniformLayout[1]{UniformLayout(10, shaderProgram->info->materialFieldCount, shaderProgram->info->materialFieldTypes)});
	uniformBuffer->uploadToGL();
	uniformLayout = &uniformBuffer->getLayout(0);
	idInProgram = shaderProgram->materials.add(this);
}

Material::~Material() {
	if (uniformBuffer != nullptr) {
		delete uniformBuffer;
	}
	shaderProgram->materials.remove(idInProgram);
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
