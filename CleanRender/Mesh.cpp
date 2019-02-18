#include "Mesh.h"

#include <string>
#include "GLUtils.h"
#include "Debug.h"



unsigned int Mesh::triangleCount = 0;


Mesh::Mesh(int vCount, int iCount) {
	vertexCount = vCount;
	indexCount = iCount;
	indices = new unsigned int[indexCount];
}

Mesh::~Mesh() {
	if (loadedToGL) {
		deleteFromGL();
	}
	if (cachedInLocal) {
		deleteLocal();
	}
}

void Mesh::setAttributesDefinition(int count, int* sizes) {
	GLenum* defTypes = new GLenum[count];
	for (int i = 0; i < count; i++) {
		defTypes[i] = GL_FLOAT;
	}
	setAttributesDefinition(count, sizes, defTypes);
}

void Mesh::setAttributesDefinition(int count, int* sizes, GLenum* types) {
	attributeCount = count;
	attributeSizes = sizes;
	attributeTypes = types;
	attributeByteOffsets = new unsigned int[count];
	vertexByteSize = 0;
	for (int i = 0; i < attributeCount; i++) {
		attributeByteOffsets[i] = vertexByteSize;
		vertexByteSize += attributeSizes[i] * glTypeSize(types[i]);
	}
	vertices = new char[vertexCount * vertexByteSize];
	cachedInLocal = true;
}

void Mesh::setAttribute(int index, signed char* values) {
	if (attributeTypes[index] != GL_BYTE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_BYTE!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, char* values) {
	if (attributeTypes[index] != GL_UNSIGNED_BYTE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_BYTE!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, short* values) {
	if (attributeTypes[index] != GL_SHORT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_SHORT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, unsigned short* values) {
	if (attributeTypes[index] != GL_UNSIGNED_SHORT || attributeTypes[index] != GL_HALF_FLOAT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_SHORT or GL_HALF_FLOAT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, int* values) {
	if (attributeTypes[index] != GL_INT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_INT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, unsigned int* values) {
	if (attributeTypes[index] != GL_UNSIGNED_INT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_INT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, float* values) {
	if (attributeTypes[index] != GL_FLOAT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_FLOAT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, double* values) {
	if (attributeTypes[index] != GL_DOUBLE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_DOUBLE!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttributeData(int index, char* bytes) {
	int attrByteOffset = attributeByteOffsets[index];
	int attrSize = attributeSizes[index];
	int typeSize = glTypeSize(attributeTypes[index]);
	for (int vi = 0; vi < vertexCount; vi++) {
		for (int i = 0; i < attrSize; i++) {
			for (int bi = 0; bi < typeSize; bi++) {
				((char*) vertices)[vi * vertexByteSize + attrByteOffset + i * typeSize + bi] = bytes[vi * attrSize * typeSize + i * typeSize + bi];
			}
		}
	}
}

void Mesh::setIndices(unsigned int* indices) {
	this->indices = indices;
}

void Mesh::deleteLocal() {
	if (attributeSizes != nullptr) {
		delete[] attributeSizes;
		attributeSizes = nullptr;
	}
	if (attributeTypes != nullptr) {
		delete[] attributeTypes;
		attributeTypes = nullptr;
	}
	if (attributeByteOffsets != nullptr) {
		delete[] attributeByteOffsets;
		attributeByteOffsets = nullptr;
	}
	if (vertices != nullptr) {
		delete[] vertices;
		vertices = nullptr;
	}
	if (indices != nullptr) {
		delete[] indices;
		indices = nullptr;
	}
	cachedInLocal = false;
}

void Mesh::uploadToGL() {
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);
	for (int i = 0; i < attributeCount; i++) {
		glEnableVertexAttribArray(i);
	}

	if (vboVertices == 0) {
		glGenBuffers(1, &vboVertices);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	unsigned int vboVerticesSize = vertexCount * vertexByteSize;
	glBufferData(GL_ARRAY_BUFFER, vboVerticesSize, vertices, GL_STATIC_DRAW);
	for (int i = 0; i < attributeCount; i++) {
		if (glTypeIsInteger(attributeTypes[i])) {
			glVertexAttribIPointer(i, attributeSizes[i], attributeTypes[i], vertexByteSize, (void*) attributeByteOffsets[i]);
		} else {
			glVertexAttribPointer(i, attributeSizes[i], attributeTypes[i], GL_FALSE, vertexByteSize, (void*) attributeByteOffsets[i]);
		}
	}

	if (vboIndices == 0) {
		glGenBuffers(1, &vboIndices);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	unsigned int vboIndicesSize = indexCount * sizeof(unsigned int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vboIndicesSize, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Not unbinding the element array since it is bound to the vao
	loadedToGL = true;
}

void Mesh::deleteFromGL() {
	if (vboIndices != 0) {
		glDeleteBuffers(1, &vboIndices);
		vboIndices = 0;
	}
	if (vboVertices != 0) {
		glDeleteBuffers(1, &vboVertices);
		vboVertices = 0;
	}
	if (vao != 0) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
}

void Mesh::render() const {
	if (!loadedToGL) return;

	triangleCount += indexCount / 3;

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}
