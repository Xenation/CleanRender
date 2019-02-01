#include "Mesh.h"



Mesh::Mesh(int vCount, int iCount) {
	vertexCount = vCount;
	indexCount = iCount;
	indices = new int[indexCount];
}

Mesh::~Mesh() {
	if (attributeSizes != nullptr) {
		delete[] attributeSizes;
	}
	if (attributeOffsets != nullptr) {
		delete[] attributeOffsets;
	}
	if (vertices != nullptr) {
		delete[] vertices;
	}
	delete[] indices;
}

void Mesh::setAttributesDefinition(int count, int* sizes) {
	attributeCount = count;
	attributeSizes = sizes;
	attributeOffsets = new int[count];
	vertexFloatSize = 0;
	for (int i = 0; i < attributeCount; i++) {
		attributeOffsets[i] = vertexFloatSize * sizeof(float);
		vertexFloatSize += attributeSizes[i];
	}
	vertices = new float[vertexCount * vertexFloatSize];
}

void Mesh::setAttribute(int index, float* values) {
	int attrOffset = attributeOffsets[index];
	int attrSize = attributeSizes[index];
	for (int vi = 0; vi < vertexCount; vi++) {
		for (int i = 0; i < attrSize; i++) {
			vertices[vi * vertexFloatSize + attrOffset] = values[vi * attrSize + i];
		}
	}
}

void Mesh::setIndices(int* indices) {
	this->indices = indices;
}

void Mesh::uploadToGL() {
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);
	if (vboVertices == 0) {
		glGenBuffers(1, &vboVertices);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);
	if (vboIndices == 0) {
		glGenBuffers(1, &vboIndices);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(float), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::deleteFromGL() {
	if (vboIndices != 0) {
		glDeleteBuffers(1, &vboIndices);
	}
	if (vboVertices != 0) {
		glDeleteBuffers(1, &vboVertices);
	}
	if (vao != 0) {
		glDeleteVertexArrays(1, &vao);
	}
}

void Mesh::render() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);

	for (int i = 0; i < attributeCount; i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attributeSizes[i], GL_FLOAT, GL_FALSE, vertexFloatSize * sizeof(float), (void*) (vertices + attributeOffsets[i]));
	}

	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
