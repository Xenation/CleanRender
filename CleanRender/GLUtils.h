#pragma once
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
