#pragma once
#include "gl3w.h"

unsigned int glTypeSize(GLenum type) {
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

bool glTypeIsInteger(GLenum type) {
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
