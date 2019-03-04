#pragma once
#include <string>
#include "gl3w.h"

enum GLSLType : unsigned int {
	GLSL_UNKNOWN = 0,
	GLSL_IS_SCALAR	= 0b10000000'00000000'00000000'00000000,
	GLSL_IS_VEC2	= 0b01000000'00000000'00000000'00000000,
	GLSL_IS_VEC3	= 0b00100000'00000000'00000000'00000000,
	GLSL_IS_VEC4	= 0b00010000'00000000'00000000'00000000,
	GLSL_IS_BOOL	= 0b00000000'00000000'00000000'00000001,
	GLSL_IS_INT		= 0b00000000'00000000'00000000'00000010,
	GLSL_IS_UINT	= 0b00000000'00000000'00000000'00000100,
	GLSL_IS_FLOAT	= 0b00000000'00000000'00000000'00001000,
	GLSL_IS_DOUBLE	= 0b00000000'00000000'00000000'00010000,
	GLSL_IS_MAT2x2	= 0b00000000'00000000'00000001'00000000,
	GLSL_IS_MAT2x3	= 0b00000000'00000000'00000010'00000000,
	GLSL_IS_MAT2x4	= 0b00000000'00000000'00000100'00000000,
	GLSL_IS_MAT3x2	= 0b00000000'00000000'00001000'00000000,
	GLSL_IS_MAT3x3	= 0b00000000'00000000'00010000'00000000,
	GLSL_IS_MAT3x4	= 0b00000000'00000000'00100000'00000000,
	GLSL_IS_MAT4x2	= 0b00000000'00000000'01000000'00000000,
	GLSL_IS_MAT4x3	= 0b00000000'00000000'10000000'00000000,
	GLSL_IS_MAT4x4	= 0b00000000'00000001'00000000'00000000,
	// Scalars
	GLSL_BOOL	= GLSL_IS_SCALAR | GLSL_IS_BOOL,
	GLSL_INT	= GLSL_IS_SCALAR | GLSL_IS_INT,
	GLSL_UINT	= GLSL_IS_SCALAR | GLSL_IS_UINT,
	GLSL_FLOAT	= GLSL_IS_SCALAR | GLSL_IS_FLOAT,
	GLSL_DOUBLE	= GLSL_IS_SCALAR | GLSL_IS_DOUBLE,
	// Vectors
	GLSL_BVEC2	= GLSL_IS_VEC2 | GLSL_IS_BOOL,
	GLSL_IVEC2	= GLSL_IS_VEC2 | GLSL_IS_INT,
	GLSL_UVEC2	= GLSL_IS_VEC2 | GLSL_IS_UINT,
	GLSL_VEC2	= GLSL_IS_VEC2 | GLSL_IS_FLOAT,
	GLSL_DVEC2	= GLSL_IS_VEC2 | GLSL_IS_DOUBLE,
	GLSL_BVEC3	= GLSL_IS_VEC3 | GLSL_IS_BOOL,
	GLSL_IVEC3	= GLSL_IS_VEC3 | GLSL_IS_INT,
	GLSL_UVEC3	= GLSL_IS_VEC3 | GLSL_IS_UINT,
	GLSL_VEC3	= GLSL_IS_VEC3 | GLSL_IS_FLOAT,
	GLSL_DVEC3	= GLSL_IS_VEC3 | GLSL_IS_DOUBLE,
	GLSL_BVEC4	= GLSL_IS_VEC4 | GLSL_IS_BOOL,
	GLSL_IVEC4	= GLSL_IS_VEC4 | GLSL_IS_INT,
	GLSL_UVEC4	= GLSL_IS_VEC4 | GLSL_IS_UINT,
	GLSL_VEC4	= GLSL_IS_VEC4 | GLSL_IS_FLOAT,
	GLSL_DVEC4	= GLSL_IS_VEC4 | GLSL_IS_DOUBLE,
	// Matrices
	GLSL_MAT2x2	= GLSL_IS_FLOAT | GLSL_IS_MAT2x2,
	GLSL_MAT2x3	= GLSL_IS_FLOAT | GLSL_IS_MAT2x3,
	GLSL_MAT2x4	= GLSL_IS_FLOAT | GLSL_IS_MAT2x4,
	GLSL_MAT3x2	= GLSL_IS_FLOAT | GLSL_IS_MAT3x2,
	GLSL_MAT3x3	= GLSL_IS_FLOAT | GLSL_IS_MAT3x3,
	GLSL_MAT3x4	= GLSL_IS_FLOAT | GLSL_IS_MAT3x4,
	GLSL_MAT4x2	= GLSL_IS_FLOAT | GLSL_IS_MAT4x2,
	GLSL_MAT4x3	= GLSL_IS_FLOAT | GLSL_IS_MAT4x3,
	GLSL_MAT4x4	= GLSL_IS_FLOAT | GLSL_IS_MAT4x4,
	GLSL_MAT2	= GLSL_MAT2x2,
	GLSL_MAT3	= GLSL_MAT3x3,
	GLSL_MAT4	= GLSL_MAT4x4,
	GLSL_DMAT2x2 = GLSL_IS_DOUBLE | GLSL_IS_MAT2x2,
	GLSL_DMAT2x3 = GLSL_IS_DOUBLE | GLSL_IS_MAT2x3,
	GLSL_DMAT2x4 = GLSL_IS_DOUBLE | GLSL_IS_MAT2x4,
	GLSL_DMAT3x2 = GLSL_IS_DOUBLE | GLSL_IS_MAT3x2,
	GLSL_DMAT3x3 = GLSL_IS_DOUBLE | GLSL_IS_MAT3x3,
	GLSL_DMAT3x4 = GLSL_IS_DOUBLE | GLSL_IS_MAT3x4,
	GLSL_DMAT4x2 = GLSL_IS_DOUBLE | GLSL_IS_MAT4x2,
	GLSL_DMAT4x3 = GLSL_IS_DOUBLE | GLSL_IS_MAT4x3,
	GLSL_DMAT4x4 = GLSL_IS_DOUBLE | GLSL_IS_MAT4x4,
	GLSL_DMAT2 = GLSL_DMAT2x2,
	GLSL_DMAT3 = GLSL_DMAT3x3,
	GLSL_DMAT4 = GLSL_DMAT4x4,
};

enum class UniformLayoutType {
	STD140,
	SHARED, // TODO Not yet supported
	PACKED, // TODO Not yet supported
};

inline unsigned int glTypeSize(GLenum type) {
	switch (type) {
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return 1;
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_HALF_FLOAT:
		return 2;
	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_FIXED:
	case GL_FLOAT:
		return 4;
	case GL_DOUBLE:
		return 8;
	}
	return 4;
}

inline bool glTypeIsInteger(GLenum type) {
	switch (type) {
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_INT:
	case GL_UNSIGNED_INT:
		return true;
	case GL_FIXED:
	case GL_HALF_FLOAT:
	case GL_FLOAT:
	case GL_DOUBLE:
		return false;
	}
	return false;
}

inline unsigned int glGetUniformBufferAlignment(unsigned int currentOffset) {
	static unsigned int uniformBufferAlignment = 0;
	if (uniformBufferAlignment == 0) {
		int align;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
		uniformBufferAlignment = (unsigned int) align;
	}
	unsigned int disalign = currentOffset % uniformBufferAlignment;
	if (disalign != 0) {
		return currentOffset + (uniformBufferAlignment - disalign);
	} else {
		return currentOffset;
	}
}

inline unsigned int glFormatBitSize(GLenum format) {
	switch (format) {
		// 128
	case GL_RGBA32I:
	case GL_RGBA32UI:
	case GL_RGBA32F:
		return 128;

		// 96
	case GL_RGB32I:
	case GL_RGB32UI:
	case GL_RGB32F:
		return 96;

		// 64
	case GL_RGBA16_SNORM:
	case GL_RGBA16I:
	case GL_RGBA16UI:
	case GL_RGBA16F:
	case GL_RGBA16:
		return 64;

		// 48
	case GL_RGB16_SNORM:
	case GL_RGB16I:
	case GL_RGB16UI:
	case GL_RGB16F:
	case GL_RGB16:
		return 48;

		// 40
	case GL_DEPTH32F_STENCIL8:
		return 40;

		// 32
	case GL_RGB9_E5:
	case GL_SRGB8_ALPHA8:
	case GL_R11F_G11F_B10F:
	case GL_RG16_SNORM:
	case GL_RG16:
	case GL_RGBA8_SNORM:
	case GL_RGBA8I:
	case GL_RGBA8UI:
	case GL_RGBA8:
	case GL_RGBA:
	case GL_DEPTH24_STENCIL8:
	case GL_DEPTH_STENCIL:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		return 32;

		// 24
	case GL_SRGB8:
	case GL_RGB8_SNORM:
	case GL_RGB8I:
	case GL_RGB8UI:
	case GL_RGB8:
	case GL_RGB:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT:
		return 24;

		// 16
	case GL_R16_SNORM:
	case GL_R16:
	case GL_RG8_SNORM:
	case GL_RG8:
	case GL_RG:
	case GL_DEPTH_COMPONENT16:
	case GL_STENCIL_INDEX16:
		return 16;

		// 12
	case GL_RGB10_A2UI:
	case GL_RGB10_A2:
		return 12;

		// 8
	case GL_R8_SNORM:
	case GL_R8:
	case GL_RED:
	case GL_STENCIL_INDEX8:
		return 8;

		// 4
	case GL_STENCIL_INDEX4:
		return 4;

		// 1
	case GL_STENCIL_INDEX1:
		return 1;
	}
}

inline unsigned int glFormatByteSize(GLenum format, unsigned int count) {
	unsigned int bitSize = glFormatBitSize(format) * count;
	unsigned int byteSize = bitSize / 8;
	if (bitSize % 8 != 0) {
		byteSize++;
	}
	return byteSize;
}

inline unsigned int glslTypeBaseAlignment(GLSLType type);
inline unsigned int glslTypeBaseAlignment(GLSLType type, unsigned int arrSize) {
	const unsigned int vec4Align = glslTypeBaseAlignment(GLSL_VEC4);
	if (type & (GLSL_IS_MAT2x2 | GLSL_IS_MAT2x3 | GLSL_IS_MAT2x4 | GLSL_IS_MAT3x2 | GLSL_IS_MAT3x3 | GLSL_IS_MAT3x4 | GLSL_IS_MAT4x2 | GLSL_IS_MAT4x3 | GLSL_IS_MAT4x4)) {
		// Rule 6
		if (type & GLSL_IS_MAT2x2) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC2, 2 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC2, 2 * arrSize);
			}
		} else if (type & GLSL_IS_MAT2x3) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC3, 2 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC3, 2 * arrSize);
			}
		} else if (type & GLSL_IS_MAT2x4) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC4, 2 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC4, 2 * arrSize);
			}
		} else if (type & GLSL_IS_MAT3x2) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC2, 3 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC2, 3 * arrSize);
			}
		} else if (type & GLSL_IS_MAT3x3) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC3, 3 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC3, 3 * arrSize);
			}
		} else if (type & GLSL_IS_MAT3x4) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC4, 3 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC4, 3 * arrSize);
			}
		} else if (type & GLSL_IS_MAT4x2) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC2, 4 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC2, 4 * arrSize);
			}
		} else if (type & GLSL_IS_MAT4x3) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC3, 4 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC3, 4 * arrSize);
			}
		} else if (type & GLSL_IS_MAT4x4) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC4, 4 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC4, 4 * arrSize);
			}
		}
	} else {
		// Rule 4
		unsigned int typeAlign = glslTypeBaseAlignment(type);
		if (typeAlign < vec4Align) {
			typeAlign = vec4Align;
		}
		return typeAlign * arrSize;
	}
}

inline unsigned int glslTypeBaseAlignment(GLSLType type) {
	if (type & GLSL_IS_SCALAR) { // Rule 1
		if (type & GLSL_IS_DOUBLE) {
			return 8;
		} else {
			return 4;
		}
	} else if (type & GLSL_IS_VEC2) { // Rule 2
		if (type & GLSL_IS_DOUBLE) {
			return 16;
		} else {
			return 8;
		}
	} else if (type & GLSL_IS_VEC3 || type & GLSL_IS_VEC4) { // Rule 2 and 3
		if (type & GLSL_IS_DOUBLE) {
			return 32;
		} else {
			return 16;
		}
	}
	// Rule 5
	else if (type & GLSL_IS_MAT2x2) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC2, 2);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC2, 2);
		}
	} else if (type & GLSL_IS_MAT2x3) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC3, 2);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC3, 2);
		}
	} else if (type & GLSL_IS_MAT2x4) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC4, 2);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC4, 2);
		}
	} else if (type & GLSL_IS_MAT3x2) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC2, 3);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC2, 3);
		}
	} else if (type & GLSL_IS_MAT3x3) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC3, 3);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC3, 3);
		}
	} else if (type & GLSL_IS_MAT3x4) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC4, 3);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC4, 3);
		}
	} else if (type & GLSL_IS_MAT4x2) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC2, 4);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC2, 4);
		}
	} else if (type & GLSL_IS_MAT4x3) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC3, 4);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC3, 4);
		}
	} else if (type & GLSL_IS_MAT4x4) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC4, 4);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC4, 4);
		}
	}
}

inline GLSLType glslTypeFromString(std::string typeStr) {
	// TODO more safety checks for wrong type strings?
	uint type = GLSL_UNKNOWN;
	uint shapeIndex = 1;
	switch (typeStr[0]) {
	case 'b':
		type |= GLSL_IS_BOOL;
		break;
	case 'i':
		type |= GLSL_IS_INT;
		break;
	case 'u':
		type |= GLSL_IS_UINT;
		break;
	case 'd':
		type |= GLSL_IS_DOUBLE;
		break;
	case 'f':
		return GLSL_FLOAT;
	case 'v':
	case 'm':
		shapeIndex = 0;
		type |= GLSL_IS_FLOAT;
		break;
	}

	switch (typeStr[shapeIndex]) {
	case 'v':
		switch (typeStr[shapeIndex + 3]) {
		case '2':
			type |= GLSL_IS_VEC2;
			break;
		case '3':
			type |= GLSL_IS_VEC3;
			break;
		case '4':
			type |= GLSL_IS_VEC4;
			break;
		}
		break;
	case 'm':
		switch (typeStr[shapeIndex + 3]) {
		case '2':
			switch (typeStr[shapeIndex + 5]) {
			case '2':
				type |= GLSL_IS_MAT2x2;
				break;
			case '3':
				type |= GLSL_IS_MAT2x3;
				break;
			case '4':
				type |= GLSL_IS_MAT2x4;
				break;
			}
			break;
		case '3':
			switch (typeStr[shapeIndex + 5]) {
			case '2':
				type |= GLSL_IS_MAT3x2;
				break;
			case '3':
				type |= GLSL_IS_MAT3x3;
				break;
			case '4':
				type |= GLSL_IS_MAT3x4;
				break;
			}
			break;
		case '4':
			switch (typeStr[shapeIndex + 5]) {
			case '2':
				type |= GLSL_IS_MAT4x2;
				break;
			case '3':
				type |= GLSL_IS_MAT4x3;
				break;
			case '4':
				type |= GLSL_IS_MAT4x4;
				break;
			}
			break;
		}
		break;
	default:
		type |= GLSL_IS_SCALAR;
		break;
	}
	
	return (GLSLType) type;
}

inline UniformLayoutType glslUniformLayoutTypeFromString(std::string typeStr) {
	switch (typeStr[0]) {
	case 'p':
		return UniformLayoutType::PACKED;
	case 's':
		if (typeStr[1] == 't') {
			return UniformLayoutType::STD140;
		} else {
			return UniformLayoutType::SHARED;
		}
		break;
	}
}
